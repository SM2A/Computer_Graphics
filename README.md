# Computer Graphics Course Project

This repository contains my Computer Graphics Course Project (Spring 2022) at University of Tehran.

![Bicycle](https://raw.githubusercontent.com/SM2A/Computer_Graphics/master/Resource/Banner.png?token=GHSAT0AAAAAABTS3IJUA57MG66CX35CNLP6YWNSAYQ)

#### Requirements

This project is build for windows and uses win32 api to work. I used freeglut library to make using **OpenGL** easier.

You can find out more about *freeglut* [here](http://freeglut.sourceforge.net/).

Then you can download *freeglut* library from [here](https://www.transmissionzero.co.uk/software/freeglut-devel/).
Because we are working on windows you should download MinGW package.

First you need [CMake](https://cmake.org/) installed on you system. Then install above mentioned library on your MinGW.

You can learn more about MinGW [here](https://www.mingw-w64.org/) and download [here](https://sourceforge.net/projects/mingw/).

#### How to Run

After meeting the needs of requirements we are ready to build and run project.

In project root directory create new folder called `build`. Open command prompt inside this folder and run command below:

```shell script
cmake -G "MinGW Makefiles" ..
```

Then return to project root directory and open command prompt here. Then run command below:

```shell script
cmake --build .\build\ --target Bicycle --config Debug
```

After command above our executable file is avaliable in `build` directory named `Bicycle.exe`.

All above steps are written in `build.bat`. You simply can open command prompt on project root directory and enter:

```shell script
build.bat
```

Or if using `PowerShell`:

```shell script
.\build.bat
```

Enjoy :)
