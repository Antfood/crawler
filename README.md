# Crawler

Crawler is a user-friendly terminal application that will help you find library recordings with the wrong format 
filename and provide a quick way to fix it. No commands are required — the program will prompt you
with a text-based user interface and walk you through the whole process.

![crawler](https://github.com/Antfood/crawler/blob/main/img/crawler.png?raw=true)

Crawler runs on OSX only.

## Dropbox

The program specifically targets Antfood's dropbox. So in order to run it you must have dropbox synced to your computer.

Crawler will first look at your home directory (`/Users/${USER}`) for the Dropbox folder. Failing at that, it will then scan the drives in `/Volumes`. It will go as far as
the root of external hardrives so if the Dropbox folder is deeply nested in that filesystem crawler will fail the search.

If a`Dropbox (Antfood, LLC)` directory is not found, crawler will quit immediately while
warning the user.

## Format

Crawler will validate the following fields

    Client_Project_RecordingName_Descriptor_Key_Date_BPM_T-S_ComposerIntitials_Talent(Instrument).wav

- `Client`, `Project`, `RecordingName`, `Descriptor`: Letters and numbers only. Word separated in CamelCase.

        JohnsonJohnson
        AutomatedSearch
        Web20
        Marimbondo  
        Original

- `Date`: Numbers only in the following format: `YYMMDD`.
- `BPM`: Numbers with 1 to 3 digits only. `5`, `12`, `120`.
- `T-S`: Time signature. Two sets of 1 to 2 digits separated by a `-`.  `4-4`, `12-8`.
- `ComposerInitials`: Between 1 and 3 capitalized letters only. Multiple composer initials are separated by a `-`.

        PB
        WB-CVV
        YE-DH-LS
- `Talent(Instrument)`: Only Letters and `()` allowed. Composers or arrangers name in CamelCase followed by the instrument within parenthesis. 
This field must be marked with `NXT` if empty. Multiple talents or arrangers also possible.

        WilsonBrown(Guitar)
        ViniNunes(Arranger)
        WillBone(Trombone)AmandaPlantin(Vocals)
        WillBone(Trombone)AmandaPlantin(Vocals)ViniNunes(Arranger)
        NXT


### Examples:

    EITSwiss_BadKitty_Cattitude_Original_220222_E_98_4-4_JH_NXT
    Intersport_SS22_RisingUp_Orignal_211120_Dm_101_4-4_PB_CharleyVanVeldhoven(Piano).wav
    Adidas_StayInPlay_3AM_Remix_211104_Cm_125_4-4_DH_PedroBotsaris(Arranger)EsperanzaDenswil(Vocals)
    WeTransfer_WTO2O_Wavvy_RevisedVersion_220114_110_D_4-4_DH-RW_NXT.wav

## Usage
You can launch the app by double-clicking on its icon on a finder window. This will prompt a terminal
which will serve as the user interface for crawler.

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
