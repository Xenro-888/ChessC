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

int get_sign(int number);

piece* create_piece(piece_type type, piece_color color, int file, int rank);

int is_move_valid(piece* piece_to_move, int file, int rank, piece* board[8][8]);
int set_piece_pos(piece* piece_to_move, int file, int rank, piece* board[8][8]);

int is_path_blocked(piece* board[8][8], piece* piece_to_move, int file_increment, int rank_increment, int path_length);
int is_position_in_board(int file, int rank);
int is_in_check(piece_color color, piece* white_pieces[16], piece* black_pieces[16], piece* board[8][8]);

void init_board(piece* board[8][8], piece* white_pieces[16], piece* black_pieces[16]);
void clear_board(piece* board[8][8]);
void display_board(piece* board[8][8]);
