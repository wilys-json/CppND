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
### Files
The game mainly draws on the following files:
```
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
```
### Classes

  * Abstract (Base) Classes / Interfaces
    * `GameObject` in [GameObject.h](src/GameObject.h) - Base class of all game objects (`Food`, `Bullet`, `Snake`): defines the basic behavior of position projection `projectToMap()` ([line 57](src/GameObject.h#L57)), type checking `isA<Type>()`([line 82](src/GameObject.h#L82)), index checking `OffGrid()` ([line 88](src/GameObject.h#L88)), and shared pointer acquisition `shareThis()`([line 102](src/GameObject.h#L102)), comprises virtual functions to update `Update()`([line 49](src/GameObject.h#L49)) and to initialize `Initialize()`([line 50](src/GameObject.h#L50)), and shares ownerships of `Map` object
    * `Movables` in [Movables.h](src/Movables.h) - Abstract Class for all moving game objects: comprises virtual function to check for collision `Collide()` ([line 16](src/Movables.h#L16))
    * `SwitchableColor` in [SwitchableColor.h](src/SwitchableColor.h) - Base Abstract Class for all color-changing / flashing game objects: defines the behavior of color getter `getColor()` ([line 17](src/SwitchableColor.h#L17)) and comprises virtual function `Blink()` ([line 19](src/SwitchableColor.h#L19))
    * `Snake` in [snake.h](src/snake.h) - Inherited from `GameObject`, `Movables`, base class of `PlayerSnake` and `RivalSnake`: defines base classes virtual function and further comprises common methods of snakes and virtual functions of food consumption threads `Digest()` ([line 35](src/snake.h#L35)) and sequelae of collision `Shrink()` ([line 36](src/snake.h#L36))

  * Main Classes
    * `Bullet` - Inherited from `GameObject`, `Movables`, `SwitchableColor`: a flashing, moving object that destroys the `RivalSnake` or make it shrink; projected when `space` is pressed while `PlayerSnake` is in Shooter Mode hence `Shoot()`([line 131](src/player.cpp#L131)) is called.
    * `Food` - Inherited from `GameObject` & `SwitchableColor` : a non-moving, potentially flashing object for scoring or changing the state and mode of `Snake` classes.
    * `PlayerSnake` - Inherited from `GameObject` and `Movables` : directly controlled by the user via `Controller` class.
    * `RivalSnake` - Inherited from `GameObject` and `Movables` : a moving antagonist of the game; shrinks or dies when collied, hence always escapes from the `PlayerSnake` unless it is in Poison Mode
    * `Game` in [game.h](src/game.h) and [game.cpp](src/game.cpp) - defines essential game operations (`Initialize()`([line 49](src/game.cpp#L49)), `Run()`([line 69](src/game.cpp#L69)), `Terminate()`([line 242](src/game.cpp#L242))), file reading and writing (`readRecord()`([line 254](src/game.cpp#L254)) and `writeRecord)()`([line 277](src/game.cpp#L277))), and gaming logic (`Update()`([line 160](src/game.cpp#L160)), `CheckEvents()`([line 199](src/game.cpp#L199)))

  * Helper Classes
    * `Base64` in [Base64.h](src/Base64.h) - Helper Code from [tomykaira](https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594) to encode and decode cache in base64
    * `Controller` in [controller.h](src/controller.h) and [controller.cpp](sr/controller.cpp) - Helper class to handle input
    * `Map` in [map.h](src/map.h) - Helper class with underlying 2D vector for efficient retrieval of object positions
    * `Renderer` in [renderer.h](src/renderer.h) and [renderer.cpp](src/renderer.cpp) - Helper class to render objects in game playing window

## Rubric

  __README (All Rubric Points REQUIRED)__

  |DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
  |-- | -- | --| -- |
  | :heavy_check_mark: | A README with instructions is included with the project |The README is included with the project and has instructions for building/running the project. If any additional libraries are needed to run the project, these are indicated with cross-platform installation instructions. You can submit your writeup as markdown or pdf.| |
  | :heavy_check_mark: | The README indicates which project is chosen. | The README describes the project you have built. The README also indicates the file and class structure, along with the expected behavior or output of the program. | |
  | :heavy_check_mark: | The README includes information about each rubric point addressed. | The README indicates which rubric points are addressed. The README also indicates where in the code (i.e. files and line numbers) that the rubric points are addressed. | |

  __Compiling and Testing (All Rubric Points REQUIRED)__

  |DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
  |-- | -- | --| -- |
  | :heavy_check_mark: | The submission must compile and run. | The project code must compile and run without errors. We strongly recommend using `cmake` and `make`, as provided in the starter repos. If you choose another build system, the code must compile on any reviewer platform. |

  __Loops, Functions, I/O__

  |DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
  |-- | -- | --| -- |
  | :heavy_check_mark: | The project demonstrates an understanding of C++ functions and control structures.| A variety of control structures are used in the project. The project code is clearly organized into functions.| Each *.cpp file |
  | :heavy_check_mark: | The project reads data from a file and process the data, or the program writes data to a file. | The project reads data from an external file or writes data to a file as part of the necessary operation of the program.| `readRecord()` in [game.cpp](src/game.cpp#L254) and `writeRecord()` in [game.cpp](src/game.cpp#L277) |
  | :heavy_check_mark: | The project accepts user input and processes the input.|The project accepts input from a user as part of the necessary operation of the program.| [controller.cpp](src/controller.cpp) handles keyboard input from user |

  __Object Oriented Programming__

  |DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
  |-- | -- | --| -- |
  | :heavy_check_mark: | The project uses Object Oriented Programming techniques. | The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks. | As explained in [Classes](#Classes) |
  | :heavy_check_mark: | Classes use appropriate access specifiers for class members. | All class data members are explicitly specified as public, protected, or private.| Each *.cpp and *.h files |
  | :heavy_check_mark: | Class constructors utilize member initialization lists. | All class members that are set to argument values are initialized through member initialization lists.| Same as above |
  | :heavy_check_mark: | Classes abstract implementation details from their interfaces. | All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.| Same as above |
  | :heavy_check_mark: | Classes encapsulate behavior. | Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.| Same as above |
  | :heavy_check_mark: | Classes follow an appropriate inheritance hierarchy. | Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.| As explained in [Classes](#Classes) |
  | :heavy_check_mark: | Overloaded functions allow the same function to operate on different parameters. | e.g. `OffGrid()` in [line 88](src/GameObject.h#L88) and [line 97](src/GameObject.h#L97) of GameObject.h|
  | :heavy_check_mark: | Derived class functions override virtual base class functions. |One member function in an inherited class overrides a virtual base class member function.| e.g. `Initialize()` in [snake.cpp](src/snake.cpp#L4) overrides [GameObject.h](src/GameObject.h#L50), `Digest()` in [player.cpp](src/player.cpp#L84) overrides [snake.h](src/snake.h#L35)  |
  | :heavy_check_mark: | Templates generalize functions in the project. | One function is declared with a template that allows it to accept a generic parameter.| `Place<Type>()` in [game.cpp](src/game.cpp#L113) is a template function for placing objects, `isA<Type>()` in [GameObject.h](src/GameObject.h#82) for type checking |

  __Memory Management__

  |DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
  |-- | -- | --| -- |
  | :heavy_check_mark: | The project makes use of references in function declarations. | At least two variables are defined as references, or two functions use pass-by-reference in the project code.|`Initialize()` in GameObject family accepts reference to `int` as arguments|
  | :heavy_check_mark: | The project uses destructors appropriately. | At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor. | All children of the GameObject family uses destructors. |
  | :heavy_check_mark:  | The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. | The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.| |
  | | The project follows the Rule of 5. | For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.|  |
  | :heavy_check_mark: | The project uses move semantics to move data, instead of copying it, where possible. | For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object. | e.g. moving raw pointer into `Collide()` to avoid atomic increment ([line 179 in game.cpp](src/game.cpp#L179)) and moving promise into the thread of `Consume()` ([line 180-181 in game.cpp](src/game.cpp#L181))  |
  | :heavy_check_mark: | The project uses smart pointers instead of raw pointers. | The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.| Extensively used in game objects |

  __Concurrency__

  |DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
  |-- | -- | --| -- |
  | :heavy_check_mark: | The project uses multithreading. | The project uses multiple threads in the execution.| Calling `RandomWalk()` from `Initialize()` in [RivalSnake](src/rival.cpp#L22), `Consume()` from `Update()` in [Game](src/game.cpp#L180), `Digest()` from `Consume` in [Snakes](src/snake.cpp#100) |
  | :heavy_check_mark: | A promise and future is used in the project. | A promise and future is used to pass data from a worker thread to a parent thread in the project code.| a promise is passed into `Consume()` in [lines 180-181 of game.cpp](src/game.cpp#L181) for setting value in [line 102 in snake.cpp](src/snake.cpp#L102) and the value is retrieved from the future in [line 190 in game.cpp](src/game.cpp#L190) |
  | :heavy_check_mark: | A mutex or lock is used in the project. | A mutex or lock (e.g. std::lock_guard or std::unique_lock) is used to protect data that is shared across multiple threads in the project code.| Extensively used in game objects. |
  | :heavy_check_mark: | A condition variable is used in the project. | A std::condition_variable is used in the project code to synchronize thread execution.| `walkConditionVariable` in `RivalSnake` to synchronize `RandomWalk()` |

## CC Attribution-ShareAlike 4.0 International

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
