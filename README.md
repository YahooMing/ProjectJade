# Project Jade
#### IN ENGLISH :uk:
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
   - Open ProjectJade.exe
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
Type in the terminal the following commands in this particular order:
   1. ```g++ -c main.cpp -I"...\SFML-2.X.X\include"```
   2. ```g++ main.o -o project_jade -L"...\SFML-2.X.X\lib```
   3. ```./project_jade```

Remember: paths to SFML's include or lib folders may differ from those presented in the example above.
   
## Controls
- **Movement:**
  - Left: Key 'A'
  - Right: Key 'D'
  - Jump: Space

- **Actions:**
  - Switch Screen Mode (only works in Main Menu): Key 'P'
  - Pause (only works in Gameplay): Key 'TAB'
  - Lock the option in the menu: Numeric keypad
  - Choose the option in the menu: Key 'ENTER'

 ## Postscriptum
That would be all. If you've decided to download and try the game, I sincerely thank you. This is my first 'larger' programming project. I know it's not perfect, but I tried my best to make everything turn out as well as possible. The game is incredibly simple, and if I had more time, I would make it even better.
 
---

#### PO POLSKU :poland:
## O projekcie
Projekt Jade to prosta gra platformowa stworzona w ramach projektu uniwersyteckiego. Gra czerpie inspirację z produkcji znanych z automatów do gier. Celem gry jest ukończenie każdego poziomu z jak najwyższym wynikiem. Składa się z trzech poziomów o różnym stopniu trudności. Gra zawiera również samouczek.

## Wymagania systemowe
System operacyjny: Windows 10 / Windows 11

Biblioteka SFML: Wersja 2.5.1 lub nowsza

Dodatkowe uwagi: Do optymalnego doświadczenia z rozgrywką wymagana jest klawiatura.

OSTRZEŻENIE

Aby zagrać w grę, potrzebujesz kilku plików (wszystkie zawarte w folderze gry). Jednak do edycji zawartości i kompilacji musisz zainstalować bibliotekę SFML. Instalacja biblioteki zostanie opisana w następnej sekcji.

## Instalacja
**Jeśli chcesz tylko zagrać w grę**
   - Pobierz PROJECT_JADE.zip
   - Wypakuj PROJECT_JADE.zip
   - Otwórz ProjectJade.exe
   - Graj w grę ile chcesz!

**Jeśli chcesz edytować zawartość i kompilować**
  - Przejdź na oficjalną [stronę SFML](https://www.sfml-dev.org/download.php)
  - Pobierz odpowiednią wersję SFML dla swojego systemu operacyjnego i kompilatora. Dla systemu Windows wybierz "MinGW".
  - Po pobraniu wypakuj pliki SFML do odpowiedniego katalogu na swoim komputerze. Możesz utworzyć nowy folder, np. "SFML", i umieścić tam wszystkie pliki.
  - Dodaj ścieżki do folderów bin (zawierających pliki DLL), lib (zawierających pliki bibliotek) i include (zawierających pliki nagłówkowe) do zmiennych środowiskowych systemu. Oto jak to zrobić w systemie Windows:
     - Kliknij prawym przyciskiem myszy ikonę Komputer (lub Ten Komputer) na pulpicie i wybierz "Właściwości".
     - Kliknij na "Zaawansowane ustawienia systemu" po lewej stronie.
     - Kliknij na "Zmienne środowiskowe..." na dole.
     - W sekcji "Zmienne systemowe" znajdź zmienną "Path" i kliknij "Edytuj..."
     - Kliknij "Nowa" i dodaj ścieżki do folderów bin, lib i include SFML.
  - Sprawdź instalację, otwierając terminal i wpisując ```g++ -v```
  - Od teraz możesz edytować i kompilować mój kod

**Instalacja GCC (Wymagane do edycji i kompilacji)**
   - Odwiedź stronę [MinGW Installer](https://osdn.net/projects/mingw/).
   - Pobierz najnowszą wersję "mingw-get-setup.exe".
   - Uruchom pobrany plik `mingw-get-setup.exe`, aby uruchomić instalator.
   - W oknie instalatora wybierz opcję "Basic Setup".
   - Z menu "Installation" wybierz "Update Catalogue" i poczekaj na zakończenie aktualizacji.
   - Ponownie w menu "Installation" wybierz "Apply Changes".
   - W oknie "Pending Changes" zaznacz "mingw32-gcc-g++" i kliknij ponownie "Apply Changes". Instalator pobierze i zainstaluje GCC.
   - Dodaj ścieżki do folderów `bin` MinGW do zmiennych środowiskowych systemu. Podobnie jak w przypadku instalacji SFML, dodaj ścieżki takie jak `C:\MinGW\bin` i `C:\MinGW\msys\1.0\bin`.
   - Otwórz terminal (cmd) i wpisz poniższą komendę, aby sprawdzić, czy GCC jest poprawnie zainstalowany: ```g++ --version```

## Kompilacja
Wpisz w terminalu poniższe komendy w kolejności:
   1. ```g++ -c main.cpp -I"...\SFML-2.X.X\include"```
   2. ```g++ main.o -o project_jade -L"...\SFML-2.X.X\lib"```
   3. ```./project_jade```

Pamiętaj: ścieżki do folderów include lub lib SFML mogą różnić się od tych przedstawionych w powyższym przykładzie.

## Sterowanie
- **Ruch:**
  - Lewo: Klawisz 'A'
  - Prawo: Klawisz 'D'
  - Skok: Spacja

- **Akcje:**
  - Przełącz tryb ekranu (działa tylko w menu głównym): Klawisz 'P'
  - Pauza (działa tylko w trakcie rozgrywki): Klawisz 'TAB'
  - Zablokuj opcję w menu: Klawiatura numeryczna
  - Wybierz opcję w menu: Klawisz 'ENTER'

## Postscriptum
To by było wszystko. Jeśli zdecydowałeś się pobrać i spróbować gry to bardzo dziękuję. To jest mój pierwszy 'większy' projekt programistyczny. Wiem, że nie jest idealny ale starałem się, aby wszystko wyszło jak najlepiej. Gra jest niezwykle prosta i jakbym miał więcej czasu to bym zrobił ją jeszcze lepszą.
