# Lem-in

The objective of this project is to navigate 'ants' through 'farm' with as few moves as possible (i.e. maximum flow problem)

### The project

A 'farm' consists of 'rooms' where only one ant can be at a time, and undirected unweighted 'tubes' between them

Two verteces marked as 'start' where all ants are, and 'end' where all ants should be

The program was tested to solve graphs with up to ten thousand verteces and dozens of possible superpositioned paths

As a bonus part of the project there is a visualziation

![visualization](../img/visualization.gif?raw=true)

### Installing

The project was written and tested on macOS Sierra with SDL2 library for visualization, which is included into this repository

```
git clone --recursive https://github.com/hauh/lem-in.git lem-in && cd lem-in && make
```

### Usage

```
lem-in [-flags] < map_file
	   (lem-in will read input from stdin, use '<' to redirect from file)
flags:
    -h - usage
    -v - visual mode
    -d - debug mode (show paths found)
    -p - precise mode (a bit better results but slower)
    -n - don't show map and ants movements
```

### Input

There are example maps and generator in 'maps' directory

The structure of valid map should be as below

![input](../img/input.png?raw=true)

The program will validate input

### Algorithm

* Find shortest path using BFS, then set its edges to be directed only backward
* Find next path, prioritizing backward directions, if backward edge was used, remove it
* Repeat until all possible paths are found
* Remove all edges from graph
* Add first few found paths to graph, then remove edges that were used more than once (Suurballe's algorithm)
* Find solution paths with BFS, then check the flow
* Repeat Suurballe with one more path at a time while solution is getting better

### Grade

125/125
