#include <stdio.h>
#include <stdlib.h>
#include "piece.h"
#include "game.h"



int main()
{
	piece* board[8][8];

	init_board(board);
	start_game(board);

	onexit(clear_board);
	
	return 0;
}