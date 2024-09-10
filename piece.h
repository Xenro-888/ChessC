#pragma once

#define true 1
#define false 0

typedef enum {PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING} piece_type;
typedef enum {WHITE, BLACK} piece_color;

typedef struct
{
	int file;
	int rank;
	piece_type type;
	piece_color color;
} piece;

piece* create_piece(piece_type type, piece_color color, int file, int rank);
int is_path_blocked(piece* board[8][8], piece* piece_to_move, int file_increment, int rank_increment, int path_length);
int move_piece(piece* piece_to_move, int file, int rank, piece* board[8][8]);
int is_position_in_board(int file, int rank);

void init_board(piece* board[8][8]);
void clear_board(piece* board[8][8]);
void display_board(piece* board[8][8]);
