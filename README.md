![CJQ Logo](/gfx/promo/CJQ_text_smol.png)
# Card-Jitsu Quartet

--- 

## About
Master the three elements within Card-Jitsu on the 3DS! If you'd like to join the community Discord, the server link is here: https://discord.gg/2aw3GXCU8D.  

---

## aCORE Engine
This application runs under an engine infrastructure of my design, with dependency on libctru. There are three main CORE modules and several sub-modules to support them and the software they are integrated in. These CORE modules were created out of the necessity to simplify the workflows from devkitPro's [3ds-examples](https://github.com/devkitpro/3ds-examples), most notably [this one](https://github.com/devkitPro/3ds-examples/tree/master/audio/ogg-vorbis-decoding)... the others I followed were more intuitive!
### RendCORE (Graphics Manager)
Allows appending of expanded sprite or text object types to a collection that is rendered every frame. A promising feature of this allows you to assign layers, and order priorities within each layer, and you can also delegate a target screen for each object. Text objects can be assigned to either the static or dynamic buffer (static is manually cleared, dynamic is automatically cleared on refresh). Deleting sprite or text objects from the collection is easy too!  
### AuraCORE (Audio Manager)
Allows appending of audio files to a collection of slots. Each slot will have a designated "thread" that frequently runs to fill DSP data to the 3DS' audio buffers. To use, you simply open the audio file with the given functions and then submit it (which returns a pointer to the slot which you can track). The main helper thread within AuraCORE, dubbed Charles, helps slots to be cleared and ready for use immediately after an audio file finishes playing!
### NetCORE (Network Manager)
Handles inter-console connectivity. *(to be implemented)*  

---

## How to Compile
An installation of a C compiler (such as GCC), Make, and devkitPro (libctru) is required. Run "make all" in directory to compile, and "make clean" to remove all compiled files.

---

## How to Play
Drag and drop "Card-Jitsu-Quartet.3dsx" into the root/3ds directory of your (modded with Luma3DS) 3DS console's SD card. Open the Homebrew Launcher on your 3DS, then find the title and press "A"!

If you want to use the "3dslink" feature included with devkitPro, run "make boot 3DSIP_ADDR=?", where "?" refers to the IP address displayed on the Homebrew menu.

---

## Additional Assets Required
You will need to acquire your own soundtrack files for compilation. The filenaming convention I used is below, **under the "/romfs/sfx/ost" directory**. **IMPORTANT:** I recommend using Audacity to apply the following changes to each audio file: convert stereo->mono channels, resample->32728Hz *(which is the 3DS native rate)*, export as OGG Vorbis *(the Vorbis container is usually the default when exporting as .ogg)*.
- BaseBattle.ogg
- BaseDojo.ogg
- BaseDojoRemix.ogg
- FireBattle.ogg
- FireDojo.ogg
- SnowBattle.ogg
- SnowDojo.ogg
- SnowTusk.ogg
- WaterBattle.ogg
- WaterDojo.ogg

---

## In-Game Notes
There is very little to no gameplay implemented at this stage, as the bare basics are being implemented (this is my first time working with devkitPro). You will start at the main menu; there are options to open Card-Jitsu and its elemental variants, but only the base title has content being developed for at this moment. For your entertainment, going into any other menu loads up a cute stickbug animation!

---
## License
For software licensing, please refer to the provided [LICENSE.md](/LICENSE.md).  

For asset licensing, no graphical, audio, or font assets from the originating [public](https://github.com/AllAcacia/CJQ-ref) and [private](https://github.com/AllAcacia/CJQ) repositories are covered under the MIT license unless explicitly stated otherwise. These assets may not be redistributed, reused, included, or modified in derivative works unless permission is explicitly granted by the copyright holder(s). 
### Disclaimer
This project is not associated with Disney's original Club Penguin or Card-Jitsu copyrights, nor are any infringements intended. No Club Penguin source files are saved into this repository, any imitations are created by the developer(s) of the project. This project is non-commercial and is intended to improve my skills in embedded software development and relive any nostalgia fans have had for the original game.
