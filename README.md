# Nozomi Engine
My very own game engine, wrote in C with the purpose of being a simpler game engine for me to use for my games.

## Questions / Answers
### Why are you writing my own game engine instead of using X, Y, or Z?
The purpose for me writing my own engine instead of using [Godot](https://godotengine.org/), [Unity](https://unity.com/), [GameMaker](https://gamemaker.io/en), or other engines is
that I feel that they all over-complicate everything for me and have their own scripting languages that I simply cannot
work with. Writing my own engine also gives me complete control over what platforms I can support, unlike using other engines.

### What are your goals with Nozomi Engine?
My goals that I have laid out are to successfully make a game engine that runs on all of my favorite platforms at well enough speeds
with compatibility for legacy hardware as well as newer concepts. I want to create the JADEFRACTURE saga using Nozomi Engine while leaving
leeway for me to make other simpler games as well. I want Nozomi Engine to support 2D, 3D, Mode 7-esque graphics, and much more without being
too complex for myself to use it.

### Can I use Nozomi Engine for my own projects?
The idea is that I want to keep Nozomi Engine usage to myself until I feel comfortable enough with its capabilities for others to use it.
I may give special permission to use the engine for small projects as long as they're commercial-free, as this is kind-of my own passion
project that I want to turn into my career after all.

## Building
Should you want to tinker with Nozomi Engine anyways (and perhaps contribute to it?), 
I will list what you need to compile it below!

There's a bash script called `build_all.sh`, which is only designed to quicken my building process.
You may use it yourself if you want, but don't expect it to work.

There's also a folder full of assets that you can copy the data folder from to test the engine executable.

### Windows
To compile Nozomi Engine on Windows, you have two options:
- Compiling with MinGW
- Compiling with WSL(2)
#### Compiling with MinGW
As I no longer daily drive Windows, I cannot ensure that these instructions will work quite exactly, 
but please bear with me. Do note that I'm not a professional at this either.

In order to compile Nozomi Engine with MinGW, you'll want to get [MSYS2](https://www.msys2.org/), and install the packages for SDL2, and SDL_Mixer, as well as any build tools.

From there it's a matter of cloning the repository, and moving into the root directory of the repository. It should contain folders like `src`, `tools`, and `assets`.

Assuming you have all the dependencies, run `make`, it should target Windows/MinGW by default.
If it compiles successfully, you should have a `<game_name>.exe` executable in the `bin/SDL` folder.
#### Compiling with WSL(2)
Compiling with WSL(2) is the preferred method for compiling Nozomi Engine for Windows, but it requires you
to have WSL(2) to begin with, experience with cross-compiling, and knowing where to put SDL libraries.

Due to the complex nature, I won't go into details on how to compile it, but do know it's possible.
If you're compiling for Windows under WSL(2), make sure you specify `make LINUX_WIN=1` instead of `make`.
### Linux
To compile Nozomi Engine on Linux, you also have two options:
- Compiling with SDL
- Compiling with GLFW (and SDL_Mixer for audio)
#### Compiling with SDL
Compiling Nozomi Engine with SDL depends on your Linux distribution, but for our purposes, we're using Ubuntu/Debian.
First, install required dependencies by opening a terminal and copy-pasting the below:

`sudo apt install -y git build-essential make libsdl2-dev libsdl2-mixer-dev`

Using Git, clone the Nozomi Engine repository:

`git clone https://github.com/supeki/Nozomi_Engine`

When it's done cloning the repository, move into the repo folder and build Nozomi Engine:

`cd Nozomi_Engine && make LINUX=1`

Once done, you can test your executable by moving into `bin/SDL` and running `./<game_name>`.
#### Compiling with GLFW
###### If you prefer using GLFW instead of SDL, you'll still need `libsdl2-mixer-dev` installed until another solution for audio is found.
Compiling Nozomi Engine with GLFW also depends on your Linux distribution, so we're still going to just use Ubuntu/Debian instructions.
First, install required dependencies by running the below in your terminal:

`sudo apt install -y git build-essential make libglfw3-dev libsdl2-mixer-dev`

Using Git, clone the Nozomi Engine repository:

`git clone https://github.com/supeki/Nozomi_Engine`

When it's done, move into the repo folder and build Nozomi Engine for GLFW:

`cd Nozomi_Engine && make LINUX=1 GLFW=1 SDL=0`

Unlike SDL builds, GLFW builds should show under `bin/GLFW`.
### Other Platforms
While Nozomi Engine supports other platforms like the Nintendo DS and Sony Playstation Portable,
instructions on how to compile for those platforms will not be a concern as of now.

## Copyright & Misc. Information
Nozomi Engine is the intellectual property of Marilyn Nozomi (aka [supeki](https://github.com/supeki)).<br>
Nozomi Engine and other related works are Copyright © 2026 Marilyn Nozomi

All non-original code belongs to their authors, contributors own the rights to their own code until it is merged into Nozomi Engine.
If you suspect that your code, or other assets are being misused, please contact me at `admin@maril.dev`, or on Discord at `mysticrealmcommunityedition`.

I personally also vow that all code in Nozomi Engine is wrote by real people, and that all other misc. assets are also made by hand.
If you suspect that AI was used in the making of any part of Nozomi Engine, 
please bring it up to me ASAP, I find GenAI very offensive to the skills of real people who *do and don't* do this for free.
