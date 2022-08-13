# *c4 opening, C for Chess*

## A C project with (possible future) online multiplayer using SDL2.0, oriented for multiple devices (including homebrew projects)

![First impact](https://i.ibb.co/HgwvmXb/Cattura.png)
(old screenshot)

### The project is still under development, no roadmap or release date available publicly. Check [here](https://github.com/waifro/c4chess/projects/2) for the work to be done.

## CLONE AND BUILD

OLD:
~~visit https://git-scm.com/book/en/v2/Git-Tools-Submodules for more info
- Windows: run `make`
- Linux: comment the line inside the Makefile where it asks it. (first few ~~lines), then run `make`
- MacOS: same thing, then run `make`~~

NEW:
Here's a quick guide on how-to compile the program without much of a hassle, independently on which system your in.

We have `configure.ac` and `Makefile.am` to automate our builds. ~~We highly suggest to use tarball's from the Release page to build, otherwise~~ you end up with more bunch of not useful files. ~~Skip up to compilation setup (step n.4)~~

Run this sequences of commands for any of your device:

- from POSIX devices
Dipendence's used: `make`, `gcc`, `libsdl2-dev, libsdl2-image-dev`, `libsdl2-ttf-dev`
(for Windows builds also install `mingw64` and everything else required)

1. `~$ aclocal`
2. `~$ autoconf`
3. `~$ automake --add-missing`
4. `~$ ./configure` (for example to build 64-bit Windows builds using WSL append `--host=x86_64-w64-mingw32`)
5. `~$ make`

- for Windows:
there's so to much to cover just to "natively" compile a program, that you have to find out yourself for it, i suggest to use WSL and cross-compile for it (by following the 5 step's on POSIX guide).

### RUN
at the moment, theres a problem with handling the resources, but to workaround this, change directory to `output` and run the executable from there.

## in depth info about my current development

Im working on making this project more suitable. I like this project, and as of right now the code does somehow work.
The only problem is that right now the code is written very badly. Let me clear this statement:

The code is written good for a small project, and this one is not in this category. I need to separate a lot of hard-code to something more useful for any long term idea/concept. In this case, i would like to make it more good looking. But there are many troubles, as of many global variables still running under memory, so problably there are a lot of use-after-free vulnerabilities, for example, after some time playing with it, the game crashes, under segmentation fault.

My idea is to separate make another version of this game, for every day use (more details soon:tm:). So, to start do it, i will open a new branch of this repo, with a lot of reimplamantation in a better way, for example, how to manage a chessboard, the core, where at the end of the day, every piece of code goes throw, managing better the pointers, unallocating the pieces initialized, create a more friendly code to work better, even for later use. As of right now, the game works, badly, because i didnt know where to start.

Anyway, if you like my game, or the idea behind, and you want to contribute, heres a list i would likely need for this game:

- Server management
- Designer
- Mechanics management
- ~~A team~~ :)

if you want to know more information, or you want to contact me, feel free to send me a message at Discord at Waifro#4890, i will try to reply as soon as possible
Thanks for listening 👋
