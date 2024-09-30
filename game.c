#include <stdio.h>
#include <string.h>
#include "game.h"
#include "piece.h"
#include "board.h"

int get_file_index(char file) {return (int)file - 97;}
int get_rank_index(char rank) {return (int)rank - 49;}

void start_game()
{
	piece* board[8][8];
	piece* white_pieces[16];
	piece* black_pieces[16];
	init_board(board, white_pieces, black_pieces);

	int game_over = false;

	while (!game_over)
	{
		display_board(board);
		printf("\n\x1b[37;22m\n");

		char player_input[12];
		piece* piece_to_move = NULL;

		while (piece_to_move == NULL)
		{
			printf("ENTER THE PIECE'S POSITION.\n");
			fgets(player_input, sizeof(player_input), stdin);
			int piece_file = get_file_index(player_input[0]);
			int piece_rank = get_rank_index(player_input[1]);

			if (!is_position_in_board(piece_file, piece_rank))
				continue;

			piece_to_move = board[piece_rank][piece_file];
			memset(player_input, 0, sizeof(player_input));
		}

		int valid_move = false;
		int move_file = 0;
		int move_rank = 0;

		while (!valid_move)
		{
			printf("ENTER MOVE COORDINATES.\n");
			fgets(player_input, sizeof(player_input), stdin);
			move_file = get_file_index(player_input[0]);
			move_rank = get_rank_index(player_input[1]);

			if (!is_position_in_board(move_file, move_rank))
				continue;

			valid_move = process_move(piece_to_move, move_file, move_rank, white_pieces, black_pieces, board);
		}

		set_piece_pos(piece_to_move, move_file, move_rank, board);
	}
}