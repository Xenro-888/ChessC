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
	piece_color curr_turn = WHITE;
	init_board(board, white_pieces, black_pieces);

	int game_over = false;

	while (!game_over)
	{
		int is_in_check = false;

		display_board(board, NULL, white_pieces, black_pieces);
		printf("\n\x1b[37;22m\n");

		char player_input[20];
		piece* piece_to_move = NULL;
		int available_spots = 0;

		while (piece_to_move == NULL || available_spots < 1)
		{
			printf("ENTER THE PIECE'S POSITION.\n");
			fgets(player_input, sizeof(player_input), stdin);
			int piece_file = get_file_index(player_input[0]);
			int piece_rank = get_rank_index(player_input[1]);
			memset(player_input, 0, sizeof(player_input));

			if (!is_position_in_board(piece_file, piece_rank))
			{
				printf("INVALID COORDINATES.\n");
				continue;
			}

			piece_to_move = board[piece_rank][piece_file];

			// check if piece can be moved
			for (int rank = 0; rank < 8; rank++)
			{
				for (int file = 0; file < 8; file++)
				{
					piece* move_spot = board[rank][file];
					if (process_move(piece_to_move, file, rank, white_pieces, black_pieces, board))
						available_spots++;
				}
			}
			if (available_spots < 1)
				printf("CANNOT MOVE THIS PIECE.\n");
		}
		display_board(board, piece_to_move, white_pieces, black_pieces);

		int valid_move = false;
		int move_file = 0;
		int move_rank = 0;

		while (!valid_move)
		{
			printf("ENTER MOVE COORDINATES.\n");
			fgets(player_input, sizeof(player_input), stdin);
			move_file = get_file_index(player_input[0]);
			move_rank = get_rank_index(player_input[1]);
			memset(player_input, 0, sizeof(player_input));

			if (!is_position_in_board(move_file, move_rank))
				continue;

			valid_move = process_move(piece_to_move, move_file, move_rank, white_pieces, black_pieces, board);
			if (!valid_move)
				printf("INVALID MOVE.\n");
		}
		set_piece_pos(piece_to_move, move_file, move_rank, board);
		if (
			(piece_to_move->type == PAWN) && 
			(piece_to_move->color == WHITE && piece_to_move->rank == 7) ||
			(piece_to_move->color == BLACK && piece_to_move->rank == 0)
			)
		{
			piece_type selected_type = 6; // 6 is undefined

			display_board(board, NULL, white_pieces, black_pieces);
			while (selected_type == 6 || strlen(player_input) != 2)
			{
				printf("PLAYER INPUT LENGTH: %d\n", strlen(player_input));
				printf("FIRST CHAR: %c\n", player_input[0]);
				printf("CONGRATS, YOU CAN NOW PROMOTE YOUR PAWN.\nWHAT KIND OF PIECE DO YOU WANT?\nQUEEN: q, ROOK: r, BISHOP: b, KNIGHT: k\n");
				fgets(player_input, sizeof(player_input), stdin);

				if (player_input[0] == 'q')
					selected_type = QUEEN;
				else if (player_input[0] == 'r')
					selected_type = ROOK;
				else if (player_input[0] == 'b')
					selected_type = BISHOP;
				else if (player_input[0] == 'k')
					selected_type == KNIGHT;
			}
			printf("SELECTED TYPE: %d\n", selected_type);
			piece_to_move->type = selected_type;
		}

		curr_turn = curr_turn == WHITE ? BLACK : WHITE;
	}
}