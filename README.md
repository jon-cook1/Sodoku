# Grid Game - COSC 208 Project

The Grid Game is a C project developed for COSC 208: Introduction to Computer Systems (Fall 2023). This command-line based game challenges players to fill a grid with unique numbers, adhering to the rules similar to Sudoku. It's designed to be both a fun puzzle and an educational tool to understand grid-based logic games and basic C programming concepts.

## Features

- **Dynamic Grid**: A 9x9 playing grid where players add or remove numbers to solve the puzzle.
- **Command Line Interface**: Simple and intuitive commands for game actions such as adding or deleting numbers, and printing the current state of the grid.
- **Game Rules**: Ensures each row, column, and 3x3 sub-grid contains unique integers from 1 to 9.
- **Win Condition Check**: Evaluates the board to determine if the player has successfully completed the puzzle.
- **Help Command**: Provides a list of available commands and their format for easy reference.

## Usage

After compiling the game using a C compiler, run the executable from the command line. The game starts by presenting the grid and waiting for player commands:

- **Add a Number**: `add X Y N` (where X and Y are coordinates, and N is the number to add)
- **Delete a Number**: `del X Y` (where X and Y are coordinates of the number to remove)
- **Print the Board**: `print`
- **Help**: `help` (lists available commands)

## Commands and Controls

- Use **Control + Z** to exit the game at any point.
- The game commands are case insensitive but require strict formatting as shown in the help menu.

## Development and Custom Features

This project includes various functions for managing game state, validating input, and checking for win conditions. While some features were planned, such as auto candidate mode, file parsing, and a timer, they were left out due to time constraints but may be revisited in the future.

## Acknowledgments

Special thanks to my project partner [Kyle Leclair] for creating the functinoality for printing the board, and documenting the codebase.

