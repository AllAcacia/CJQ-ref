![CJQ Logo](/gfx/promo/CJQ_text_smol.png)
## About
Master the three elements within Card-Jitsu on the 3DS! If you'd like to join the community Discord, the server link is here: https://discord.gg/2aw3GXCU8D. Please note that this is a restricted copy of the actual repository I work on: **no graphical or audio assets are included**. **This likely means you won't be able to compile the game**, but you are welcome to reference or use some of my code (with attribution). This is for purely educational reasons, for those wanting to learn to develop 3DS Homebrew. **More files will be uploaded as I go, I will try to remember to announce new updates via my Discord server**.

**Another note:** I do use the template Makefile used in most of the devkitPro examples but I made another command "make boot", which allows you to compile and send the game to the Homebrew launcher on your console (reference How to Play).

## How to Compile
An installation of a C compiler (such as GCC), Make, and devkitPro (libctru) is required. Run "make all" in directory to compile, and "make clean" to remove all compiled files.

## How to Play
Drag and drop "Card-Jitsu-Quartet.3dsx" into the root/3ds directory of your (modded with Luma3DS) 3DS console's SD card. Open the Homebrew Launcher on your 3DS, then find the title and press "A"!

If you want to use the "3dslink" feature included with devkitPro, run "make boot 3DSIP_ADDR=?", where "?" refers to the IP address displayed on the Homebrew menu.

## In-Game Notes
There is very little to no gameplay implemented at this stage, as the bare basics are being implemented (this is my first time working with devkitPro).

# Disclaimer
This project is not associated with Disney's original Club Penguin or Card-Jitsu copyrights, nor are any infringements intended. No Club Penguin source files are saved into this repository, any imitations are created by the developer(s) of the project. This project is non-commercial and is intended to improve my skills in embedded software development and relive any nostalgia fans have had for the original game.
