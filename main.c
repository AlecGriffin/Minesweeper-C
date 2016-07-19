#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define Board_Size 15
#define Bomb_Probability .10

// Boolean Variable
typedef enum {

	false,
	true
} Boolean;

typedef struct 
{
	int visibilityLayer[Board_Size][Board_Size];
	char boardLayer[Board_Size][Board_Size];
} Board;

typedef struct Coordinates{
	int row;
	int col;

} Coordinate;

void boardInit(Board *board);
void printVisLayer(Board *board);
void fillBoardWithBombs(Board *board);
float generateRandomNumbers();
void printBoardLayer(Board *board);
void populateBoardWithNumBombsNearby(Board *board);
int determineNumBombsAtPosition(Board *board, int row, int col);
Coordinate * getPlayerCoordinate();
void recursiveChecker(Board *board, int row, int col);
int modifyVisLayer(Board *board, int row, int col);
void gameLoop();
void printBoardLayerShowAll(Board *board);
int checkCoordinateValidity(int row, int col);

int main(){
	printf("%s\n", "\nWELCOME TO MINESWEEPER!!!");
	gameLoop();
	printf("%s\n", "Game Over!!!");
	return 1;
}

void gameLoop(){

	printf("HELLO %s\n", "Game Loop Starts!");
	Board board;
	boardInit(&board);


	Boolean isOver = false;
	int i = 1;

	while(!isOver){
		// printf("%s\n", "Round OnGoing");

		// 1.) Generate / Display Board
		printBoardLayer(&board);

		// 2.) Ask for row and column input
		// printBoardLayerShowAll(&board);
		Coordinate coordinate= *getPlayerCoordinate();
		

		// 3.) Update Board based on user input / Determine if player hit a bomb
	
		int result = modifyVisLayer(&board,coordinate.row, coordinate.col);
		// printVisLayer(&board); // For Debuggin!!!
		if(result == -1)
			isOver = true;

	}

}

Coordinate * getPlayerCoordinate(){
	Coordinate result;

	Coordinate * resultRef = &result;

	int isValid = 0;

while(!isValid){
	printf("Please Enter Row: ");
	scanf("%i", &(resultRef->row));


	printf("Please Enter Column: ");
	scanf("%i", &(resultRef->col ));

	isValid = checkCoordinateValidity(result.row, result.col);
	if(!isValid)
		printf("\nInvalid Input --> %s %i    %s %i\n", "Row:", result.row, "Col:", result.col);
		printf("%s\n", "Please enter another coordinate!");
}

	return resultRef;
}

//Post: Returns 1 if valid input
//	    Returns 0 if invalid input
int checkCoordinateValidity(int row, int col){
	return col >= 0 && col < Board_Size && row >= 0 && row < Board_Size;
}





void boardInit(Board *board){

	// Sets visibility Layer
	int i;
	for(i = 0;i < Board_Size; i++){

		int j;
		for(j = 0;j < Board_Size; j++){
			board->visibilityLayer[i][j] = 0;
		}
	}

	// Create Random Board With Bombs and #'s in place'
	fillBoardWithBombs(board);
	populateBoardWithNumBombsNearby(board);
}

void fillBoardWithBombs(Board *board){

	srand(time(NULL));


	//	Assigns Bombs To Board
	int i;
	for(i = 0; i < Board_Size; i++){

		int j;
		for(j = 0; j < Board_Size; j++){
			float randNum = generateRandomNumbers();

			if(randNum < Bomb_Probability){
				board->boardLayer[i][j] = '*';
			}else{
				board->boardLayer[i][j] = '0';
			}
		}
	}

}

//Post: Returns a random number from 0 - 1.0
float generateRandomNumbers(){
	
	float randNum = (rand() % 100) / 100.0;

	return randNum;
}

void printVisLayer(Board *board){

	int i;
	for(i = 0;i < Board_Size; i++){

		int j;
		for(j = 0;j < Board_Size; j++){
			int value = board->visibilityLayer[i][j];
			if(value == 1){
				printf("%d ", value);
			}else{
				printf("- ");
			}
			
		}
		printf("\n");
	}
}

void printBoardLayer(Board *board){

	int i;
	printf("\n");
	printf("\n");

	int k;
	for(k = 0; k < Board_Size ;k++)
		printf("%i ", k);

	printf("\n");
	for(i = 0;i < Board_Size; i++){
		printf("%i ", i + 1);

		int j;
		for(j = 0;j < Board_Size; j++){

			if(board->visibilityLayer[i][j] == 1){
				char value = board->boardLayer[i][j];
				printf("%c ", value);
			}else{
				printf("%c ", 'X');
			}
	
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

void printBoardLayerShowAll(Board *board){

	int i;
	printf("\n");
	printf("\n");

	for(i = 0;i < Board_Size; i++){

		int j;
		for(j = 0;j < Board_Size; j++){
				char value = board->boardLayer[i][j];
				printf("%c ", value);
			
	
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

void populateBoardWithNumBombsNearby(Board *board){
		int i;
	for(i = 0;i < Board_Size; i++){

		int j;
		for(j = 0;j < Board_Size; j++){
			char c = board->boardLayer[i][j];
			if(c != '*'){
				board->boardLayer[i][j] = determineNumBombsAtPosition(board, i, j) + '0';
			}

	
		}
	}
}

int determineNumBombsAtPosition(Board *board, int row, int col){
	int bombCount = 0;

		int rowIndex;
	for(rowIndex = -1;rowIndex <= 1; rowIndex++){

		int colIndex;
		for(colIndex = -1;colIndex <= 1; colIndex++){

			if(row + rowIndex >= 0 && row + rowIndex < Board_Size && col + colIndex >= 0 && col + colIndex < Board_Size){
				char c = (char) board->boardLayer[row + rowIndex][col + colIndex];

				if(c == '*'){
					bombCount++;
				}
			}
	
		}
	}
	return bombCount;
}

int modifyVisLayer(Board *board, int row, int col){
	if(board->boardLayer[row][col] == '*'){
		// Player Hit a Bomb!
		return -1;
	}

	if(col >= 0 && col < Board_Size && row >= 0 && row < Board_Size)
		recursiveChecker(board, row, col);
	else
		printf("%s\n", "Invalid Input");

	return 1;
}

//Post: Display Board if player enters a row/col containing a non-bomb
//		Otherwise, informs the user he lost if he hits a bomb
//
// Assume input is a valid poisition on the bpard!
void recursiveChecker(Board *board, int row, int col){
	board->visibilityLayer[row][col] = 1;

	if(board->boardLayer[row][col] == '0'){

		// Checks the Surronding Spaces 
		int i;
		for( i = -1; i <= 1 ; i++){
			int j;
			for( j = -1; j <= 1; j++){
				int surroundingRow = row + i;
				int surroundingCol = col + j;

				if(  surroundingRow >= 0 && surroundingCol >= 0 && surroundingRow < Board_Size && surroundingCol < Board_Size && board->visibilityLayer[surroundingRow][surroundingCol] == 0){
					recursiveChecker(board, surroundingRow, surroundingCol);
				}

			}
		}
	}

}

