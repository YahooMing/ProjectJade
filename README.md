# Project Jade
#### IN ENGLISH
## About The Project 
Project Jade is a simple platformer game created as part of a university project. The game draws inspiration from old-school productions commonly found in arcade games. The objective of the game is to complete each level with the highest score possible. It consists of three levels, each with a different level of difficulty. The game also includes a tutorial.
## System Requirements
Operating system: Windows 10 / Windows 11
SFML Library: Version 2.5.1 or later
Additional Notes: A keyboard is required for optimal gameplay experience

WARNING
To play the game, you need a few files (all included in the game folder). However, to edit the content and compile, you must install the SFML library. The installation of the library will be described in the next section.
## Instalation
**If you only want to play the game**
   - Download PROJECT_JADE.zip
   - Extract PROJECT_JADE.zip
   - Open ProjectJade.ext
   - Play the game all you want!

**If you want to change the content and compile**
  -  Go to the official [SFML Website](https://www.sfml-dev.org/download.php)
  -  Download the appropriate version of SFML for your operating system and compiler. On Windows, choose "MinGW".
  -  After downloading, extract the SFML files to an appropriate directory on your computer. You can create a new folder, e.g., "SFML," and place all the files there.
  -  Add paths to the bin (containing DLL files), lib (containing library files), and include (containing header files) folders to the system's environment variables. Here's how to do it on Windows:
     - Right-click on the Computer (or This PC) icon on your desktop and select "Properties."
     - Click on "Advanced system settings" on the left side.
     - Click on "Environment Variables..." at the bottom.
     - In the "System variables" section, find the "Path" variable and click "Edit..."
     - Click "New" and add the paths to the SFML bin, lib, and include folders.
  - Check installation by opening the terminal and typing ```g++ -v```
  - From now on you can edit and compile my code

**GCC Instalation (It is required in order to edit and compile)**
   - Visit the [MinGW Installer page](https://osdn.net/projects/mingw/).
   -  Download the latest version of "mingw-get-setup.exe".
   -  Run the downloaded `mingw-get-setup.exe` file to launch the installer.
   -  In the installer window, select the "Basic Setup" option.
   -  From the "Installation" menu, choose "Update Catalogue" and wait for the update to complete.
   -  Back in the "Installation" menu, select "Apply Changes".
   -  In the "Pending Changes" window, check "mingw32-gcc-g++" and click "Apply Changes" again. The installer will download and install GCC.
   -  Add paths to the MinGW `bin` folders to the system's environment variables. Similar to the SFML installation, include paths such as `C:\MinGW\bin` and `C:\MinGW\msys\1.0\bin`.
   -  Open a terminal (cmd) and type the following command to check if GCC is correctly installed: ```g++ --version```
   
## Compilation

## Controls
- **Movement:**
  - Left: Key 'A'
  - Right: Key 'D'
  - Jump: Space

- **Akcje:**
  - Switch Screen Mode (only works in Main Menu): Key 'P'
  - Pause (only works in Gameplay): Key 'TAB'

#### PO POLSKU
## O Projekcie
Project Jade jest prostą grą platformową, stworzoną w ramach projektu na studia.  Gra jest wzorowana na starych produkcjach, które mogliśmy spotkać w automatach do gier. Celem gry jest ukończenie poziomu z jak największym wynikiem. Składa się z trzech poziomów z których każdy ma inny poziom trudności. Gra zawiera też samouczek.
## Wymagania systemowe
System operacyjny: Windows 10 / Windows 11
Biblioteka SFML: Wersja 2.5.1 lub nowsza
Dodatkowe uwagi: Do optymalnego doświadczenia z gry wymagana jest klawiatura.
## Sterowanie
- **Ruch:**
  - W lewo: Klawisz 'A'
  - W prawo: Klawisz 'D'
  - Skok: Spacja

- **Akcje:**
  - Zmiana trybu ekranu (działa wyłącznie w Menu Głównym): Klawisz 'P'
  - Pauza (działa wyłącznie w trakcie rozgrywki): Klawisz 'TAB'
