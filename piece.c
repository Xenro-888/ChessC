#include "stdio.h"
#include "stdlib.h"
#include "piece.h"


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

void init_board(piece* board[8][8], piece* white_pieces[16], piece* black_pieces[16])
{
	int white_piece_index = 0;
	int black_piece_index = 0;

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			piece_color possible_piece_color = rank <= 1 ? WHITE : BLACK; // if the rank is 1 or less, the possible color is WHITE. if not, it's black.
			piece* new_piece = create_piece(PAWN, possible_piece_color, file, rank);

			// only create pieces if the rank is the edges of the board
			if (rank <= 1 || rank >= 6)
			{
				// create pawns
				if (rank == 1 || rank == 6)
					new_piece->type = PAWN;
				// create rooks
				else if (file == 0 || file == 7)
					new_piece->type = ROOK;
				// create knights
				else if (file == 1 || file == 6)
					new_piece->type = KNIGHT;
				// create bishops
				else if (file == 2 || file == 5)
					new_piece->type = BISHOP;
				// create queens
				else if (file == 3)
					new_piece->type = QUEEN;
				// create kings
				else if (file == 4)
					new_piece->type = KING;

				board[rank][file] = new_piece;

				if (new_piece->color == WHITE)
				{
					white_pieces[white_piece_index] = new_piece;
					white_piece_index++;
				}
				else
				{
					black_pieces[black_piece_index] = new_piece;
					black_piece_index++;
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