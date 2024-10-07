#include "piece.h"

int get_sign(int number);
int set_piece_pos(piece* piece_to_move, int file, int rank, piece* board[8][8]);
int is_path_blocked(piece* board[8][8], piece* piece_to_move, int file_increment, int rank_increment, int path_length);
int is_in_check(piece_color color, piece* white_pieces[16], piece* black_pieces[16], piece* board[8][8]);
int is_move_valid_basic(piece* piece_to_move, int file, int rank, piece* board[8][8]);
int process_move(piece* piece_to_move, int file, int rank, piece* white_pieces[16], piece* black_pieces[16], piece* board[8][8]);
void display_board(piece* board[8][8], piece* piece_to_move, piece* white_pieces[16], piece* black_pieces[16]);
