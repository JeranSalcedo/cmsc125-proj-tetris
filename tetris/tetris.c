#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define not	 !=
#define over 1
#define bool int
#define true 0
#define false 1

#define queue_size	 4
#define board_width	 10
#define board_height 20

#define empty		0
#define occupied	1


void init();
void start();
void printScreen();
int dequeue();
bool stopPiece();
void changePiece();

/**************************************
 *	PIECES 							  *
 *		1:	** 			5:	*		  *
 *			**				*0*		  *
 *									  *
 *		2:	** 			6:	  *		  *
 *			 0*				*0*		  *
 *									  *
 *		3:	 ** 		7:	 *		  *
 *			*0				*0*		  *
 *									  *
 *		4:	****					  *
 *									  *
 **************************************/


int game, speed, playerPiece, pieceOrientation, playerX, playerY, queueHead, queueTail, *pieceQueue, **board;

int main(){
	init();
	start();

	return 0;
}

//	initializes game values
void init(){
	int i, j;

	//	for allowing generation of random numbers
	srand(time(NULL));

	game = 0;
	speed = 1;
	queueHead = 0;
	queueTail = queue_size - 1;
	//	initializes player piece to null
	playerPiece = 0;
	pieceOrientation = 0;
	playerX = 4;
	playerY = 0;

	//	allocates memory for the queue
	pieceQueue = (int *) malloc(queue_size * sizeof(int));
	//	initializes the queue with random pieces
	for(i = 0; i < queue_size; i++){
		//	fills the array with an integer from 1 to 7
		pieceQueue[i] = rand() % 7 + 1;
	}

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
		//	if player has no current piece
		if(playerPiece == 0){
			//	gets a new piece from the queue
			playerPiece = dequeue();
			//	sets initial position to middle top part of play area
			playerX = 4;
			playerY = 0;
		}

		//	checks if player piece should be stopped
		if(stopPiece() == false){
			//	if not, keep making piece fall
			playerY++;
		} else {
			changePiece();
		}

		printScreen();

		// waits for 1 second
		usleep(500000);
	}
}

//	prints the screen
void printScreen(){
	int x, y;

	system("clear");

	for(x = 0; x < board_width + 2; x++){
		printf("0 ");
	}
	putchar('\n');
	for(y = 0; y < board_height; y++){
		printf("0 ");
		for(x = 0; x < board_width; x++){

			//	if any of the conditions become true then board cell (x,y) is occupied by a piece
			printf("%c ",
				board[y][x] == occupied ||
				y == playerY && 
				(
					x == playerX ||
					x == playerX - 1 &&
					(
						playerPiece == 3 ||
						playerPiece == 4 ||
						playerPiece == 5 ||
						playerPiece == 6 ||
						playerPiece == 7
					)
					||
					x == playerX + 1 &&
					(
						playerPiece == 1 ||
						playerPiece == 2 ||
						playerPiece == 4 ||
						playerPiece == 5 ||
						playerPiece == 6 ||
						playerPiece == 7
					)
					||
					x == playerX + 2 && playerPiece == 4
				)
				||
				y == playerY - 1 &&
				(
					x == playerX &&
					(
						playerPiece == 1 ||
						playerPiece == 2 ||
						playerPiece == 3 ||
						playerPiece == 7
					)
					||
					x == playerX - 1 &&
					(
						playerPiece == 2 ||
						playerPiece == 5
					)
					||
					x == playerX + 1 &&
					(
						playerPiece == 1 ||
						playerPiece == 3 ||
						playerPiece == 6
					)
				)
				?
				'*' :
				' '
			);
		}
		if(y == 0){
			printf("0\tNEXT: %d %d %d %d\n", pieceQueue[queueHead], pieceQueue[queueHead == queue_size - 1? 0 : queueHead + 1], pieceQueue[queueTail == 0? queue_size - 1 : queueTail - 1], pieceQueue[queueTail]);
		} else {
			printf("0\n");
		}
	}
	for(x = 0; x < board_width + 2; x++){
		printf("0 ");
	}
}

int dequeue(){
	//	saves the next piece to be returned
	int returnVal = pieceQueue[queueHead]; 
	
	//	queue head and tail are adjusted
	queueTail = queueHead;
	queueHead = queueHead + 1 == queue_size? 0 : queueHead + 1;

	//	enqueues a new piece
	pieceQueue[queueTail] = rand() % 7 + 1;

	return returnVal;
}

bool stopPiece(){
	if(
		//	checks origin
		(
			playerPiece == 1 ||
			playerPiece == 2 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 3
			)
			||
			playerPiece == 3 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 1
			)
			||
			playerPiece == 4 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 2
			)
			||
			playerPiece == 5 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 2
			)
			||
			playerPiece == 6 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 3
			)
			||
			playerPiece == 7 && pieceOrientation == 0
		)
		&&
		(
			playerY == board_height - 1 ||
			playerY < board_height - 1 && board[playerY + 1][playerX] != empty
		)
		||
		//	checks left
		(
			playerPiece == 2 && pieceOrientation == 2 ||
			playerPiece == 3 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 3
			)
			||
			playerPiece == 4 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 2
			)
			||
			playerPiece == 5 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 2
			)
			||
			playerPiece == 6 && pieceOrientation == 0 ||
			playerPiece == 7 && pieceOrientation != 1
		)
		&&
		(
			playerY == board_height - 1 ||
			playerY < board_height - 1 && board[playerY + 1][playerX - 1] != empty
		)
		||
		//	checks left left
		playerPiece == 4 && pieceOrientation == 2 &&
		(
			playerY == board_height - 1 ||
			playerY < board_height - 1 && board[playerY + 1][playerX - 2] != empty
		)
		||
		//	checks right
		(
			playerPiece == 1 ||
			playerPiece == 2 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 1
			)
			||
			playerPiece == 3 && pieceOrientation == 2 ||
			playerPiece == 4 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 2
			)
			||
			playerPiece == 5 && pieceOrientation == 0 ||
			playerPiece == 6 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 2
			)
			||
			playerPiece == 7 && pieceOrientation != 3
		)
		&&
		(
			playerY == board_height - 1 ||
			playerY < board_height - 1 && board[playerY + 1][playerX + 1] != empty
		)
		||
		//	checks right right
		playerPiece == 4 && pieceOrientation == 0 &&
		(
			playerY == board_height - 1 ||
			playerY < board_height - 1 && board[playerY + 1][playerX + 2] != empty
		)
		||
		//	checks down
		(
			playerPiece == 2 &&
			(
				pieceOrientation == 1 ||
				pieceOrientation == 2
			)
			||
			playerPiece == 3 &&
			(
				pieceOrientation == 2 ||
				pieceOrientation == 3
			)
			||
			playerPiece == 4 &&
			(
				pieceOrientation == 1 ||
				pieceOrientation == 3
			)
			||
			(
				playerPiece == 5 ||
				playerPiece == 6
			)
			&&
			(
				pieceOrientation == 1 ||
				pieceOrientation == 3
			)
			||
			playerPiece == 7 && pieceOrientation != 0
		)
		&&
		(
			playerY + 1 == board_height - 1 ||
			playerY + 1 < board_height - 1 && board[playerY + 2][playerX] != empty
		)
		||
		//	checks down down
		playerPiece == 4 && pieceOrientation == 1 &&
		(
			playerY + 2 == board_height - 1 ||
			playerY + 2 < board_height - 1 && board[playerY + 3][playerX] != empty
		)
		||
		//	checks upper left
		(
			playerPiece == 2 && pieceOrientation == 0 ||
			playerPiece == 6 && pieceOrientation == 3
		)
		&&
		(
			board[playerY][playerX - 1] != empty
		)
		||
		//	checks upper right
		(
			playerPiece == 3 && pieceOrientation == 0 ||
			playerPiece == 5 && pieceOrientation == 1
		)
		&&
		(
			board[playerY][playerX + 1] != empty
		)
		||
		//	checks lower left
		(
			(playerPiece == 2 || playerPiece == 5) && pieceOrientation == 3 ||
			(playerPiece == 3 || playerPiece == 6) && pieceOrientation == 2
		)
		&&
		(
			playerY + 1 == board_height - 1 ||
			playerY + 1 < board_height - 1 && board[playerY + 2][playerX - 1] != empty
		)
		||
		//	checks lower right
		(
			(playerPiece == 2 || playerPiece == 5) && pieceOrientation == 2 ||
			(playerPiece == 3 || playerPiece == 6) && pieceOrientation == 1
		)
		&&
		(
			playerY + 1 == board_height - 1 ||
			playerY + 1 < board_height - 1 && board[playerY + 2][playerX + 1] != empty
		)
	){
		return true;
	}

	return false;
}

void changePiece(){
	//	origin
	board[playerY][playerX] = occupied;
	//	up
	if(
		playerY > 0 &&
		(
			playerPiece == 1 ||
			playerPiece == 2 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 3
			)
			||
			playerPiece == 3 &&
			(
				pieceOrientation == 0 ||
				pieceOrientation == 1
			)
			||
			(
				playerPiece == 4 ||
				playerPiece == 5 ||
				playerPiece == 6
			)
			&&
			(
				pieceOrientation == 1 ||
				pieceOrientation == 3
			)
			||
			playerPiece == 7 && pieceOrientation != 2
		)
	){
		board[playerY - 1][playerX] = occupied;
	}
	//	up up
	if(playerY > 1 && playerPiece == 4 && pieceOrientation == 3){
		board[playerY - 2][playerX] = occupied;
	}
	//	left
	if(
		playerPiece == 2 &&
		(
			pieceOrientation == 2 ||
			pieceOrientation == 3
		)
		||
		playerPiece == 3 &&
		(
			pieceOrientation == 0 ||
			pieceOrientation == 3
		)
		||
		playerPiece == 4 &&
		(
			pieceOrientation == 0 ||
			pieceOrientation == 2
		)
		||
		(
			playerPiece == 5 ||
			playerPiece == 6
		)
		&&
		(
			pieceOrientation == 0 ||
			pieceOrientation == 2
		)
		||
		playerPiece == 7 && pieceOrientation != 1
	){
		board[playerY][playerX - 1] = occupied;
	}
	//	left left
	if(playerPiece == 4 && pieceOrientation == 2){
		board[playerY][playerX - 2] = occupied;
	}
	//	right
	if(
		playerPiece == 1 ||
		playerPiece == 2 &&
		(
			pieceOrientation == 0 ||
			pieceOrientation == 1
		)
		||
		playerPiece == 3 &&
		(
			pieceOrientation == 1 ||
			pieceOrientation == 2
		)
		||
		playerPiece == 4 &&
		(
			pieceOrientation == 0 ||
			pieceOrientation == 2
		)
		||
		(
			playerPiece == 5 ||
			playerPiece == 6
		)
		&&
		(
			pieceOrientation == 0 ||
			pieceOrientation == 2
		)
		||
		playerPiece == 7 && pieceOrientation != 3
	){
		board[playerY][playerX + 1] = occupied;
	}
	//	right right
	if(playerPiece == 4 && pieceOrientation == 0){
		board[playerY][playerX + 2] = occupied;
	}
	//	down
	if(
		playerPiece == 2 &&
		(
			pieceOrientation == 1 ||
			pieceOrientation == 2
		)
		||
		playerPiece == 3 &&
		(
			pieceOrientation == 2 ||
			pieceOrientation == 3
		)
		||
		(
			playerPiece == 4 ||
			playerPiece == 5 ||
			playerPiece == 6
		)
		&&
		(
			pieceOrientation == 1 ||
			pieceOrientation == 3
		)
		||
		playerPiece == 7 && pieceOrientation != 0
	){
		board[playerY + 1][playerX] = occupied;
	}
	//	down down
	if(playerPiece == 4 && pieceOrientation == 1){
		board[playerY + 2][playerX] = occupied;
	}
	//	upper left
	if(
		playerY > 0 &&
		(
			(playerPiece == 2 || playerPiece == 5) && pieceOrientation == 0 ||
			(playerPiece == 3 || playerPiece == 6) && pieceOrientation == 3
		)
	){
		board[playerY - 1][playerX - 1] = occupied;
	}
	//	upper right
	if(
		playerY > 0 &&
		(
			playerPiece == 1 ||
			(playerPiece == 2 || playerPiece == 5) && pieceOrientation == 1 ||
			(playerPiece == 3 || playerPiece == 6) && pieceOrientation == 0
		)
	){
		board[playerY - 1][playerX + 1] = occupied;
	}
	//	lower left
	if(
		(playerPiece == 2 || playerPiece == 5) && pieceOrientation == 3 ||
		(playerPiece == 3 || playerPiece == 6) && pieceOrientation == 2
	){
		board[playerY + 1][playerX - 1] = occupied;
	}
	//	lower right
	if(
		(playerPiece == 2 || playerPiece == 5) && pieceOrientation == 2 ||
		(playerPiece == 3 || playerPiece == 6) && pieceOrientation == 1
	){
		board[playerY + 1][playerX + 1] = occupied;
	}

	//	removes current player piece
	playerPiece = 0;
}

/*
	PIECE_TYPE(0-7), ORIENTATION(0-3):

	0,x:		5,0:
	 **			*
	 **			*0*
	
	1,0:		5,1:
	**			 **
	 0*			 0
	 			 *

	1,1:	 			 
	  *			5,2:
	 0*			*0*
	 *			  *

	1,2:		5,3:
	*0			 *
	 **			 0
	 			**
	1,3:
	 *			6,0:
	*0			  *
	*			*0*

	2,0:		6,1:
	 **			 *
	*0			 0
				 **
	2,1:
	 *			6,2:
	 0*			*0*
	  *			*

	2,2:		6,3:
	 0*			**
	**			 0
				 *
	2,3:
	*			7,0:
	*0			 *
	 *			*0*

	4,0:		7,1:
	*0**		 *
				 0*
	4,1:		 *
	 *			 
	 0			7,2:
	 *			*0*
	 *			 *

	4,2:		7,3:
	**0*		 *
				*0
	4,3:		 *
	  *	
	  *
	  0
	  *
*/