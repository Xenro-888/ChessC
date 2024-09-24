#include <stdio.h>
#include <string.h>
#include "game.h"
#include "piece.h"
#include "board.h"

int get_file_index(char file)
{    
	return (int)file - 97;
}

int get_rank_index(char rank)
{
	return (int)rank - 49;
}

void start_game(piece* board[8][8])
{
	piece* white_pieces[16];
	piece* black_pieces[16];

	int game_over = false;
	int valid_move = false;

	piece* chosen_piece = NULL;
	piece_color current_turn = WHITE;

	init_board(board, white_pieces, black_pieces);

	while (!game_over)
	{
		char selected_piece_input[12];
		char move_input[12];
		int move_file = 0;
		int move_rank = 0;

		display_board(board);
		printf("\n\x1b[37;22mENTER A MOVE (RF RF):\n");

		while (!valid_move)
		{
			while (chosen_piece == NULL)
			{
				fgets(selected_piece_input, sizeof(selected_piece_input), stdin);
				int chosen_piece_file = get_file_index(selected_piece_input[0]);
				int chosen_piece_rank = get_rank_index(selected_piece_input[1]);

				if (is_position_in_board(chosen_piece_file, chosen_piece_rank))
					chosen_piece = board[chosen_piece_rank][chosen_piece_file];
				else
					printf("INVALID PIECE COORDINATES\n");

				memset(selected_piece_input, 0, sizeof(selected_piece_input));
			}

			fgets(move_input, sizeof(move_input), stdin);
			move_file = get_file_index(move_input[0]);
			move_rank = get_rank_index(move_input[1]);

			if (is_position_in_board(move_file, move_rank))
				valid_move = is_move_valid_basic(chosen_piece, move_file, move_rank, board);
			else
				printf("INVALID MOVE COORDINATES\n");

			memset(move_input, 0, sizeof(move_input));
		}

		if (process_move(chosen_piece, move_file, move_rank, white_pieces, black_pieces, board))
			set_piece_pos(chosen_piece, move_file, move_rank, board);
		else
			printf("INVALID MOVE!\n");

		valid_move = false;
		current_turn = current_turn == WHITE ? BLACK : WHITE; // if the current turn is white, set it to black. if not, set it to white.
	}
}