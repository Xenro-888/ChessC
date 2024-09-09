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

int move_piece(piece* piece_to_move, int file, int rank, piece* board[8][8])
{
	int can_move = false;

	// if the piece exists and the move position is in the board
	if (piece_to_move != NULL && is_position_in_board(file, rank))
	{
		if (piece_to_move->type == PAWN)
		{
			int piece_direction = piece_to_move->color == WHITE ? 1 : -1; // if the pawn's color is white, 1 is the forward direction. if it's not white, -1 if the forward direction.
			int pawn_moves[][2] = {
				{piece_to_move->file, piece_to_move->rank + piece_direction},
				{piece_to_move->file - 1, piece_to_move->rank + piece_direction},
				{piece_to_move->file + 1, piece_to_move->rank + piece_direction},
				{piece_to_move->file, piece_to_move->rank + piece_direction * 2} // double move
			};

			// check if one of the attempted moves and possible moves match up
			for (int i = 0; i < 4; i++)
			{
				int* possible_move = pawn_moves[i];

				// if the attempted move matches a possible move
				if (possible_move[0] == file && possible_move[1] == rank)
				{
					piece* next_square_piece = board[rank][file];

					// if attempting double move
					if (i == 3)
					{
						if (
							piece_to_move->rank == 1 || 
							piece_to_move->rank == 6 &&
							next_square_piece == NULL
							)
						{
							can_move = true;
						}
					}
					// if attempting capture
					else if (i == 1 || i == 2)
					{
						if (next_square_piece != NULL && next_square_piece->color != piece_to_move->color)
						{
							free(next_square_piece);
							can_move = true;
						}
					}
					// if attempting normal move
					else if (i == 0)
					{
						if (next_square_piece == NULL)
						{
							can_move = true;
						}
					}

					printf("%d\n", can_move);
				}
			}
		}
		else if (piece_to_move->type == BISHOP)
		{
			int move_slope = (piece_to_move->file - file) / (piece_to_move->rank - rank);
			piece* next_square_piece = board[rank][file];

			if (move_slope == 1 || move_slope == -1)
			{
				// if move is a capture
				if (next_square_piece != NULL && next_square_piece->color != piece_to_move->color)
				{
					free(next_square_piece);
					can_move = true;
				}
				// if move is a normal move
				else if (next_square_piece == NULL)
				{
					can_move = true;
				}
			}
		}
		else if (piece_to_move->type == KNIGHT)
		{
			int file_change = abs(piece_to_move->file - file);
			int rank_change = abs(piece_to_move->rank - rank);
			piece* next_square_piece = board[rank][file];

			// if move is in moveset
			if (
				(file_change == 2 && rank_change == 1) ||
				(file_change == 1 && rank_change == 2)
				)
			{
				// if move is a capture
				if (next_square_piece != NULL && next_square_piece->color != piece_to_move->color)
				{
					free(next_square_piece);
					can_move = true;
				}
				else if (next_square_piece == NULL)
				{
					can_move = true;
				}
			}
		}
		else if (piece_to_move->type == ROOK)
		{
			int file_change = abs(piece_to_move->file - file);
			int rank_change = abs(piece_to_move->rank - rank);

			if (
				(file_change > 0 && rank_change == 0) ||
				(rank_change > 0 && file_change == 0)
				)
			{
				
			}
		}
	}

	if (can_move)
	{
		board[piece_to_move->rank][piece_to_move->file] = NULL;
		piece_to_move->file = file;
		piece_to_move->rank = rank;
		board[rank][file] = piece_to_move;
		return true;
	}

	return false;
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