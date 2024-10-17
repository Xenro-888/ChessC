#include <stdio.h>
#include "board.h"
#include "stdlib.h"

int get_sign(int number)
{
	if (number > 0)
		return 1;
	else if (number < 0)
		return -1;
	else
		return 0;
}

int set_piece_pos(piece* piece_to_move, int file, int rank, piece* board[8][8])
{
	if (board[rank][file] != NULL)
		free(board[rank][file]);

	board[piece_to_move->rank][piece_to_move->file] = NULL;
	board[rank][file] = piece_to_move;
	piece_to_move->file = file;
	piece_to_move->rank = rank;
	return true;
}

int is_path_blocked(piece* board[8][8], piece* piece_to_move, int file_increment, int rank_increment, int path_length)
{
	int check_space_file = piece_to_move->file + file_increment;
	int check_space_rank = piece_to_move->rank + rank_increment;
	piece* check_space = board[check_space_rank][check_space_file];

	for (int i = 0; i < path_length; i++)
	{
		if (check_space_file < 0 || check_space_file > 7 || check_space_rank < 0 || check_space_rank > 7)
			return false;

		if (check_space != NULL)
		{
			if (check_space->color == piece_to_move->color)
				return true;
			else if (i + 1 < path_length)
				return true;
		}

		check_space_file += file_increment;
		check_space_rank += rank_increment;
		check_space = board[check_space_rank][check_space_file];
	}
	
	return false;
}

int is_in_check(piece_color color, piece* white_pieces[16], piece* black_pieces[16], piece* board[8][8])
{
	const piece* king = NULL;
	piece** ally_pieces;
	piece** enemy_pieces;

	if (color == WHITE)
	{
		ally_pieces = white_pieces;
		enemy_pieces = black_pieces;
	}
	else
	{
		ally_pieces = black_pieces;
		enemy_pieces = white_pieces;
	}

	for (int i = 0; i < 16; i++)
	{
		if (ally_pieces[i]->type == KING)
		{
			king = ally_pieces[i];
			break;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		piece* current_piece = enemy_pieces[i];
	}

	return false;
}

// checks if a move is move moveset and not blocked
int is_move_valid_basic(piece* piece_to_move, int file, int rank, piece* board[8][8])
{
	if (piece_to_move == NULL || !is_position_in_board(file, rank))
		return false;

	piece* space_to_move = board[rank][file];
	int file_difference = file - piece_to_move->file;
	int rank_difference = rank - piece_to_move->rank;
	int abs_file_difference = abs(file_difference);
	int abs_rank_difference = abs(rank_difference);

	if (piece_to_move->type == PAWN)
	{
		if (
			(rank_difference < 1 || rank_difference > 2) ||
			(abs_file_difference > 0 && space_to_move == NULL) ||
			(abs_file_difference > 1) ||
			(rank_difference > 1 && (abs_file_difference > 0 || (piece_to_move->rank != 1 && piece_to_move->rank != 6))) ||
			(space_to_move != NULL && abs_file_difference == 0)
			)
		{
			return false;
		}
	}
	else if (piece_to_move->type == BISHOP)
	{
		int check_space_file = piece_to_move->file;
		int check_space_rank = piece_to_move->rank;
		piece* check_space = board[check_space_rank][check_space_file];

		if (abs_file_difference != abs_rank_difference)
			return false;

		if (
			is_path_blocked(
				board,
				piece_to_move,
				file_difference > 0 ? 1 : -1,
				rank_difference > 0 ? 1 : -1,
				max(abs_file_difference, abs_rank_difference))
			)
		{
			return false;
		}
	}
	else if (piece_to_move->type == KNIGHT)
	{
		if (
			(abs_file_difference < 1 || abs_file_difference > 2) ||
			(abs_rank_difference < 1 || abs_rank_difference > 2) ||
			(abs_file_difference == abs_rank_difference)
			)
		{
			return false;
		}
	}
	else if (piece_to_move->type == ROOK)
	{
		if (
			(abs_file_difference > 0 && abs_rank_difference != 0) ||
			(abs_rank_difference > 0 && abs_file_difference != 0)
			)
		{
			return false;
		}

		if (
			is_path_blocked(
				board,
				piece_to_move,
				get_sign(file_difference),
				get_sign(rank_difference),
				max(abs_file_difference, abs_rank_difference))
			)
		{
			return false;
		}
	}
	else if (piece_to_move->type == QUEEN)
	{
		if (
			(abs_file_difference != abs_rank_difference) &&
			(abs_file_difference > 0 && abs_rank_difference != 0) &&
			(abs_rank_difference > 0 && abs_file_difference != 0)
			)
		{
			return false;
		}

		if (
			is_path_blocked(
				board,
				piece_to_move,
				get_sign(file_difference),
				get_sign(rank_difference),
				max(abs_file_difference, abs_rank_difference))
			)
		{
			return false;
		}
	}
	else if (piece_to_move->type == KING)
	{
		if (abs_rank_difference > 1 || abs_file_difference > 1)
			return false;
	}

	if (space_to_move != NULL && space_to_move->color == piece_to_move->color)
		return false;

	return true;
}

int process_move(piece* piece_to_move, int file, int rank, piece* white_pieces[16], piece* black_pieces[16], piece* board[8][8])
{
	piece* covered_piece = board[rank][file];

	if (!is_move_valid_basic(piece_to_move, file, rank, board))
		return false;
	
	board[rank][file] = piece_to_move;
	board[piece_to_move->rank][piece_to_move->file] = NULL;

	if (is_in_check(piece_to_move->color, white_pieces, black_pieces, board))
		return false;

	board[piece_to_move->rank][piece_to_move->file] = piece_to_move;
	board[rank][file] = covered_piece;

	return true;
}

void display_board(piece* board[8][8], piece* piece_to_move, piece* white_pieces[16], piece* black_pieces[16])
{
	printf("\x1b[H\x1b[2J");
	printf("---/TROY'S CHESS GAME/---\n\n");
	for (int rank = 7; rank >= 0; rank--)
	{
		for (int file = 0; file < 8; file++)
		{
			int can_move = false;
			piece* current_piece = board[rank][file];

			if (piece_to_move != NULL)
				can_move = process_move(piece_to_move, file, rank, white_pieces, black_pieces, board);

			if (current_piece != NULL)
			{
				printf("\x1b[22m");
				if (current_piece->color == WHITE)
					printf("\x1b[37m");
				else if (current_piece->color == BLACK)
					printf("\x1b[32m");

				if (can_move)
					printf("\x1b[31m");

				if (current_piece->type == PAWN)
					printf("p");
				else if (current_piece->type == BISHOP)
					printf("b");
				else if (current_piece->type == KNIGHT)
					printf("k");
				else if (current_piece->type == ROOK)
					printf("r");
				else if (current_piece->type == QUEEN)
					printf("Q");
				else if (current_piece->type == KING)
					printf("K");
			}
			else
				if (!can_move)
					printf(" ");
				else
					printf("*");

			if (file < 7)
			{
				printf("\x1b[37;2m");
				printf(", ");
			}
			else
				printf("\x1b[37;2m  %c", (char)rank + 49);
		}

		printf("\n");
		if (rank == 0)
			printf("\n\x1b[2mA  B  C  D  E  F  G  H\n");
	}
	printf("\x1b[22m");
}

int game_end_state(piece* board[8][8], piece* white_pieces[16], piece* black_pieces[16])
{
	int colors_that_can_move = 0;
	for (int i = 0; i < 2; i++)
	{
		int can_move = false;
		piece_color curr_color = (piece_color)i;
		piece** curr_piece_array = curr_color == WHITE ? white_pieces : black_pieces; // if the currrent color is white, use the white piece array. if not, use the black piece array.

		for (int i = 0; i < 16; i++)
		{
			piece* curr_piece = curr_piece_array[i];
			if (curr_piece == NULL)
				continue;

			printf("FILE: %d, RANK: %d\n", curr_piece->file, curr_piece->rank);
			for (int rank = 0; rank < 8; rank++)
			{
				for (int file = 0; file < 8; file++)
				{
					if (process_move(curr_piece, file, rank, white_pieces, black_pieces, board))
					{
						colors_that_can_move++;
						goto done_process_moves;
					}
				}
			}
		}

	done_process_moves:
		if (is_in_check(WHITE, white_pieces, black_pieces, board) && !can_move)
		{
			piece_color winner = curr_color == WHITE ? BLACK : WHITE;
			printf("CHECKMATE!\n");
			if (winner = WHITE)
				return 1;
			else
				return 2;
		}
		else if (curr_color == BLACK && colors_that_can_move < 2)
		{
			printf("STALEMATE!\n");
			return 3;
		}
	}

	return 0;
}