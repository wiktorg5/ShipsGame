#pragma once
#include "Player.h"

void clear_ship(int shipNumber, char shipClass[], char direction, int pNumber, int cl_number, struct Player players[], char** board, char** board_advanced);
void move(char command[],struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced, int extended);



int max_moves(int pNumber, int cl_number, int shipNumber, struct Player players[], int* endGame);
void copy_segments(int ship_size, char direction, int shipX, int shipY,char seg[], char** board);
int radar_number(int pNumber, int fX, int fY, struct Player players[]);
void change_radar_position(int pNumber, int fX, int fY, int i, struct Player players[]);

//move forward
void move_f(char direction, int pNumber, int shipNumber, char shipClass[], int cl_number, char side, char command[], struct Player players[], int* endGame, int* x, int* y, char** board, char** board_advanced);
void move_forward_W(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_forward_E(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_forward_S(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_forward_N(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);

//move left
void move_l(char direction, int pNumber, int shipNumber, char shipClass[], int cl_number, char side, char command[], struct Player players[], int* endGame, int* x, int* y, char** board, char** board_advanced);
void move_left_W(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_left_E(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_left_S(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_left_N(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);

//move right
void move_r(char direction, int pNumber, int shipNumber, char shipClass[], int cl_number, char side, char command[], struct Player players[], int* endGame, int* x, int* y, char** board, char** board_advanced);
void move_right_W(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_right_E(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_right_S(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);
void move_right_N(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY);