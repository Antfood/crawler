# Crawler

This program specifically targets Antfood's dropbox. So in order to run you must have it synced to your computer.
Failing to find a `Dropbox (Antfood, LLC)` directory in your home folder the program will quit immediately 
warning the user.

This app is designed to be used in OSX only. 

## Usage
You can launch the app by double-clicking on its icon on a finder window. This will prompt a terminal
which will run serve the user interface.

Alternatively, you can open your favorite terminal emulator and execute from the command line. For example, if the 
app is in your Desktop

       $ cd Desktop
       $ ./crawler

## Build
Create a `build` directory and the script `build.sh` to compile.

        ./build.sh --build 

Re-builds make files. You should run this the first time you clone this project.

        ./build.sh --run 

Compile and run the binary.

        ./build.sh --release

Compiles and copy the binary to the root of the `Dropbox (Antfood, LLC)`.
