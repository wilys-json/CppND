# CPPND: Capstone Snake Game with Rival

This is the final project of the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## Build Using Docker

Alternatively, you can compile and run the program on a Docker Image.
*Make sure you have [Docker](https://www.docker.com/get-started) installed*
1. Clone this repo.
2. Navigate to the main directory of this repo.
3. Build the Docker image: `docker build -t capstone .` (This may take some time, and an image `capstone` will be created.)
4. Run the script: `bash RunDocker.sh` (on Windows installed with Git Bash: `RunDocker.sh`)
5. Open `localhost:6080` on your browser.
6. Open command line tool like LX Terminal.
7. Navigate to the project directory: `cd project/capstone`
8. Make a build directory: `mkdir build && cd build`
9. Compile: `cmake .. && make`
10. Run: `./SnakeGame`

## File and Class Structures
* Files
The game mainly draws on the following files:

  ...
  ├── data
  │   └── cache.txt
  
  └── src

      ├── Base64.h
      ├── GameObject.h
      ├── Movables.h
      ├── SwitchableColor.h
      ├── bullet.cpp
      ├── bullet.h
      ├── controller.cpp
      ├── controller.h
      ├── food.cpp
      ├── food.h
      ├── game.cpp
      ├── game.h
      ├── main.cpp
      ├── map.h
      ├── player.cpp
      ├── player.h
      ├── renderer.cpp
      ├── renderer.h
      ├── rival.cpp
      ├── rival.h
      ├── snake.cpp
      └── snake.h

* Classes

  * Abstract (Base) Classes / Interfaces
    * `GameObject` in [GameObject.h](src/GameObject.h) - Base class of all game objects (`Food`, `Bullet`, `Snake`): defines the basic behavior of position projection `projectToMap()` ([line 57](src/GameObject.h#L57)), type checking `isA<Type>()`([line 82](src/GameObject.h#L82)), index checking `OffGrid()` ([line 88](src/GameObject.h#L88)), and shared pointer acquisition `shareThis()`([line 102](src/GameObject.h#L102)), comprises virtual functions to update `Update()`([line 49](src/GameObject.h#L49)) and to initialize `Initialize()`([line 50](src/GameObject.h#L50)), and shares ownerships of `Map` object
    * `Movables` in [Movables.h](src/Movables.h) - Abstract Class for all moving game objects: comprises virtual function to check for collision `Collide()` ([line 16](src/Movables.h#L16))
    * `SwitchableColor` in [SwitchableColor.h](src/SwitchableColor.h) - Base Abstract Class for all color-changing / flashing game objects: defines the behavior of color getter `getColor()` ([line 17](src/SwitchableColor.h#L17)) and comprises virtual function `Blink()` ([line 19](src/SwitchableColor.h#L19))
    * `Snake` in [snake.h](src/snake.h) - Inherited from `GameObject`, `Movables`, base class of `PlayerSnake` and `RivalSnake`: defines base classes virtual function and further comprises common methods of snakes and virtual functions of food consumption threads `Digest()` ([line 35](src/snake.h#L35)) and sequelae of collision `Shrink()` ([line 36](src/snake.h#L36))

  * Main Classes
    * `Bullet` - Inherited from `GameObject`, `Movables`, `SwitchableColor`: a flashing, moving object that destroys the `RivalSnake` or make it shrink; projected when `space` is pressed while `PlayerSnake` is in [Shooter Mode](#Shooter-Mode) hence `Shoot()`([line 131](src/player.cpp#L131)) is called.
    * `Food` - Inherited from `GameObject` & `SwitchableColor` : a non-moving, potentially flashing object for scoring or changing the state and mode of `Snake` classes.
    * `PlayerSnake` - Inherited from `GameObject` and `Movables` : directly controlled by the user via `Controller` class.
    * `RivalSnake` - Inherited from `GameObject` and `Movables` : a moving antagonist of the game; shrinks or dies when collied, hence always escapes from the `PlayerSnake` unless it is in [Poison Mode](#Poison-Mode)
    * `Game` in [game.h](src/game.h) and [game.cpp](src/game.cpp) - defines essential game operations (`Initialize()`([line 49](src/game.cpp#L49)), `Run()`([line 69](src/game.cpp#L69)), `Terminate()`([line 242](src/game.cpp#L242))), file reading and writing (`readRecord()`([line 254](src/game.cpp#L254)) and `writeRecord)()`([line 277](src/game.cpp#L277))), and gaming logic (`Update()`([line 160](src/game.cpp#L160)), `CheckEvents()`([line 199](src/game.cpp#L199)))

  * Helper Classes
    * `Base64` in [Base64.h](src/Base64.h) - Helper Code from [tomykaira](https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594) to encode and decode cache in base64
    * `Controller` in [controller.h](src/controller.h) and [controller.cpp](sr/controller.cpp) - Helper class to handle input
    * `Map` in [map.h](src/map.h) - Helper class with underlying 2D vector for efficient retrieval of object positions
    * `Renderer` in [renderer.h](src/renderer.h) and [renderer.cpp](src/renderer.cpp) - Helper class to render objects in game playing window

## CC Attribution-ShareAlike 4.0 International

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
