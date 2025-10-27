# WildfireSimulation
A C-based cellular automaton that models how fire spreads through a forest based on probability, density, and neighbor influence. Uses command-line arguments for customizable parameters and real-time ASCII visualization to simulate and analyze forest fire behavior!

# Features:

Simulates fire spread across a two-dimensional forest grid, where each cell represents a tree, burning tree, burned tree, or empty space.

Implements customizable simulation settings through command-line flags for grid size, forest density, probability of catching fire, neighbor influence, and initial burning proportion.

Supports two display modes:

Print Mode — outputs each simulation state sequentially to the console.

Overlay Mode — continuously updates the grid display in place for a live simulation effect.


Displays real-time statistics such as cycle count, number of state changes, and cumulative changes as the fire progresses.

Uses random number generation to produce natural and unpredictable fire spread patterns.

Designed with modular structure using separate files (display.h and display.c) provided by RIT for rendering the grid in the terminal.

# How to Run:

Compile the program and then run with any combination of configuration flags (all optional):

Use

./wildfire -H

To see options


Example:

./wildfire -s20 -d70 -c50 -b10

# Command-Line Options:

-H  # View simulation help and quit.

-bN # Proportion of trees already burning (1–100).

-cN # Probability that a tree will catch fire (1–100).

-dN # Density of trees in the grid (1–100).

-nN # Neighbor effect threshold (0–100).

-pN # Run in print mode for N cycles.

-sN # Grid size (5–40).


# Example Output:

Each cell in the grid is displayed using ASCII characters:

' ' → Empty cell  

'Y' → Living tree  

'*' → Burning tree  

'.' → Burned tree  



The simulation updates each step to show how fire spreads, burns out, and ultimately extinguishes.

# Project Overview:

This project was developed for CSCI-243: Mechanics of Programming at RIT.
It applies principles of systems programming, random simulation, and algorithmic modeling to visualize real-world phenomena such as wildfire spread.

Concepts Used:
Cellular automata
Random number generation
Command-line parsing with getopt()
Cursor-control and terminal rendering
Data structure design with structs and arrays

# Thanks for reading!!! :)
