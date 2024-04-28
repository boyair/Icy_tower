# Icy_tower
a clone for icy tower made using c++ and SDL2.
the project includes a simple physics built from scratch and an animation system built using SDL textures.


# regular installation
go to Releases and download the correct build for your os.
than extract it and launch the executable in the folder Icy_tower.
## warning:
Do not change file names and relative locations.
*replacing a texture with a different file with the same name is possible :)*


# installation from source
## required dependancies for linux:
python, premake5,gcc compiler, SDL2, SDL2_ttf, SDL2_image, SDL2_mixer

## instructions for linux:
install SDL2 packages for your distribution

arch based:
```bash
sudo pacman -S sdl2 sdl2-image sdl2-ttf sdl2-mixer sdl2-devel sdl2-image-devel sdl2-ttf-devel sdl2-mixer-devel
```
debian/ubuntu based:
```bash
sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-ttf-2.0-0 libsdl2-mixer-2.0-0 libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```
fedora based:
```bash
sudo dnf install SDL2 SDL2_image SDL2_ttf SDL2_mixer SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
```

then install python, premake5, gcc from your package manger.

clone the repo using  "git clone https://github.com/boyair/Icy_tower.git --recursive" (no quotes)
run the build script by typing "python build.py" from project directory and follow the instructions.




## required dependancies for windows:
python interpreter and visual studio 2022

## instructions for windows:
download and install python.
download visual studio 2022 and install with c++ checkbox.

clone the repo using  "git clone https://github.com/boyair/Icy_tower.git --recursive" (no quotes)
run the build script by typing "python build.py" from project directory and open the visual studio solution.

# Known issues:
right now on windows, if you resize the window during gameplay
platform textures dissapear but get restored after losing and replaying.
