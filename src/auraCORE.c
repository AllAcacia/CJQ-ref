/*
 * Filename: auraCORE.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles audio in
 * different threads.
 * 
 * Adapted from devkiPro
 * OGG Vorbis example code.
 */


#include "auraCORE.h"


// ---- DEFINITIONS ----
static const int THREAD_STACK_SZ = 32 * 1024;    // 32kB stack for audio thread
static const u16 PCM_SAMPLE_TIME_LEN = 120; // milliseconds
static const u64 AUDIO_THREAD_SLEEP_TIME = (1000000 * PCM_SAMPLE_TIME_LEN) / AUDIO_COLLECTION_MAX; // nanoseconds
// ---- END DEFINITIONS ----

static VorbisPlayback AUD_COLLECTION[AUDIO_COLLECTION_MAX];
static bool CHN_COLLECTION[NDSP_CHANNELS_COUNT] = {false};  // "is channel i in use?"

static CharlesThread charles; // Good ol' Charles


void AuraCORE_Init(void)
{
    // Initialise platform features
    romfsInit();
    // Setup NDSP
    ndspInit();
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);

    for(size_t i = 0; i < NDSP_CHANNELS_COUNT; i += 1) {
        CHN_COLLECTION[i] = false;
        ndspChnInitParams(i);
        ndspChnReset(i);
    }

    AuraCORE_AudioSlotsClear();

    AuraCORE_CharlesInit();

    printf("Initialised AuraCORE\n");
}


void AuraCORE_Exit(void)
{
    // Signal audio threads to quit
    for (size_t i = 0; i < AUDIO_COLLECTION_MAX; i += 1) {
        VorbisPlayback* aud = &AUD_COLLECTION[i];
        if ((aud->in_use) || (aud->threadId)) {
            aud->s_quit = true;
            LightEvent_Signal(&aud->s_event);
            AuraCORE_AudioSlotExit(aud);
        }
    }

    AuraCORE_CharlesExit();

    // Cleanup audio things and de-init platform features
    ndspExit();
    romfsExit();

    printf("Exited AuraCORE\n");
}


int AuraCORE_CharlesInit(void)
{
    // Init Charles (cleanup thread)
    charles.s_quit = false;
    charles.flag = false;
    charles.arr_i = 0;
    LightEvent_Init(&charles.s_event, RESET_ONESHOT);

    int32_t cleanup_priority = 0x30;
    // get main thread's threadId
    svcGetThreadPriority(&cleanup_priority, CUR_THREAD_HANDLE);
    cleanup_priority += 1; // Lower priority
    cleanup_priority = cleanup_priority < 0x18 ? 0x18 : cleanup_priority;
    cleanup_priority = cleanup_priority > 0x3F ? 0x3F : cleanup_priority;

    // Start the cleanup thread
    charles.cleanupThreadId = threadCreate(AuraCORE_CharlesThread, NULL,
                                THREAD_STACK_SZ, cleanup_priority,
                                THREAD_AFFINITY, false);
    
    LightEvent_Signal(&charles.s_event);

    printf("Deployed Charles\n");

    return EXIT_SUCCESS;
}


int AuraCORE_CharlesExit(void)
{
    // Call back Charles (cleanup thread)
    charles.s_quit = true;
    LightEvent_Signal(&charles.s_event);
    threadJoin(charles.cleanupThreadId, UINT64_MAX);
    threadFree(charles.cleanupThreadId);

    printf("Called Charles back\n");

    return EXIT_SUCCESS;
}


void AuraCORE_CharlesThread(void *const nul_)
{
    (void)nul_;  // Unused

    while (!charles.s_quit) {
        if (charles.flag) {
            for (size_t i = 0; i < AUDIO_COLLECTION_MAX; i += 1) {
                VorbisPlayback* aud = &AUD_COLLECTION[i];
                if (aud->s_quit) {
                    threadJoin(aud->threadId, UINT64_MAX);
                    threadFree(aud->threadId);
                    aud->threadId = 0;
                    aud->s_quit = false;
                    aud->in_use = false;
                    printf("Charles has cleaned up the thread for slot %d\n", aud->slotId);
                }
            }
            charles.flag = false;
        }
        printf("Charles is now taking a nap...\n");
        LightEvent_Wait(&charles.s_event);
    }

    threadExit(0);
}


VorbisFileData AuraCORE_VorbisOpen(const char* PATH)
{
    VorbisFileData aud_data;

    // Open the Ogg Vorbis audio file
    OggVorbis_File vorbisFile;
    FILE* fh = fopen(PATH, "rb");
    int error = ov_open(fh, &vorbisFile, NULL, 0);
    aud_data.init_success = true;
    if(error) {
        // Only fclose manually if ov_open failed.
        // If ov_open succeeds, fclose happens in ov_clear.
        fclose(fh);
        aud_data.init_success = false;
    }

    // Assign vorbis file
    aud_data.file = vorbisFile;

    // Grab file metadata
    vorbis_info* vi = ov_info(&aud_data.file, -1);
    aud_data.rate = vi->rate;
    aud_data.channels = vi->channels;
    s64 pcm_num = (s64)ov_pcm_total(&aud_data.file, -1);
    aud_data.pcm_num = pcm_num * aud_data.channels * 2;
    aud_data.in_use = false;

    return aud_data;
}


int AuraCORE_VorbisClose(VorbisFileData* aud_data)
{
    if (!ov_clear(&aud_data->file)) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}


void AuraCORE_AudioSlotsClear(void)
{
    for (size_t i = 0; i < AUDIO_COLLECTION_MAX; i += 1) {
        VorbisPlayback* aud = &AUD_COLLECTION[i];

        aud->aud_data = NULL;
        aud->slotId = i;
        aud->chnId = 0;
    }
    printf("Cleared all audio slots\n");
}


bool AuraCORE_AudioSlotInit(VorbisPlayback* aud)
{
    // Setup NDSP
    ndspChnSetInterp(aud->chnId, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(aud->chnId, aud->aud_data->rate);
    ndspChnSetFormat(aud->chnId, aud->aud_data->channels == 1
        ? NDSP_FORMAT_MONO_PCM16
        : NDSP_FORMAT_STEREO_PCM16);

    // Allocate audio buffer
    // 120ms buffer
    const size_t SAMPLES_PER_BUF = (aud->aud_data->rate * PCM_SAMPLE_TIME_LEN) / 1000;
    // mono (1) or stereo (2)
    const size_t CHANNELS_PER_SAMPLE = aud->aud_data->channels;
    // s16 buffer
    const size_t WAVEBUF_SIZE = SAMPLES_PER_BUF * CHANNELS_PER_SAMPLE * sizeof(s16);
    const size_t bufferSize = WAVEBUF_SIZE * ARRAY_SIZE(aud->wav_buf);
    aud->aud_buf = (int16_t *)linearAlloc(bufferSize);
    if(!aud->aud_buf) {
        return false;
    }

    // Setup waveBufs for NDSP
    memset(&aud->wav_buf, 0, sizeof(aud->wav_buf));
    int16_t* buffer = aud->aud_buf;

    for(size_t i = 0; i < ARRAY_SIZE(aud->wav_buf); ++i) {
        aud->wav_buf[i].data_vaddr = buffer;
        aud->wav_buf[i].nsamples   = WAVEBUF_SIZE / sizeof(buffer[0]);
        aud->wav_buf[i].status     = NDSP_WBUF_DONE;

        buffer += WAVEBUF_SIZE / sizeof(buffer[0]);
    }

    return true;
}


void AuraCORE_AudioSlotExit(VorbisPlayback* aud)
{
    if (aud->aud_data != NULL) {
        aud->aud_data->in_use = false;
        aud->aud_data = NULL;
    }

    ndspChnWaveBufClear(aud->chnId);
    ndspChnReset(aud->chnId);

    if (aud->aud_buf != NULL) {
        linearFree(aud->aud_buf);
        aud->aud_buf = NULL;
    }

    charles.flag = true;
    LightEvent_Signal(&charles.s_event);
}


void AuraCORE_StopAllPlayback(void)
{
    for (size_t i = 0; i < AUDIO_COLLECTION_MAX; i += 1) {
        // Signal all threads to close
        if (AUD_COLLECTION[i].in_use) {
            AUD_COLLECTION[i].s_quit = true;
            LightEvent_Signal(&AUD_COLLECTION[i].s_event);
        }
    }

    bool ready_to_close = false;
    do {
        ready_to_close = true;
        for (size_t i = 0; i < AUDIO_COLLECTION_MAX; i += 1) {
            if (AUD_COLLECTION[i].s_quit) {
                ready_to_close = false;
                break;
            }
        }
        svcSleepThread(1000000); // 1ms to let other threads die
    } while (!ready_to_close);
}


void AuraCORE_PausePlayback(VorbisPlayback* aud)
{
    aud->is_paused = true;
    ndspChnWaveBufClear(aud->chnId);
}


void AuraCORE_ResumePlayback(VorbisPlayback* aud)
{
    aud->is_paused = false;
}


bool AuraCORE_GetIsPlaybackPaused(VorbisPlayback* aud)
{
    return aud->is_paused;
}


bool AuraCORE_FillBuffer(VorbisPlayback* aud, ndspWaveBuf *waveBuf_)
{
    if (aud->aud_data != NULL) {
        // Decode (2-byte) samples until our waveBuf is full
        int totalBytes = 0;
        while(totalBytes < waveBuf_->nsamples * sizeof(s16)) {
            int16_t* buffer = waveBuf_->data_pcm16 + (totalBytes / sizeof(s16));
            const size_t bufferSize = (waveBuf_->nsamples * sizeof(s16) - totalBytes);

            // Decode bufferSize bytes from vorbisFile_ into buffer,
            // storing the number of bytes that were read (or error)
            const int bytesRead = ov_read(&aud->aud_data->file, (char *)buffer, bufferSize, NULL);
            if (bytesRead <= 0) {
                break;
            }
            
            totalBytes += bytesRead;
        }

        // If no samples were read in the last decode cycle, we're done
        if(totalBytes == 0) {
            return false;
        }

        // Pass samples to NDSP
        // this calculation will make a number <= the previous nsamples
        // = for most cases
        // < for the last possible chunk of the file, which may have less samples before EOF
        // after which we don't care to recover the length
        waveBuf_->nsamples = totalBytes / sizeof(s16);
        DSP_FlushDataCache(waveBuf_->data_pcm16, totalBytes);
        ndspChnWaveBufAdd(aud->chnId, waveBuf_);

        return true;
    } else {
        return false;
    }
}


void AuraCORE_AudioCallback(void* const nul_)
{
    VorbisPlayback* aud = (VorbisPlayback*)nul_;

    if(aud->s_quit) { // Quit flag
        return;
    }
    
    LightEvent_Signal(&aud->s_event);
}


void AuraCORE_AudioThread(void *const nul_)
{
    VorbisPlayback* aud = (VorbisPlayback*)nul_;

    printf("Audio started playing on slot %d, channel %d\n", aud->slotId, aud->chnId);

    while(!aud->s_quit) { // Whilst the quit flag is unset,
                          // search our waveBufs and fill any that aren't currently
                          // queued for playback (i.e, those that are 'done')
        for(size_t i = 0; i < ARRAY_SIZE(aud->wav_buf); ++i) {
            if(aud->wav_buf[i].status != NDSP_WBUF_DONE || aud->is_paused) {
                continue;
            }
            
            if(!AuraCORE_FillBuffer(aud, &aud->wav_buf[i])) { // Playback complete or file unusable
                aud->s_quit = true;
                goto finish;
            }
        }
        // Wait for a signal that we're needed again before continuing,
        // so that we can yield to other things that want to run
        // (Note that the 3DS uses cooperative threading)
        // LightEvent_Wait(&aud->s_event);

        svcSleepThread(AUDIO_THREAD_SLEEP_TIME);
    }

finish:
    
    CHN_COLLECTION[aud->chnId] = false;
    AuraCORE_AudioSlotExit(aud);

    printf("Audio in slot %d stopped playing\n", aud->slotId);

    threadExit(0);
}


VorbisPlayback* AuraCORE_AudioAppend(VorbisFileData* aud_data)
{
    s16 free_slot = AuraCORE_FindFreeAudioSlot();
    s16 free_channel = AuraCORE_FindFreeNDSPchn();

    if ((free_slot >= 0) && (free_channel >= 0) && (!aud_data->in_use)) { // if -1 or less, none are free
        CHN_COLLECTION[free_channel] = true;

        VorbisPlayback* aud = &AUD_COLLECTION[free_slot];
        aud->aud_data = aud_data;
        aud_data->in_use = true;
        aud->chnId = free_channel;
        aud->slotId = free_slot;
        aud->is_paused = false;
        aud->in_use = true;
        aud->s_quit = false;

        // Revert pointer to file data
        ov_pcm_seek(&aud_data->file, 0);

        ndspSetCallback(AuraCORE_AudioCallback, aud);

        // Setup LightEvent for synchronisation of audioThread
        LightEvent_Init(&aud->s_event, RESET_ONESHOT);

        // Attempt audioInit
        if(!AuraCORE_AudioSlotInit(aud)) {
            ov_clear(&aud_data->file);
            ndspExit();
            romfsExit();
            return NULL;
        }

        // Spawn audio thread

        // Set the thread priority to the main thread's priority ...
        int32_t priority = 0x30;
        svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
        // ... then subtract 1, as lower number => higher actual priority ...
        priority += 1;
        // ... finally, clamp it between 0x18 and 0x3F to guarantee that it's valid.
        priority = priority < 0x18 ? 0x18 : priority;
        priority = priority > 0x3F ? 0x3F : priority;

        // Start the thread, passing the address of our vorbisFile as an argument.
        aud->threadId = threadCreate(AuraCORE_AudioThread, aud,
                                    THREAD_STACK_SZ, priority,
                                    THREAD_AFFINITY, false);
        
        LightEvent_Signal(&aud->s_event);
        return aud;
    } else {
        printf("No free slots/channels or file already in use\n");
        return NULL;
    }
}


s16 AuraCORE_FindFreeAudioSlot(void)
{
    s16 free_slot = -1;
    for (size_t i = 0; i < AUDIO_COLLECTION_MAX; i += 1) {
        VorbisPlayback* aud = &AUD_COLLECTION[i];
        if (!aud->in_use) {
            free_slot = i;
            break;
        }
    }
    return free_slot;
}


s8 AuraCORE_FindFreeNDSPchn(void)
{
    s8 free_channel = -1;

    for (size_t i = 0; i < NDSP_CHANNELS_COUNT; i += 1) {
        if (!CHN_COLLECTION[i]) {
            free_channel = i;
            break;
        }
    }
    return free_channel;
}


const char* AuraCORE_VorbisStrError(int error)
{
    switch(error) {
        case OV_FALSE:
            return "OV_FALSE: A request did not succeed.";
        case OV_HOLE:
            return "OV_HOLE: There was a hole in the page sequence numbers.";
        case OV_EREAD:
            return "OV_EREAD: An underlying read, seek or tell operation "
                   "failed.";
        case OV_EFAULT:
            return "OV_EFAULT: A NULL pointer was passed where none was "
                   "expected, or an internal library error was encountered.";
        case OV_EIMPL:
            return "OV_EIMPL: The stream used a feature which is not "
                   "implemented.";
        case OV_EINVAL:
            return "OV_EINVAL: One or more parameters to a function were "
                   "invalid.";
        case OV_ENOTVORBIS:
            return "OV_ENOTVORBIS: This is not a valid Ogg Vorbis stream.";
        case OV_EBADHEADER:
            return "OV_EBADHEADER: A required header packet was not properly "
                   "formatted.";
        case OV_EVERSION:
            return "OV_EVERSION: The ID header contained an unrecognised "
                   "version number.";
        case OV_EBADPACKET:
            return "OV_EBADPACKET: An audio packet failed to decode properly.";
        case OV_EBADLINK:
            return "OV_EBADLINK: We failed to find data we had seen before or "
                   "the stream was sufficiently corrupt that seeking is "
                   "impossible.";
        case OV_ENOSEEK:
            return "OV_ENOSEEK: An operation that requires seeking was "
                   "requested on an unseekable stream.";
        default:
            return "Unknown error.";
    }
}
