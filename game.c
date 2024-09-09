#include <stdio.h>
#include <string.h>
#include "game.h"
#include "piece.h"

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
	char move_input[12];
	int game_over = 0;
	int valid_move = false;
	piece_color current_turn = WHITE;

	while (!game_over)
	{
		display_board(board);

		printf("\n\x1b[37;22mENTER A MOVE (RF RF):\n");

		while (!valid_move)
		{
			fgets(move_input, sizeof(move_input), stdin);

			int start_pos_file = get_file_index(move_input[0]);
			int start_pos_rank = get_rank_index(move_input[1]);
			int move_pos_file = get_file_index(move_input[3]);
			int move_pos_rank = get_rank_index(move_input[4]);

			if (
				is_position_in_board(start_pos_file, start_pos_rank) &&
				is_position_in_board(move_pos_file, move_pos_rank)
				)
			{
				piece* selected_piece = board[start_pos_rank][start_pos_file];

				if (selected_piece != NULL && selected_piece->color == current_turn)
				{
					valid_move = move_piece(selected_piece, move_pos_file, move_pos_rank, board);

					if (!valid_move)
					{
						printf("INVALID MOVE.\n");
					}
				}
				else
				{
					printf("PIECE DOESNT EXISTS OR IS WRONG COLOR.\n");
				}
			}
			else
			{
				printf("COORDINATES OUT OF RANGE. %d, %d to %d, %d\n", start_pos_file, start_pos_rank, move_pos_file, move_pos_rank);
			}
		}

		valid_move = false;
		memset(move_input, 0, sizeof(move_input));
		current_turn = current_turn == WHITE ? BLACK : WHITE; // if the current turn is white, set it to black. if not, set it to white.
	}
}