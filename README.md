# Crawler

Crawler is a user-friendly terminal application that will help you find library recordings with the wrong format 
filename and provide a quick way to fix it. No commands are required — the program will prompt you
with a text-based user interface and walk you through the whole process.

![crawler](https://github.com/Antfood/crawler/blob/main/img/crawler.png?raw=true)

Crawler runs on OSX only.

## Dropbox

The program specifically targets Antfood's dropbox. So in order to run it you must have dropbox synced to your computer.
Failing to find a `Dropbox (Antfood, LLC)` directory in your home directory, crawler will quit immediately while
warning the user.

## Usage
You can launch the app by double-clicking on its icon on a finder window. This will prompt a terminal
which will run serve the user interface.

Alternatively, you can open your favorite terminal emulator and execute from the command line. For example, if the 
app is in your Desktop

       $ cd Desktop
       $ ./crawler

## Terminal
The terminal is the main window for user interaction with crawler. 
So depending on the terminal program you will have a different experience. 


OSX terminal is great but offers only very basic color configuration. If you are having
trouble reading text prompts you can adjust the colors in preferences. Tick all the preferences below.

![crawler](https://github.com/Antfood/crawler/blob/main/img/terminal.png?raw=true)

For a better experience it is recommended to use a terminal emulator with 24-bit color such as [iterm2](`https://iterm2.com/`).
To launch crawler in iterm2 right click on Crawler then get info and change `Open with` to iterm2.


![crawler](https://github.com/Antfood/crawler/blob/main/img/iterm2.png?raw=true)


## Build

You must have `Cmake` to build this project. Use the script `build.sh` to build and compile.

        ./build.sh --build 

Re-builds make files. You should run this the first time you clone this project.

        ./build.sh --run 

Compile and run the binary.

        ./build.sh --release

Compiles and copy the binary to `Dropbox (Antfood, LLC)/ANTFOOD WORLDWIDE/`.
