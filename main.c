#include <stdio.h>
#include <stdlib.h>
#include "piece.h"
#include "game.h"
#include "board.h"

int main()
{
	piece* board[8][8];

	start_game(board);
	onexit(clear_board);
	
	return 0;
}