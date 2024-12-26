# mvSync
A very simple app built in C++ for educational purposes which takes in a video file and displays its first frame on an OpenGL surface.

This simple script has 3 external dependencies:

-  CMake to build the project,
-  pkg-config to locate shared libraries,
-  FFmpeg to read and write video files.

### Build steps
Clone this repository to your system: 
`$ git clone https://github.com/ShuvraneelMitra/mvSync.git`
1. Make a build directory inside the project folder and enter it:
   ```
   $ mkdir build
   $ cd build
   ```
2. Run CMake and build the project using the `--build .` option:
   ```
   $ cmake .. -Wno-dev
   $ cmake --build .
   ```
3. Run the application on the terminal: `$ ./DeriBot`
   Depending on the system the correct syntax could be `$ DeriBot`, `$ ./DeriBot` or `$ .\DeriBot`.
