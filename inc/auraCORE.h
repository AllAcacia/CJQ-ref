/*
 * Filename: auraCORE.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles audio in a
 * different thread.
 */


#ifndef AURACORE_H
#define AURACORE_H

#include <tremor/ivorbisfile.h>
#include <tremor/ivorbiscodec.h>
#include <3ds.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define NDSP_CHANNELS_COUNT 24  // Number of NDSP channels
#define AUDIO_COLLECTION_MAX 24 // Number of audios queued allowed
#define THREAD_AFFINITY -1      // Execute thread on any core
#define THREAD_CORE_AUDIO 1

typedef struct {
    OggVorbis_File file; // .ogg file
    s64 pcm_num;         // num PCM samples
    float rate;          // typically 32768 Hz
    size_t channels;     // num channels; mono (1) / stereo (2)
    bool init_success;   // determines if obj init was successful
    bool in_use;         // is this file already in use in this module?
} VorbisFileData;


typedef struct {
    VorbisFileData* aud_data; // VorbisFileData struct
    Thread threadId;         // ThreadID
    s16 slotId;              // Index in audio collection
    int chnId;               // channel number
    LightEvent s_event;      // LightEvent
    bool s_quit;             // Flag for thread to quit
    bool is_paused;          // Currently playing?
    bool in_use;             // slot occupancy
    int16_t* aud_buf;        // a buffer
    ndspWaveBuf wav_buf[3];  // a buffer
} VorbisPlayback;


typedef struct {
    Thread cleanupThreadId; // ThreadID
    bool flag;              // Flag to signal attention required
    LightEvent s_event;     // LightEvent
    bool s_quit;            // Flag for thread to quit
    
    // Record of threads
    Thread threadIds_arr[AUDIO_COLLECTION_MAX];
    u8 arr_i;
} CharlesThread;


// AuraCORE module initialisation
void AuraCORE_Init(void);

// AuraCORE module cleanup
void AuraCORE_Exit(void);

// Deploy Charles
int AuraCORE_CharlesInit(void);

// Call back Charles
int AuraCORE_CharlesExit(void);

// Charles' own (cleanup) thread. Makes sure threads get joined and freed.
void AuraCORE_CharlesThread(void *const nul_);

// Opens a Vorbis file and writes its metadata a VorbisFileData object
VorbisFileData AuraCORE_VorbisOpen(const char* PATH);

// Closes a Vorbis file from a VorbisFileData object
int AuraCORE_VorbisClose(VorbisFileData* aud_data);

// Audio initialisation code; everything gets set to 0
void AuraCORE_AudioSlotsClear(void);

// Audio slot initialisation code
// This sets up NDSP and our primary audio buffer
bool AuraCORE_AudioSlotInit(VorbisPlayback* aud);

// Audio slot de-initialisation code
// Frees the primary audio buffers
void AuraCORE_AudioSlotExit(VorbisPlayback* aud);

// Stops all audio playback
void AuraCORE_StopAllPlayback(void);

// Pauses the specified audio slot's playback
void AuraCORE_PausePlayback(VorbisPlayback* aud);

// Resumes the specified audio slot's playback
void AuraCORE_ResumePlayback(VorbisPlayback* aud);

// Returns whether the current audio slot is playing
bool AuraCORE_GetIsPlaybackPaused(VorbisPlayback* aud);

// Main audio decoding logic
// This function pulls and decodes audio samples from vorbisFile_ to fill waveBuf_
bool AuraCORE_FillBuffer(VorbisPlayback* aud, ndspWaveBuf *waveBuf_);

// NDSP audio frame callback
// This signals the audioThread to decode more things
// once NDSP has played a sound frame, meaning that there should be
// one or more available waveBufs to fill with more data.
void AuraCORE_AudioCallback(void *const nul_);

// Audio thread
void AuraCORE_AudioThread(void *const nul_);

// Adds audio file to be played by providng a filepath
VorbisPlayback* AuraCORE_AudioAppend(VorbisFileData* aud_data);

// Finds a free audio slot
s16 AuraCORE_FindFreeAudioSlot(void);

// Finds a free NDSP channel to play on
s8 AuraCORE_FindFreeNDSPchn(void);

// Retrieve strings for libvorbisidec errors
const char* AuraCORE_VorbisStrError(int error);

#endif // AURACORE_H