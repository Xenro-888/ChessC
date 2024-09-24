#include <stdio.h>
#include "board.h"
#include "stdlib.h"

int get_sign(int number)
{
	if (number > 0)
		return 1;
	else if (number == 0)
		return number;
	else
		return -1;
}

int set_piece_pos(piece* piece_to_move, int file, int rank, piece* board[8][8])
{
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
		if (check_space != NULL && check_space->color == piece_to_move->color)
			return true;

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
			(rank_difference > 1 && (abs_file_difference > 0 || (piece_to_move->rank != 1 && piece_to_move->rank != 6)))
			)
		{
			return false;
		}
	}
	else if (piece_to_move->type == BISHOP)
	{
		if (
			(file_difference == 0 || rank_difference == 0) ||
			abs_file_difference / abs_rank_difference != 1
			)
			return false;

		int check_space_file = piece_to_move->file;
		int check_space_rank = piece_to_move->rank;
		piece* check_space = board[check_space_rank][check_space_file];

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

		if (space_to_move != NULL)
		{
			if (space_to_move->color != piece_to_move->color)
				space_to_move = NULL;
			else
				return false;
		}
	}
	else if (piece_to_move->type == ROOK)
	{
		if (
			(abs_file_difference > 0 && abs_rank_difference) != 0 ||
			(abs_rank_difference > 0 && abs_file_difference) != 0
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
			abs_rank_difference > 0 && abs_file_difference != 0 &&
			abs_rank_difference != 0 && (abs(file_difference / rank_difference) > 0 && abs(file_difference / rank_difference) < 1 || abs(file_difference / rank_difference) > 1)
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
}

int process_move(piece* piece_to_move, int file, int rank, piece* white_pieces, piece* black_pieces, piece* board[8][8])
{
	int previous_file = piece_to_move->file;
	int previous_rank = piece_to_move->rank;

	if (!is_move_valid_basic(piece_to_move, file, rank, board))
		return false;

	board[rank][file] = piece_to_move;
	board[piece_to_move->file][piece_to_move->rank] = NULL;
	if (is_in_check(piece_to_move->color, white_pieces, black_pieces, board))
		return false;

	return true;
}