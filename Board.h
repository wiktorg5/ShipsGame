#pragma once
#include "Fleet.h"


//functions
void alloc_radars(struct Player players[]);
void default_board(struct Player players[], int* x, int* y, char*** board, char*** board_advanced);
void get_board_size(int* x, int* y);
void set_the_board(struct Player players[], int* x, int* y, char*** board, char*** board_advanced);
void print_the_board_simple(struct Player players[], int x, int y, char** board, int current_state);
void clean_board(struct Player players[], int* y, char** board, char** board_advanced);
void check_for_win(struct Player players[], int* endGame);
void parts_remaining(struct Player players[]);
void place_reef(char command[], int* endGame, int* x, int* y, char** board, int* reefNumber, reef** reefs);
void print_command(struct Player players[], int* x, int* y, char** board, char** board_advanced, int current_state);
void print_the_board_advanced(struct Player players[], int* x, int* y, char** board_advanced, int current_state);

void blank_board(int x, int y, char** board, char** board_advanced, struct Player players[]);

void print_command_player(struct Player players[], char* current_player, int* x, int* y, char** board, char** board_advanced, int current_state);
void radar_to_add(int shipNumber, int i, int pNumber,double size, struct Player players[]);
void check_visibility(int pNumber, int advanced, struct Player players[], int* x, int* y, char** board, char** board_advanced);
void players_ships_visibility(int pNumber, int advanced, struct Player players[], char** board, char** board_advanced);
void place_spy(char command[], struct Player players[], char* current_player, int* endGame, char** board);
void reefs_visibility(int pNumber, struct Player players[], int* x, int* y, char** board);
void save_reefs(int* reefNumber, reef reefs[]);

