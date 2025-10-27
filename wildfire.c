// file: wildfire.c
// description: spreading fire simulation
// @author Mia Venesky
// version contorl: github CSCI_Project1 repo

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "display.h"

// default values if not given by user
#define DEF_GRIDSIZE 10
#define DEF_NEIGH 25
#define DEF_FIRECHANCE 30
#define DEF_STARTBURN 10
#define DEF_DENSITY 50
#define DEF_PRINT -1

// global variables to track fire and statistics
static int fireTrue = 1;
static int cycle = 0;
static int cum_changes = 0;
static int cur_changes = 0;

//global probabilities and size
static double pCatch = 0;
static double dens = 0;
static double pBurning = 0;
static double pNeighbor = 0;
static int siz = 0;

// struct for simulation settings
typedef struct {
	int gridSize;
	int neighbors;
	int fireChance;
	int startBurn;
	int density;
	int print;
} Settings;

/// prints general information about program and its options
static void generalPrintInfo(){
	fprintf(stderr, "usage: wildfire [options]\n");
	fprintf(stderr, "By default, the simulation runs in overlay display mode.\n");
	fprintf(stderr, "The -pN option makes the simulation run in print mode for up to N states.\n");
	fprintf(stderr, "\nSimulation Configuration Options:\n");
	fprintf(stderr, " -H  # View simulation options and quit.\n");
	fprintf(stderr, " -bN # proportion of trees that are already burning. 0 < N < 101.\n");
        fprintf(stderr, " -cN # probability that a tree will catch fire. 0 < N < 101.\n");
        fprintf(stderr, " -dN # density: the proportion of trees in the grid. 0 < N < 101.\n");
        fprintf(stderr, " -nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.\n");
        fprintf(stderr, " -pN # number of states to print before quitting. -1 < N < ...\n");
        fprintf(stderr, " -sN # simulation grid size. 4 < N < 41.\n\n");
}

/// prints the grid and translates its abcde variables to their proper repersentation
/// @param gridSize: dimensions of the grid
/// @param griddy: 2D grid of characters representing cell states
static void printGrid(int gridSize, char griddy[gridSize][gridSize]) {
	char temp;
	for(int i = 0; i < gridSize; i++){
		for(int j = 0; j < gridSize; j++){
			temp = griddy[i][j];
			if(temp == 'a'){
				temp = 'Y';
			}
			else if(temp == ' '){
				temp = ' ';
			}
			else if(temp == 'e'){
				temp = '.';
			}
			else{
				temp = '*';
			}
			printf("%c", temp);
		}
		printf("\n");
	}
}

/// decides what happens to one cell in next round 
/// @param size: grid dimensions
/// @param grid: current state of the grid
/// @param row, col: current cell position
/// @param neighbors: % of burning neighbors required to ignite
/// @param fireChance: probability that an eligible tree ignites
/// @return new cell state character ('a','b','c','d','e',' ')
static char spread(int size, char grid[size][size], int row, int col, double neighbors, double fireChance){
	int neighs = 0;
	int burning = 0;
	if(grid[row][col] == ' '){
		return ' ';
	}
	else if(grid[row][col] == 'e'){
		return 'e';
	}
	else if(grid[row][col] == 'b'){
		fireTrue = 1;
		return 'c';
	}
        else if(grid[row][col] == 'c'){
		fireTrue = 1;
                return 'd';
        }
        else if(grid[row][col] == 'd'){
		cur_changes += 1;
		cum_changes += 1;
                return 'e';
        }
	// For trees ('a'), count burning neighbors
        else if(grid[row][col] == 'a'){
		// Check 8 directions (N, S, E, W, and diagonals)
                if(row != 0){
			if(grid[row-1][col] == 'a'){
				neighs += 1;
			}
			else if(grid[row-1][col] == 'b' || grid[row-1][col] == 'c' || grid[row-1][col] == 'd'){
				burning += 1;
                                neighs += 1;

			}
		}
		if(row != size-1){
                        if(grid[row+1][col] == 'a'){
                                neighs += 1;
                        }
                        else if(grid[row+1][col] == 'b' || grid[row+1][col] == 'c' || grid[row+1][col] == 'd'){
                                burning += 1;
                                neighs += 1;

                        }
		}
                if(col != 0){
                        if(grid[row][col-1] == 'a'){
                                neighs += 1;
                        }
                        else if(grid[row][col-1] == 'b' || grid[row][col-1] == 'c' || grid[row][col-1] == 'd'){
                                burning += 1;
                                neighs += 1;
                        }
                }
                if(col != size-1){
                        if(grid[row][col+1] == 'a'){
                                neighs +=1;
                        }
                        else if(grid[row][col+1] == 'b' || grid[row][col+1] == 'c' || grid[row][col+1] == 'd'){
                                burning +=1;
                                neighs +=1;
                        }
                }
		
		if(row != 0 && col != 0){
                        if(grid[row-1][col-1] == 'a'){
                                neighs += 1;
                        }
                        else if(grid[row-1][col-1] == 'b' || grid[row-1][col-1] == 'c' || grid[row-1][col-1] == 'd'){
                                burning += 1;
                                neighs += 1;
                        }
		}
                if(row != size-1 && col != size-1){
                        if(grid[row+1][col+1] == 'a'){
                                neighs += 1;
                        }
                        else if(grid[row+1][col+1] == 'b' || grid[row+1][col+1] == 'c' || grid[row+1][col+1] == 'd'){
                                burning += 1;
                                neighs += 1;
                        }
                }
                if(row != 0 && col != size-1){
                        if(grid[row-1][col+1] == 'a'){
                                neighs += 1;
                        }
                        else if(grid[row-1][col+1] == 'b' || grid[row-1][col+1] == 'c' || grid[row-1][col+1] == 'd'){
                                burning += 1;
                                neighs += 1;
                        }
                }
                if(row != size-1 && col != 0){
                        if(grid[row+1][col-1] == 'a'){
                                neighs += 1;
                        }
                        else if(grid[row+1][col-1] == 'b' || grid[row+1][col-1] == 'c' || grid[row+1][col-1] == 'd'){
                                burning += 1;
				neighs += 1;
                        }
                }
		double burningAmt = burning/(neighs/1.0);
		if(burningAmt > neighbors){
			if(((double)rand() / RAND_MAX) < fireChance){
				fireTrue = 1;
				cum_changes += 1;
                                cur_changes += 1;
				return 'b';
			}
		}
        }
	return 'a';
}

/// Updates the entire grid by one time step
/// @param size: grid dimension
/// @param grid: current grid, modified in place
/// @param neighbors: neighbor influence threshold
/// @param fireChance: probability of catching fire
static void update(int size, char grid[size][size], double neighbors, double fireChance){
	cur_changes = 0;
	fireTrue = 0;
	char gridCopy[size][size];
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			gridCopy[i][j] = grid[i][j];
		}
	}
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			grid[i][j] = spread(size, gridCopy, i, j, neighbors, fireChance);
		}
	}
}

/// Prints current simulation statistics (size, probabilities, etc.)
static void printOutput(){
	printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f\n", siz, pCatch, dens, pBurning, pNeighbor);
	fflush( stdout );
	printf("cycle %d, current changes %d, cumulative changes %d.  \n", cycle, cur_changes, cum_changes);
}

/// Runs the simulation in print mode (fixed # of steps, printed each time).
/// @param size: grid dimension
/// @param grid: current grid, modified in place
/// @param neighbors: neighbor influence threshold
/// @param fireChance: probability of catching fire
static void printMode(int size, char grid[size][size], int print, double neighbors, double fireChance){
        printf("===========================\n======== Wildfire =========\n===========================\n=== Print %d Time Steps ===\n===========================\n", print);
	printGrid(size, grid);
	printOutput();
	for(int i = 0; i < print; i++){
		if(fireTrue == 0){
			printf("Fires are out.\n");
			break;
		}
		update(size, grid, neighbors, fireChance);
		cycle += 1;
		printGrid(size, grid);
		printOutput();
	}
        
}

/// Runs simulation in overlay mode (live-updating terminal view).
/// @param size: grid dimension
/// @param grid: current grid, modified in place
/// @param neighbors: neighbor influence threshold
/// @param fireChance: probability of catching fire
static void overlayMode(int size, char grid[size][size], double neighbors, double fireChance){
        clear();
        set_cur_pos(1, 0);
        while(fireTrue != 0){
                update(size, grid, neighbors, fireChance);
                cycle += 1;
                printGrid(size, grid);
		fflush( stdout );
                printOutput();
		fflush( stdout );
                usleep(750000);
		if(fireTrue == 0){
			break;
		}
                set_cur_pos(1, 0);
        }
	fflush( stdout );
}

/// Entry point: parses command-line options, sets up grid, and starts simulation.
/// @param argc: argument count
/// @param argv: argument vector
/// @return EXIT_SUCCESS or EXIT_FAILURE
int main(int argc, char *argv[]){
	char flags[] = "s:n:c:b:d:p:H";
	int opt = 0;
	Settings gSettings;
	gSettings.gridSize = DEF_GRIDSIZE;
	gSettings.neighbors = DEF_NEIGH;
	gSettings.fireChance = DEF_FIRECHANCE;
	gSettings.startBurn = DEF_STARTBURN;
	gSettings.density = DEF_DENSITY;
	gSettings.print = DEF_PRINT;
	pCatch = DEF_FIRECHANCE / 100.0;
	dens = DEF_DENSITY / 100.0;
	pBurning = DEF_STARTBURN / 100.0;
	pNeighbor = DEF_NEIGH / 100.0;
	siz = DEF_GRIDSIZE;
 	// Parse command-line flags
	while((opt = getopt(argc, argv, flags)) != -1){
		switch(opt) {
		case 's':
			if(atoi(optarg) > 40 || atoi(optarg) < 5){
				fprintf(stderr, "(-sN) simulation grid size must be an integer in [5...40].\n");
				generalPrintInfo();
				exit(EXIT_FAILURE);
			}
			gSettings.gridSize = atoi(optarg);
			siz = atoi(optarg);
			break;
		case 'n':
			if(atoi(optarg) > 100 || atoi(optarg) < 0){
                                fprintf(stderr, "(-nN) %%neighbors influence catching fire must be an integer in [0...100].\n");
                                generalPrintInfo();
                                exit(EXIT_FAILURE);
                        }
                        gSettings.neighbors = atoi(optarg);
			pNeighbor = atoi(optarg) / 100.0;
                        break;
		case 'c':
                        if(atoi(optarg) > 100 || atoi(optarg) < 1){
                                fprintf(stderr, "(-cN) probability a tree will catch fire must be an integer in [1...100].\n");
                                generalPrintInfo();
                                exit(EXIT_FAILURE);
                        }
                        gSettings.fireChance = atoi(optarg);
			pCatch = atoi(optarg) / 100.0;
                        break;
		case 'b':
			if(atoi(optarg) > 100 || atoi(optarg) < 1){
                                fprintf(stderr, "(-bN) proportion already burning must be an integer in [1...100].\n");
                                generalPrintInfo();
                                exit(EXIT_FAILURE);
                        }
                        gSettings.startBurn = atoi(optarg);
			pBurning = atoi(optarg) / 100.0;
                        break;
		case 'd':
                        if(atoi(optarg) > 100 || atoi(optarg) < 1){
                                fprintf(stderr, "(-dN) density of trees in the grid must be an integer in [1...100].\n");
                                generalPrintInfo();
                                exit(EXIT_FAILURE);
                        }
                        gSettings.density = atoi(optarg);
			dens = atoi(optarg) / 100.0;
                        break;
		case 'p':
                        if(atoi(optarg) > 10000 || atoi(optarg) < 0){
                                fprintf(stderr, "(-pN) number of states to print must be an integer in [0...10000].\n");
                                generalPrintInfo();
                                exit(EXIT_FAILURE);
                        }
			gSettings.print = atoi(optarg);
			break;
                case 'H':
                        generalPrintInfo();
			return EXIT_SUCCESS;
                        break;

		case '?':
			fprintf(stderr, "Error! Invalid Flag");
			generalPrintInfo();
			exit(EXIT_FAILURE);
		}

	}
	 // Initialize random seed and grid
	srandom(41);
 	// Fill grid with spaces initially
	char grid[gSettings.gridSize][gSettings.gridSize];
	for (int i = 0; i < gSettings.gridSize; i++) {
		for (int j = 0; j < gSettings.gridSize; j++) {
        		grid[i][j] = ' ';
    		}
	}
	 // Determine number of trees and burning trees
	int gridAmounts = gSettings.gridSize * gSettings.gridSize;
	int treeAmount = (gSettings.density * gridAmounts) / 100;
	int burningAmount = (gSettings.startBurn * treeAmount) / 100;
	int numR = rand() % gSettings.gridSize;
	int numC = rand() % gSettings.gridSize;
	for(int i = 0; i < treeAmount; i++){
		while(grid[numR][numC] == 'a') {
			numR = rand() % gSettings.gridSize;
			numC = rand() % gSettings.gridSize;
			}
		grid[numR][numC] = 'a';
	}
	for(int i = 0; i < burningAmount; i++){
                while(grid[numR][numC] == ' ' || grid[numR][numC] == 'b') {
                        numR = rand() % gSettings.gridSize;
                        numC = rand() % gSettings.gridSize;
                        }
                grid[numR][numC] = 'b';
        }
	double neighPer = gSettings.neighbors / 100.0;
	double firePer = gSettings.fireChance / 100.0;
	 // Run in print or overlay mode
	if(gSettings.print > -1){
	        printMode(gSettings.gridSize, grid, gSettings.print,neighPer, firePer);
		return EXIT_SUCCESS;
	}
	overlayMode(gSettings.gridSize, grid, neighPer, firePer);
        printf("Fires are out.\n");
	return EXIT_SUCCESS;
}
