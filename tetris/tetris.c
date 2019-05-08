#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init();
void start();
void printScreen();

#define not	 !=
#define over 1

#define board_width	 10
#define board_height 20

#define empty	0
#define moving	1
#define static	2

int game, speed, **board;

int main(){
	init();
	start();

	return 0;
}

//	initializes game values
void init(){
	int i, j;

	game = 0;
	speed = 1;

	//	allocates memory for the game board
	board = (int **) malloc(board_height * sizeof(int *));
	for(i = 0; i < board_height; i++){
		board[i] = (int *) malloc(board_width * sizeof(int));
		for(j = 0; j < board_width; j++){
			//	initializes the board as empty
			board[i][j] = empty;
		}
	}
}

//	starts the game
void start(){
	while(game not over){
		printScreen();

		// waits for 1 second
		sleep(1);
	}
}

//	prints the screen
void printScreen(){
	int x, y;

	system("clear");

	for(y = 0; y < board_height; y++){
		for(x = 0; x < board_width; x++){
			printf("%d ", board[y][x]);
		}
		putchar('\n');
	}
}