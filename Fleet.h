#pragma once


void set_the_fleet(struct Player players[]);
void place_the_ship(char command[], struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced);
int choose_ship_length(char shipClass[]);
void choose_ship_of_class(char shipClass[], int shipNumber, int shipX, int shipY, char shipDirection,char player, char command[], struct Player players[], int* endGame, int* can_set);
void choose_ship_position(char shipClass[], char direction, int shipX, int shipY, char** board, char** board_advanced);
void ship_allocating(struct Player players[], int p);
void pre_ship(char command[], struct Player players[], int* endGame, int* x, int* y, char** board, char** board_advanced);
int choose_pNumber(char player);
void set_ship_positon(int pNumber, char shipClass[], int shipX, int shipY, char shipDirection, int shipNumber, struct Player players[]);
int choose_class_number(char shipClass[]);
int choose_ship_length(char shipClass[]);

typedef struct
{
	int reefX;
	int reefY;
}reef;