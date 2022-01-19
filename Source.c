#include<stdio.h>
#include<math.h>

#define SPADE "Spade"
#define CLUB "Club"
#define HEART "Heart"
#define DIAMOND "Diamond"

#define NEW_GAME 100
#define LOAD_GAME 200
#define EXIT_GAME 99
#define SAVE_GAME 50
#define STATUS_GAME 66
#define LEADER_BOARD 77
#define QUIT 999

//void loadGame(int* numberOplayers, int* count, int** players, int** history, int* scores);
void saveGame(int numberOplayers, int count, int** history, int** players);
void showGameStatus(int numberOplayers, int count, int* scores, int** history);
int getNumber(int id);
int getPoints(int id);
void play(int cardIndex, int round, int* player);
void printCard(int id);
void drawCard(int id, int row);
char* getCard(int id);
/*
* Array of chars that holds characters for drawing each card
*/
char chars[] = { ' ', '|', '_', '^', '\\', '/', '(', ')', '-', '<', '>' };
void initializeCards();
void shuffleCards(int cards[], int size);
int cards[52];
char SUITES[4][10] = {SPADE, CLUB, HEART, DIAMOND};

/*
* Each suite is displayed using combination of 11 different characters to model the appearances. 
* Information about how each card suite will be drawn is stored in a Two dimensional jagged array. 
* Each card has 7 rows of characters. What is drawn and where it starts and ends in each entry of the raw is 
  stored in two integer numbers representing per entry. 
  The first number holds information about where each character begins and end and has 24 significant bits
  The second number holds information about what character to be drawn as per 'char chars[]' and has 28 significant bits
*/

// DIAMOND
int DMOND[][2] = {	{(1 << 21) | (0 << 18), 8 << 24},
					{(((3 << 21) | (0 << 18)) | (3 << 15)) | (4 << 12), ((0 << 24) | (3 << 20)) | (0 << 16)},
					{(((((5 << 21) | (0 << 18)) | (2 << 15)) | (3 << 12)) | (4 << 9)) | (5 << 6), ((((0 << 24) | (5 << 20)) | (0 << 16)) | (4 << 12)) | (0 << 8)},
					{(((3 << 21) | (0 << 18)) | (1 << 15)) | (6 << 12), ((9 << 24) | (0 << 20)) | (10 << 16)},
					{(((((5 << 21) | (0 << 18)) | (2 << 15)) | (3 << 12)) | (4 << 9)) | (5 << 6), ((((0 << 24) | (4 << 20)) | (0 << 16)) | (5 << 12)) | (0 << 8)},
					{(1 << 21) | (0 << 18), 0 << 24},
					{(1 << 21) | (0 << 18), 8 << 24}
				};
// SPADE
int SPAD[][2] = {	{(1 << 21) | (0 << 18), 8 << 24},
					{(((3 << 21) | (0 << 18)) | (3 << 15)) | (4 << 12), ((0 << 24) | (3 << 20)) | (0 << 16)}, 
					{(((((5 << 21) | (0 << 18)) | (2 << 15)) | (3 << 12)) | (4 << 9)) | (5 << 6), ((((0 << 24) | (5 << 20)) | (0 << 16)) | (4 << 12)) | (0 << 8)},
					{(((((5 << 21) | (0 << 18)) | (1 << 15)) | (2 << 12)) | (5 << 9)) | (6 << 6), ((((0 << 24) | (6 << 20)) | (0 << 16)) | (7 << 12)) | (0 << 8)},
					{(((3 << 21) | (0 << 18)) | (3 << 15)) | (4 << 12), ((0 << 24) | (3 << 20)) | (0 << 16)},
					{(1 << 21) | (0 << 18), 0 << 24},
					{(1 << 21) | (0 << 18), 8 << 24}
				};
// HEART
int HART[][2] = {	{(1 << 21) | (0 << 18), 8 << 24},
					{(((((5 << 21) | (0 << 18)) | (1 << 15)) | (2 << 12)) | (4 << 9)) | (5 << 6), ((((0 << 24) | (2 << 20)) | (0 << 16)) | (2 << 12)) | (0 << 8)},
					{(((((((7 << 21) | (0 << 18)) | (1 << 15)) | (2 << 12)) | (3 << 9)) | (4 << 6)) | (5 << 3)) | (6 << 0), ((((((6 << 24) | (0 << 20)) | (4 << 16)) | (5 << 12)) | (0 << 8)) | (7 << 4)) | (0 << 0)},
					{(((((5 << 21) | (0 << 18)) | (1 << 15)) | (2 << 12)) | (4 << 9)) | (5 << 6), ((((0 << 24) | (4 << 20)) | (0 << 16)) | (5 << 12)) | (0 << 8)},
					{((((4 << 21) | (0 << 18)) | (2 << 15)) | (3 << 12)) | (4 << 9), (((0 << 24) | (4 << 20)) | (5 << 16)) | (0 << 12)},
					{(1 << 21) | (0 << 18), 0 << 24},
					{(1 << 21) | (0 << 18), 8 << 24},
				};
// CLUB
int CLB[][2] = {	{(1 << 21) | (0 << 18), 8 << 24},
					{(((3 << 21) | (0 << 18)) | (3 << 15)) | (4 << 12), ((0 << 24) | (2 << 20)) | (0 << 16)},
					{(((((5 << 21) | (0 << 18)) | (2 << 15)) | (3 << 12)) | (4 << 9)) | (5 << 6), ((((0 << 24) | (6 << 20)) | (0 << 16)) | (7 << 12)) | (0 << 8)},
					{(((((5 << 21) | (0 << 18)) | (1 << 15)) | (3 << 12)) | (4 << 9)) | (6 << 6), ((((6 << 24) | (0 << 20)) | (2 << 16)) | (0 << 12)) | (7 << 8)},
					{(((3 << 21) | (0 << 18)) | (3 << 15)) | (4 << 12), ((0 << 24) | (3 << 20)) | (0 << 16)},
					{(1 << 21) | (0 << 18), 0 << 24},
					{(1 << 21) | (0 << 18), 8 << 24}
				};
					 

int getNumberOplayers(int GAME, FILE* fptr);
int getCount(int GAME, FILE* fptr);
void loadPlayers(int numberOplayers, int** players, int GAME, FILE* fptr);
void loadHistory(int numberOplayers, int** history, int GAME, FILE* fptr);
void loadScores(int numberOplayers, int count, int** history, int* scores, int GAME, int* pCarryOver);
int getWinner(int numberOplayers, int count, int* history, int* pSum);
void showRoundSummary(int numberOplayers, int count, int* scores, int** history, int* pCarryOver);
void showGameOver(int numberOplayers, int* scores, int go);

int getWinner(int numberOplayers, int count, int* history, int* pSum) {
	int max = -1;
	for (int i = 0; i < numberOplayers; i++) {
		*pSum = *pSum + getPoints(history[i]);
		if (max == -1 || getPoints(history[i]) > getPoints(history[max])) {
			int temp = max;
			max = i;
			for (int j = 0; j < numberOplayers; j++) {
				if (i == j) {
					continue;
				}
				if (getPoints(history[i]) == getPoints(history[j])) {
					max = temp;
					break;
				}
			}
		}
	}
	return max;
}
void loadScores(int numberOplayers, int count, int** history, int* scores, int GAME, int* pCarryOver) {
	if (GAME == LOAD_GAME) {
		for (int i = 0; i < count / numberOplayers + 1; i++) {
			int sum = 0;
			int redundant = 0;
			for (int j = 0; j < numberOplayers && (i * numberOplayers + j) < count; j++) {
				sum += getPoints(history[i][j]);
			}
			if (count >= i * numberOplayers + numberOplayers - 1) { // FINISHED ROUND
				int winner = getWinner(numberOplayers, count, history[i], &redundant);
				if (winner != -1) {
					scores[winner] += (sum + *pCarryOver);
					*pCarryOver = 0;
				}
				else {
					*pCarryOver = *pCarryOver + sum;
				}
			}
		}
		
	}
	return scores;
}

/*
 * A function that retrieves the number of players.  
 * When GAME is NEW_GAME it will prompt the user to Enter number of players. 
 * Else if it is LOAD_GAME it will load from FILE*
*/
int getNumberOplayers(int GAME, FILE* fptr) {
	int numberOplayers = 0;
	int MAX = 4;
	int MIN = 2;
	if (GAME == LOAD_GAME) {		
		fscanf(fptr, "%x", &numberOplayers);
	}
	else {
		do{
			printf("\nEnter number of Players (2 - 4): ");
			scanf(" %d", &numberOplayers);
			while ((getchar()) != '\n'); // FLUSHING INPUT STREAM https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/ - Manjeet Singh
		}while (numberOplayers < MIN || numberOplayers > MAX);
	}
	return numberOplayers;
}
/*
 * A function that retrieves the number of cards played.
 * When GAME is NEW_GAME it resets count to 0.
 * Else if it is LOAD_GAME it will load from FILE*
 * Data is written and read-in Hexadecimal format
*/
int getCount(int GAME, FILE* fptr) {
	int count = 0;
	if (GAME == LOAD_GAME) {
		fscanf(fptr, "%x", &count);
	}
	return count;
}
/*
* This function shuffles and deals card if GAME is NEW_GAME
* It will retrieve each players card from FILE* if GAME is LOAD_FILE
* Data is written and read-in Hexadecimal format
*/
void loadPlayers(int numberOplayers, int** players, int GAME, FILE* fptr) {
	if (GAME == LOAD_GAME) {
		for (int i = 0; i < numberOplayers; i++) {
			for (int j = 0; j < 13; j++) {
				fscanf(fptr, "%x", &players[i][j]);
			}
		}
	}
	else {
		shuffleCards(cards, 52);
		for (int i = 0; i < 52; i++) {
			int player = i % numberOplayers;
			int card = i / numberOplayers;
			players[player][card] = cards[i];
		}
	}
}
/*
* This method does nothing if GAME is NEW_GAME and will retrieve previously played cards
* from FILE* if GAME is LOAD_GAME
* Data is read-in Hexadecimal format
*/
void loadHistory(int numberOplayers, int** history, int GAME, FILE* fptr) {
	if (GAME == LOAD_GAME) {
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < numberOplayers; j++) {
				fscanf(fptr, "%x", &history[i][j]);
			}
		}
	}
}
// Main function
void main() {
	int response;
	printf("\t\t\t***!!!!! WAR !!!!***\n");
	initializeCards();
	do {
		// Prompts the user to enter options
		printf("Please enter option (%d - New Game, %d - Load Saved Game, %d - Quit) ", NEW_GAME, LOAD_GAME, QUIT);
		if (scanf("%d", &response) < 1 || (response != NEW_GAME && response != LOAD_GAME && response != QUIT)) {
			while ((getchar()) != '\n'); // FLUSHING INPUT STREAM https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/ - Manjeet Singh
			continue;
		}
		else if (response == QUIT) {
			break;
		}
		// Pointer to the file to save game
		FILE* fptr = fopen("saved_game", "r");

		int numberOplayers = getNumberOplayers(response, fptr);
		int count = getCount(response, fptr);
		// Allocate memory to store number of players and cards in a two dimensional jagged array
		int** players = malloc(numberOplayers * sizeof(int*));
		// Allocate memory for each array in the players array
		for (int i = 0; i < numberOplayers; i++) {
			players[i] = malloc(52 / numberOplayers * sizeof(int));
		}
		loadPlayers(numberOplayers, players, response, fptr);
		// Allocate memory to store history of cards played through 13 rounds by each player
		int** history = malloc(13 * sizeof(int*));
		// Allocate memory for cards for each player
		for (int i = 0; i < 13; i++) {
			history[i] = malloc(numberOplayers * sizeof(int));
		}
		loadHistory(numberOplayers, history, response, fptr);
		// Allocate memory to store the current total score of each player
		int* scores = malloc(numberOplayers * sizeof(int));
		// Initialize each players score to 0
		for (int i = 0; i < numberOplayers; i++) {
			scores[i] = 0;
		}
		/* Declare and initialize carryOver variable which represents score carried over
		*  from a previous round with no winner
		*/
		int carryOver = 0;
		loadScores(numberOplayers, count, history, scores, response, &carryOver);
		// close FILE pointer
		fclose(fptr);
		/*
		* This portion of code only runs when response is LOAD_GAME to display previously played rounds
		* Redundant scores (rscores) and count (redundant) are used to avoid corrupting the scores and count pointer variables. 
		*/
		if (response == LOAD_GAME) {
			int* rscores = malloc(numberOplayers * sizeof(int));
			for (int i = 0; i < count / numberOplayers; i++) {
				int redundant = 0;
				showRoundSummary(numberOplayers, (i + 1) * numberOplayers - 1, rscores, history, &redundant);

		//		count% numberOplayers == (numberOplayers - 1)

			}
			free(rscores);
			showGameStatus(numberOplayers, count, scores, history);
		}
		// A variable to monitor when each round ends
		int roundEnd = 0;
		do {

			// For formatting
			if (roundEnd == 0 && response != STATUS_GAME && response != LOAD_GAME && response != LEADER_BOARD){//response != STATUS_GAME) {// || roundEnd != 1)
				printf("\033[2J\033[H");
			}
			printf("\nPlayer %d to play. Press Enter to show cards ", (count % numberOplayers) + 1);
			while ((getchar()) != '\n');
			char res = 0;
			scanf("%c", &res);
			printf("\n");
			/*
			* This portion of code draws the cards on the screen
			*/
			int row = 8, column = 9, sp = 4;
			for (int r = 0; r < row; r++) {
				for (int i = 0; i < 13 - (count / numberOplayers); i++) {
					if (r == 7) {
						for (int k = 0; k < sp; k++) {
							printf(" ");
						}
						printf("%d", i);
						sp = 10 - ((i > 9) ? 1 : 0);
						if (i == 12) {
							break;
						}
						continue;
					}
					int card = players[count % numberOplayers][i];
					drawCard(card, r);
					printf("  ");
				}
				printf("\n");
			}

			

			printf("\nEnter index of the card you wish to play or (%d - EXIT, %d - SAVE, %d - STATUS, %d - LEADER BOARD) ", EXIT_GAME, SAVE_GAME, STATUS_GAME, LEADER_BOARD);
			int value = scanf(" %d", &response);
			
			int ok = ((response < (13 - count / numberOplayers) && response >= 0) || (response == EXIT_GAME || response == SAVE_GAME || response == STATUS_GAME || response == LEADER_BOARD));
			// Validate entry
			while (value == 0 || !ok) {
				printf("\nEnter index of the card you wish to play or (%d - EXIT, %d - SAVE, %d - STATUS, %d - LEADER BOARD) ", EXIT_GAME, SAVE_GAME, STATUS_GAME, LEADER_BOARD);
				while ((getchar()) != '\n'); // FLUSHING INPUT STREAM https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/ - Manjeet Singh
				value = scanf(" %d", &response);
				
				ok = ((response < (13 - count / numberOplayers) && response >= 0) || (response == EXIT_GAME || response == SAVE_GAME || response == STATUS_GAME || response == LEADER_BOARD));
			}
			if (response == EXIT_GAME) { // Exits game
			/* 
			*	https://stackoverflow.com/questions/41430453/how-to-clear-console-in-c-on-windows-using-netbeans-cygwin
			*	-chqrlie
			*/
				system("cls");
				break;
			}
			else if (response == SAVE_GAME) { // Save game
				saveGame(numberOplayers, count, players, history);
				printf("\033[2J\033[H");
				break;
			}
			else if (response == LEADER_BOARD) { // Clear screen and show Leader board
				printf("\033[2J\033[H");
				showGameOver(numberOplayers, scores, 0);
				continue;
			}
			else if (response == STATUS_GAME) { // Clear screen and show status using redundant pointers. 
				printf("\033[2J\033[H");

				int* rscores = malloc(numberOplayers * sizeof(int));
				for (int i = 0; i < count / numberOplayers; i++) {
					int redundant = 0;
					showRoundSummary(numberOplayers, (i + 1) * numberOplayers - 1, rscores, history, &redundant);
				}
				free(rscores);


				showGameStatus(numberOplayers, count, scores, history);
				
				continue;
			}
			else { // CARD PLAYED
				// Retrieve card played
				int card = players[count % numberOplayers][response];
				play(response, count / numberOplayers, players[count % numberOplayers]);
				// update history with current card played
				history[count / numberOplayers][count % numberOplayers] = card;

				if (count % numberOplayers == (numberOplayers - 1)) { // Shwo round summary for each end of round
					printf("\033[2J\033[H");
					showRoundSummary(numberOplayers, count, scores, history, &carryOver);
					roundEnd = 1;
				}
				else {
					roundEnd = 0;
				}
				
				if (((count + 1) / numberOplayers + 1) > 13) { // GAME OVER
					showGameOver(numberOplayers, scores, 1);
					break;
				}
			}
			// Increment count
			count++;
		} while (1);
		// Deallocate memory(s)
		for (int i = 0; i < numberOplayers; i++) {
			free(players[i]);
		}
		free(players);
		free(scores);
	}while (1);
}

/*This function saves the status of the game into a file in the following format
* number of players - count
* player1 cards
* .
* playerN cards
* history of cards played
*/
void saveGame(int numberOplayers, int count, int** players, int** history) {
	printf("Saving Game... ");
	FILE* fptr = fopen("saved_game", "w");
	fprintf(fptr, "%x %x\n", numberOplayers, count);
	int round = count / numberOplayers + 1;
	for (int i = 0; i < numberOplayers; i++) {
		for (int j = 0; j < 13; j++) {
			fprintf(fptr, "%x ", players[i][j]);
		}
		fprintf(fptr, "\n");
	}
	for (int i = 0; i < round; i++) {
		for (int j = 0; j < numberOplayers && (i * numberOplayers + j) < count; j++) {
			fprintf(fptr, "%x ", history[i][j]);
		}
		fprintf(fptr, "\n");
	}
	fclose(fptr);
}
/*
* This function will by default show leader board and will
* show the game over dialog if game over is detected
*/
void showGameOver(int numberOplayers, int* scores, int go) {
	int* rank = malloc(sizeof(int) * numberOplayers);
	int* index = malloc(sizeof(int) * numberOplayers);
	for (int i = 0; i < numberOplayers; i++) {
		rank[i] = 0;
		index[i] = -1; 
	}

	for (int i = 0; i < numberOplayers; i++) {
		int a = 0;
		for (int j = 0; j < numberOplayers; j++) {
			if (i == j) {
				continue;
			}
			if (scores[i] >= scores[j]) {
				a++;
			}
		}
		rank[i] = numberOplayers - a;
		if (index[rank[i] - 1] != -1) {
			for (int k = 0; ; k++) {
				if (index[rank[i] - 1 + k] == -1) {
					index[rank[i] - 1 + k] = i;
					break;
				}
			}
		}
		else {
			index[rank[i] - 1] = i;
		}
	}
	int rows = 2 * (numberOplayers + 2) + 1;
	const int marker1 = 6 + 1, marker2 = marker1 + 16 + 1, marker3 = marker2 + 6 + 1;
	for (int i = 0; i < rows; i++) {
		int cols = 0;
		if (i % 2 == 0) {
			printf("\t\t+");
			cols++;
			for (int j = 0; j < marker3; j++) {
				if (i != 0 && (cols == marker1 || cols == marker2)) {
					printf("+");
				}
				else {
					printf((i == 0 || i == 2) ? "=" : "-");
				}
				cols++;
			}
			printf("+");
		}
		else {
			printf("\t\t|");
			cols++;
			if (i == 1) {
				for (int x = 0; x < 10; x++) {
					printf("%c", ' ');
					cols++;
				}
				printf(go ? "Game Over" : "Leader Board");
				cols += (go ? 9 : 12);
				for (int x = 0; x < marker3 - cols + 1; x++) {
					printf("%c", ' ');
				}
				printf("|");
			}
			else if (i == 3) {
				printf("Rank");
				//printf("Marker3 = %d", marker3);
				cols += 4;
				int xx = cols;
				for (int x = 0; x < marker3 - xx + 1; x++) {
					if (cols == marker1 || cols == marker2) {
						printf("%s", (cols == marker1) ? "|Player" : "|Score ");
						cols += 7;
					}
					else if (cols == marker3) {
						printf(" ");
						break;
					}
					else {
						printf("%c", ' ');
						cols++;
					}
				}
				printf("|");
			}
			else {
				printf(" %d", rank[index[(i - 1) / 2 - 2]]);
				cols += 2;
				int xx = cols;
				for (int x = 0; x < marker3 - xx + 1; x++) {
					if (cols == marker1) {
						printf("|Player%d", index[(i - 1) / 2 - 2] + 1);
						cols += 8;
					} 
					else if(cols == marker2) {
						int score = scores[index[(i - 1) / 2 - 2]];
						printf("| %d", score);
						cols += (score > 9 ? (score > 99 ? 5 : 4) : 3);
					}
					else if (cols == marker3) {
						printf(" ");
						break;
					}
					else {
						printf(" ");
						cols++;
					}
				}
				printf("|");
			}
		}
		printf("\n");
	}
	free(rank);
	free(index);
}

// This function displays the game status
void showGameStatus(int numberOplayers, int count, int* scores, int** history) {
	int rows = 2 * (3 + numberOplayers) + 1;
//	int cols = 0;
	const int marker = 30;
	int tab = 2;
	for (int i = 0; i < rows; i++) {
		int cols = 0;
		if (i % 2 == 0) {
			printf("+");
			cols++;
			for (int j = 0; j < marker - cols + 6 + 1; j++) {
				printf("%c", (j == marker - cols || j == marker - cols + 6) ? '+' : '-');
			}
		}
		else {
			printf("|");
			cols++;
			for (int j = 0; j < tab; j++) {
				printf(" ");
				cols++;
			}
			if (i == 1) {
				printf("Current Round");
				cols += 13;
			}
			else if (i == 3) {
				printf("Number of Players");
				cols += 17;
			}
			else if (i == 5) {
				printf("Scores");
				cols += 6;
			}
			else {
				for (int k = 0; k < tab; k++) {
					printf(" ");
					cols++;
				}
				printf("Player%d", (i - 1) / 2 - 2);
				cols += 7;
			}
			for (int z = 0; z < (marker - cols); z++) {
				printf("%c", ' ');// marker - cols);
			}

			printf("%c", i == 5 ? ' ' : '|');
			cols = 0;
			if (i == 1) {
				printf(" %d", count / numberOplayers + 1);
				cols = 2 + (count / numberOplayers > 8 ? 1 : 0);
			}
			else if (i == 3) {
				printf(" %d", numberOplayers);
				cols = 2;
			}
			else if (i == 5) {
				cols = 0;
			}
			else {
				printf(" %d", scores[(i - 1) / 2 - 2 - 1]);
				cols = 2 + (scores[(i - 1) / 2 - 2 - 1] > 9 ? (scores[(i - 1) / 2 - 2 - 1] > 99 ? 2 : 1) : 0);
			}
			for (int k = 0; k < 5 - cols; k++) {
				printf(" ");
			}
			printf("|");
			
		}
		printf("\n");
	}
//	showGameOver(numberOplayers, scores);
}
// This method displays the summary of round as per count
void showRoundSummary(int numberOplayers, int count, int* scores, int** history, int* pCarryOver) {
	int sum = 0;
	int max = getWinner(numberOplayers, count, history[count / numberOplayers], &sum);
	int spc = 5;
	printf("%c", '+');
	for (int x = 0; x < ((9 * numberOplayers) + spc + spc + ((numberOplayers - 1) * spc)); x++) {
		printf("%c", '-');
	}
	printf("%c\n", '+');
	int row = 8, column = 9, sp = 4;
	for (int r = 0; r < row; r++) {
		printf("%c", '|');
		for (int x = 0; x < spc; x++) {
			printf("%c", ' ');
		}
		for (int i = 0; i < numberOplayers; i++) {
			if (r == 7) {
				printf(" %s%d ", "Player", (i + 1));
				sp = 10 - ((i > 9) ? 1 : 0);
				if (i == 12) {
					break;
				}
				for (int x = 0; x < spc; x++) {
					printf("%c", ' ');
				}
				continue;
			}
			int card = history[count / numberOplayers][i];
			drawCard(card, r);
			for (int x = 0; x < spc; x++) {
				printf("%c", ' ');
			}
		}

		printf("%c", '|');
		printf("\n");
	}
	printf("%c", '+');
	int marker = 12;
	for (int x = 0; x < ((9 * numberOplayers) + spc + spc + ((numberOplayers - 1) * spc)); x++) {
		printf("%c", x == marker ? '+' : '-');
	}
	printf("%c\n", '+');
	int rows = 1;
	for (int i = 0; i < 6; i++) {
		rows = 1;
		if (i % 2 == 0) {
			printf("%c", '|');
			if (i == 0) {
				rows += 5;
				printf("Round");
			}
			else if (i == 2) {
				rows += 6;
				printf("Winner");
			}
			else if (i = 4) {
				rows += 6;
				printf("Points");
			}
			for (int x = 0; x < marker - rows + 1; x++) {
				printf("%c", ' ');
			}
			printf("%c", '|');
			rows = 2;
			if (i == 0) {
				printf(" %d", count / numberOplayers + 1);
				rows += (1 + ((count / numberOplayers > 8) ? 1 : 0));
			}
			else if (i == 2) {
				if (max != -1) {
					printf(" %s%d", "Player", (max + 1));
					rows += 7;
				}
				else {
					printf(" %s", "Tie Game!");
					rows += 9;
				}
			}
			else if (i == 4) {
				printf(" %d", sum);
				rows += (0 + (sum > 9 ? (sum > 99 ? (3) : (2)) : (1)));
			}


			for (int x = 0; x < ((9 * numberOplayers) + spc + spc + ((numberOplayers - 1) * spc)) - marker - rows; x++) {
				printf("%c", ' ');
			}
			printf("%c", '|');
		}
		else {
			printf("%c", '+');
			for (int x = 0; x < marker; x++) {
				printf("-");
			}
			rows = marker + 1;
			for (int x = 0; x < marker - rows + 1; x++) {
				printf("%c", ' ');
			}
			printf("%c", '+');
			for (int x = 0; x < ((9 * numberOplayers) + spc + spc + ((numberOplayers - 1) * spc)) - marker - 1; x++) {
				printf("%c", '-');
			}
			printf("%c", '+');
		}


		printf("\n");
	}

	if (max != -1) {
//		printf("max = ");
//		printCard(history[count / numberOplayers][max]);
		scores[max] += (sum + *pCarryOver);
		*pCarryOver = 0;
	}
	else {
		printf("!!! TIE !!! - The round is tie, points will be carried over to the next round\n");
		*pCarryOver = *pCarryOver + sum;
	}
	printf("\n");
}

/*
* This method emulates playing a card and will move the selected card to 
* the end of the player array and move subsequent ones one step backward
*/
void play(int cardIndex, int round, int* player) {
	int temp = player[cardIndex];
//	player[cardIndex] = player[13 - 1 - round];
//	player[13 - 1 - round] = temp;
	for (int i = 0; i < 13 - 1 - cardIndex; i++) {		
		player[cardIndex + i] = player[cardIndex + i + 1];
	}
	player[13 - 1] = temp;
}

/*
* This method creates all cards of all suites as a single integer value per card. 
* The integer has 6 significant bits with the first two (XX00000) representing the Suite -(00 - Spade, 
* 01 - Club, 10 - Heart, 11 - Diamond)
* The proceeding 4 bits (00XXXX) represents the card number - (0001 - A, 0010 - 2, 0011 - 3, ...., 1101 - K) 
*/
void initializeCards() {
	for (int i = 0; i < 4; i++) {
//		printf("Suite: %d\n", i);
		for (int j = 0; j < 13; j++) {
			cards[(i * 13) + j] = (i << 4) + (j + 1);
	//		printf("\tCard: %d = %d\n", j, cards[(i * 13) + j]);
	//		printCard(cards[(i * 13) + j]);
	//		printf("\t\t%s", getCard((i << 4) + (j + 1)));
		}
	}
}
/* 
* This function draws a given row of each card (id) 
*/
void drawCard(int id, int row) {
	char numChar[] = "  ";
	int num = getNumber(id);
	if (num == 1) {
		numChar[0] = 'A'; numChar[1] = ' ';// , numChar[2] = "\0";
	}
	else if (num > 10) {
		int add[] = {63, 69, 62};
		numChar[0] = (char)(num + add[num - 11]);
	}
	else if (num == 10) {
		numChar[0] = '1'; 
		numChar[1] = '0';
	}
	else {
		numChar[0] = (char)(num + 48);
	}
	int (*DRAW)[2];
	int suite = getSuite(id);
	switch (suite) {
	case 0: // SPADE
		DRAW = SPAD;
		break;
	case 1: // CLUB
		DRAW = CLB;
		break;
	case 2: // HEART
		DRAW = HART;
		break;
	case 3: // DIAMOND
		DRAW = DMOND;
		break;
	}
	int length = DRAW[row][0] >> 21;
//	printf("Length = %d\n", length);
	if (row != 0 && row != 6) {
		printf("%c", '|');
	}
	else {
		printf("%c", '+');
	}
	for (int i = 0; i < length; i++) {	
//		printf("Data : %d \t&\t %d\t>> %d", DRAW[row][0], ((1 << (21 - (i * 3))) - 1), (18 - (((i + 1) * 3))));
		int index = (DRAW[row][0] & ((1 << (21 - (i * 3))) - 1)) >> (18 - ((i * 3)));
		
		int nextNode = (i == (length - 1)) ? 7 : (DRAW[row][0] & ((1 << (21 - ((i + 1) * 3))) - 1)) >> (18 - (((i + 1) * 3)));
//		printf("\ncounter = %d\ti = %d\t(i + 1) = %d", i, index, nextNode);
		for (int j = index; j < nextNode; j++) {
			if (row == 1 && j == 5) {
				printf("%s", numChar);
				break;
			}
			printf("%c", chars[(DRAW[row][1] & ((1 << (28 - (i * 4))) - 1)) >> (24 - (i * 4))]);
		}
	}
	if (row != 0 && row != 6) {
		printf("%c", '|');
	}
	else {
		printf("%c", '+');
	}
}

/*
* Retrieves the number for a given card by eliminating the first two bits (XX0000)
* of the integer value of the card. 
*/
int getNumber(int id) {
	return id & ((1 << 4) - 1);
}

// Gets the point equivalent for each card
int getPoints(int id) {
	int number = getNumber(id);
	return number == 1 ? 14 : number;
}

/*
* Retrieves the suite of each given card by eliminating the last 4 bits (00XXXX)
* of the integer value of the card
*/
int getSuite(int id) {
	return id >> 4;
}

void printCard(int id) {
	int suite = id >> 4;
	int number = id & ((1 << 4) - 1);
	printf("\t\tCard(%d): %s %d\n", id, SUITES[suite], number);
}

char* getCard(int id) {
	return strcat(SUITES[id >> 4], id & 15);
}

void shuffleCards(int cards[], int size) {
	srand(time(0));
	for (int i = 0; i < size; i++) {
		int j = rand() % size;
		int t = cards[i];
		cards[i] = cards[j];
		cards[j] = t;
	}
}