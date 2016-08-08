This project shows how to use OpenCV imagefilters to process cellular automata of the type that Conway's Game of Life belongs to. OpenCV allows this project to be a very terse and very fast, GPU accelerated implementation.

# Installation

Prerequisites:

```
git build-essential cmake opencv
```

For example, on debian based systems such as Ubuntu, use:

```
sudo apt-get install git build-essential cmake libopencv-dev
export OpenCV_DIR=/usr/share/OpenCV
git clone git@github.com:TamaHobbit/cellularautomaton.git
cd cellularautomaton
mkdir build-linux && cd build-linux
cmake ..
make
```

# Usage

Running the program without arguments explains its usage:

```
carmen@serenity:~/code/opencv/CellularAutomaton/build-linux$ ./CA
Defaulting to Life. To do a custom Cellular Automaton, run:
./CA [18-bit binary string]
The first 9 bits specify when a live bit survives, the second 9 bits specify when a dead bit is born. From left to right, the first of these nine means there are 8 live neighbours, while the last means there are none. For example, 000001100000001000 is Life.

Type a letter to choose how to seed the matrix:
c: Use a camera capture.
i: Seed from image.
r: Random seed.
r
Enter the display size. 1 is the lowest, meaning one pixel per cell.
1
Enter image width: 1024
Enter image height: 968
Enter the millentage of cells that start alive (0-1000): 333
1470678646
Cellular Automaton controls:
esc - exit
h - display this message
spacebar - pause/unpause
a - switch to different Cellular Automaton
f - run a specified number of ticks
w - run until stable
v - toggle verbose (displays ticks)
s - save image to disk
> - Speed up simulation (50 ms)
< - Slow down simulation (50 ms)
shift + > - Speed up simulation (10 ms)
shift + < - Slow down simulation (10 ms)
right arrow - step
Gtk-Message: Failed to load module "canberra-gtk-module"
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Delay: 200 milliseconds
>>>>>>Delay: 150 milliseconds
>>>Delay: 100 milliseconds
>>Delay: 50 milliseconds
>>>>>>Delay: 0 milliseconds
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>No key binding for: 113
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
```
