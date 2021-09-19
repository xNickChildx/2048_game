#include <stdio.h>
#include <stdlib.h>

// game dimensions
// number of tiles = D*D
#define D  4
// size of printed board
#define PRINT_D  6

int get_strlen_of_int(int num)
{
	if (num / 10 == 0)
		return 1;
	return get_strlen_of_int( num / 10) + 1;
}

void print_number_line(int board[D][D], int row)
{
	int strlen_of_n, value;
	for (int x = 0; x < D; x++) {
		value = board[row][x];
		strlen_of_n = get_strlen_of_int(value);
		if (value < 0)
			strlen_of_n += 1;
		printf("|");
                for (int y = 0; y < PRINT_D; y++) {
        		if (y == PRINT_D/2 - strlen_of_n/2) {
				if (value == 0) 
					printf(" ");
				else	
					printf("%d", value);
				y += strlen_of_n - 1;
			}
			else
				printf(" ");
		}
	}
}

void print_line(char delim)
{
	for (int i = 0; i < D; i++) {
		printf("|");
		for (int x = 0; x < PRINT_D; x++)
			printf("%c", delim);
	}
}

void print_board(int board[D][D])
{
	printf("\n\n");
	// for every row
	for (int y = 0; y < D; y++) {
		// print the column based on printing size
		for (int b = 0; b < PRINT_D; b++) {
			if (b == 0)
				print_line('-');
			else if (b == PRINT_D / 2)
				print_number_line(board, y);
			else
				print_line(' ');
			printf("|\n");
		}
	}
	print_line('-');
	printf("|\n");

}

int get_open_spaces(int board[D][D], int *open)
{
	int opens_len = 0;
	for (int i = 0; i < D*D; i ++)
		if (board[i/4][i%4] == 0)
			open[opens_len++] = i;
	return opens_len;
}

void update_board(char cmd, int board[D][D])
{
	int tmp_array [D], tmp_i = 0, current_v = 0, indexer;

	for (int n = 0; n < D; n++) {
		//  for every row/col reinitialize values
		tmp_i = 0;
		current_v = 0;
		for (int j = 0; j < D; j++) {
			switch (cmd){
			case 'a':
				current_v = board[n][D - 1 - j];
				break;
			case 'd':
				current_v = board[n][j];
				break;
			case 'w':
				current_v = board[D - 1 - j][n];
				break;
			case 's':
				current_v = board[j][n];
				break;
			default:
				printf("Great Scott! This is impossible!\n");
			}

			if (current_v != 0) {
				tmp_array[tmp_i++] = current_v;
			}
		}
		// now squish
		for (int j = 0; j < tmp_i; j++) {
			// if value should be squished
			if (j != tmp_i -1 && tmp_array[j] == tmp_array[j + 1]) {
				tmp_array[j] += tmp_array[j+1];
				tmp_array[j+1] = -1;
				j++;
			}
		}
		// now write values back to original
		indexer = tmp_i - 1;
		 for (int j = D -1; j >=0; j--) {
			// get next value in tmp to use
			do {	
				if (indexer < 0)
					current_v = 0;
				else 
					current_v = tmp_array[indexer--];
			}
			while (current_v == -1);

                        switch (cmd){
                        case 'a':
                                board[n][D - 1 - j] = current_v;
                                break;
                        case 'd':
                                board[n][j] = current_v;
                                break;
                        case 'w':
                                board[D - 1 - j][n] = current_v;
                                break;
			case 's':
                                board[j][n] = current_v;
                                break;
                        default:
                                printf("Great Scott! This is impossible!\n");
                        }
                }

	}


}
int get_score(int board[D][D])
{
	int score = 0;
	for (int i = 0; i < D*D; i++)
		score += board[i/D][i%D];
	return score;
}
int run_game()
{
	int open_len = 1;
	int board[D][D], open_spaces[D*D];
	int new_num;
	char user_cmd;
	// open spaces shows where we can add a new number
	// as long as there are new numbers we can keep running the game 
	for (int i = 0; i < D*D; i++) {
		board[i/D][i%D] = 0;
		open_spaces[i] = i;
	}
	do {
		new_num = rand()%(open_len);
		board[open_spaces[new_num]/D][open_spaces[new_num]%D] = 2;
		print_board(board);
		// get user input
		do {
			printf("Enter command [ASWD] > ");
			scanf("%c", &user_cmd);
		}
		while (user_cmd != 'w' && user_cmd != 'a' && user_cmd != 's' && user_cmd != 'd');
		// change board
		update_board(user_cmd, board);
		open_len = get_open_spaces(board, open_spaces);
	}
	while(open_len);	
	return get_score(board);
}

int main() 
{
	char cont;
	int score = -1, high_score = 0;
	do {
		score = run_game();
		printf("\n\nGAME OVER\n");
		if (score > high_score) {
			printf("NEW HIGH SCORE! old high was %d\n", high_score);
			high_score = score;
		}
		printf("Your score was %d. High score is %d\n"
			"Type 'q' to quit, anything else to play again\n"
			, score, high_score);
		scanf("%c", &cont);

	}
	while (cont != 'q');

	return 0;
}
