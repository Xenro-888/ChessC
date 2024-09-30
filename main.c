#include <stdio.h>
#include <stdlib.h>
#include "piece.h"
#include "game.h"
#include "board.h"

int main()
{
	start_game();
	onexit(clear_board);
	
	return 0;
}