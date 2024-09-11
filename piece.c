#include "stdio.h"
#include "stdlib.h"
#include "piece.h"

int get_sign(int number)
{
	if (number > 0)
		return 1;
	else if (number == 0)
		return number;
	else
		return -1;
}

piece* create_piece(piece_type type, piece_color color, int file, int rank)
{
	if (is_position_in_board(file, rank))
	{
		piece* allocated_piece = malloc(sizeof(piece));
		allocated_piece->type = type;
		allocated_piece->color = color;
		allocated_piece->file = file;
		allocated_piece->rank = rank;
		
		return allocated_piece;
	}

	return NULL;
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

int move_piece(piece* piece_to_move, int file, int rank, piece* board[8][8])
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
			(rank_difference > 1 && (abs_file_difference > 0 || (piece_to_move->rank != 1 || piece_to_move->rank != 6)))
			)
			return false;
	}
	else if (piece_to_move->type == BISHOP)
	{
		if (
			(file_difference == 0 || rank_difference == 0) ||
			abs_file_difference/abs_rank_difference != 1
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
			{
				free(space_to_move);
				space_to_move = NULL;
			}
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
			abs_rank_difference != 0 && (abs(file_difference/rank_difference) > 0 && abs(file_difference / rank_difference) < 1 || abs(file_difference / rank_difference) > 1)
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

	if (space_to_move != NULL)
	{
		if (space_to_move->color != piece_to_move->color)
		{
			free(space_to_move);
			space_to_move == NULL;
		}
		else
			return false;
	}

	board[piece_to_move->rank][piece_to_move->file] = NULL;
	board[rank][file] = piece_to_move;
	piece_to_move->file = file;
	piece_to_move->rank = rank;

	return true;
}

int is_position_in_board(int file, int rank)
{
	if (
		file >= 0 && file <= 7 &&
		rank >= 0 && rank <= 7
		)
	{
		return true;
	}

	return false;
}

void init_board(piece* board[8][8])
{
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			piece_color possible_piece_color = rank <= 1 ? WHITE : BLACK; // if the rank is 1 or less, the possible color is WHITE. if not, it's black.

			// only create pieces if the rank is the edges of the board
			if (rank <= 1 || rank >= 6)
			{
				// create pawns
				if (rank == 1 || rank == 6)
				{
					board[rank][file] = create_piece(PAWN, possible_piece_color, file, rank);
				}
				// create rooks
				else if (file == 0 || file == 7)
				{
					board[rank][file] = create_piece(ROOK, possible_piece_color, file, rank);
				}
				// create knights
				else if (file == 1 || file == 6)
				{
					board[rank][file] = create_piece(KNIGHT, possible_piece_color, file, rank);
				}
				// create bishops
				else if (file == 2 || file == 5)
				{
					board[rank][file] = create_piece(BISHOP, possible_piece_color, file, rank);
				}
				// create queens
				else if (file == 3)
				{
					board[rank][file] = create_piece(QUEEN, possible_piece_color, file, rank);
				}
				// create kings
				else if (file == 4)
				{
					board[rank][file] = create_piece(KING, possible_piece_color, file, rank);
				}
			}
			// if the rank isn't the edge, make a empty space
			else
			{
				board[rank][file] = NULL;
			}
		}
	}
}

void clear_board(piece* board[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] != NULL)
			{
				free(board[i][j]);
			}
		}
	}
}

void display_board(piece* board[8][8])
{
	printf("\x1b[H\x1b[2J");
	printf("---/TROY'S CHESS GAME/---\n\n");
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{	
			piece* current_piece = board[i][j];

			if (current_piece != NULL) {
				printf("\x1b[22m");
				if (current_piece->color == WHITE)
				{
					printf("\x1b[37m");
				}
				else if (current_piece->color == BLACK)
				{
					printf("\x1b[31m");
				}

				switch (current_piece->type)
				{
				case PAWN:
					printf("p");
					break;

				case BISHOP:
					printf("b");
					break;

				case KNIGHT:
					printf("k");
					break;

				case ROOK:
					printf("r");
					break;

				case QUEEN:
					printf("Q");
					break;

				case KING:
					printf("K");
					break;
				}
			}
			else
			{
				printf(" ");
			}

			if (j < 7)
			{
				printf("\x1b[37;2m");
				printf(", ");
			}
			else
			{
				printf("\x1b[37;2m  %c", (char)i + 49);
			}
		}

		printf("\n");

		if (i == 0)
		{
			printf("\n\x1b[2mA  B  C  D  E  F  G  H\n");
		}
	}
}