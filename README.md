# *c4 opening, C for Chess*
# Discord invite: [Official C4Chess Server](https://discord.com/invite/2ma5DeAFFq)

## A C project with (possible future) online multiplayer using SDL2.0, oriented for multiple devices (including homebrew projects)

![First impact](https://i.ibb.co/HgwvmXb/Cattura.png)
(old screenshot)

### The project is still under development, no roadmap or release date available publicly. Check [here](https://github.com/waifro/c4chess/projects/2) for the work to be done.
### The official server **c4server** (195.58.38.16) is running only on testnet till further advice.

## CLONE AND BUILD

Here's a quick guide on how-to clone and compile the program without much of a hassle, independently on which system your in.

We have `configure.ac` and `Makefile.am` to automate our builds. ~~We highly suggest to use tarball's from the Release page to build, otherwise~~ you end up with more bunch of not useful files. ~~Skip up to compilation setup (step n.4)~~

Run this sequences of commands for any of your device:

### **- Linux/Posix**

Dipendence's used: `git make gcc autoconf automake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev`

(for Windows builds also install `mingw64` and everything else required)

cloning:
```
git clone https://github.com/waifro/c4chess && cd c4chess
git submodule update --init
```

compilation:
```
aclocal
autoconf
automake --add-missing
./configure (to build for example 64-bit Windows executable's from WSL append this `--host=x86_64-w64-mingw32`)
make
```

### **- Windows 7/8.1/10/11**

there's so to much to cover just to "natively" compile from MSCode, Chocolatey, and/or other's, that you have to find out yourself how.
I suggest to use WSL and cross-compile for it (by following the 5 step's on POSIX guide).

### RUN
at the moment, theres a problem with handling the resources, but to workaround this, change directory to `output` and run the executable from there.

## LAST WORDS
if you want to know more information, or you want to contact me, feel free to send me a message at Discord at Waifro#4890, i will try to reply as soon as possible
Thanks for listening 👋
