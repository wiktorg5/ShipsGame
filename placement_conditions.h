#pragma once

int placement_in_init_for_player(int shipY, int shipX, int pNumber,char direction,int ship_size,struct Player players[]);
int is_placement_possible(int shipX, int ship_size, int shipY, char direction, int* x, int* y, char** board);
int all_ships_placed(struct Player players[]);
int is_placement_in_init_position(int shipY, int shipX,char playing,char direction, int size, struct Player players[]);
int point(int pointY, int pointX, int* x, int* y, char** board);
int check_for_every_segment(int pointY, int pointX, int* x, int* y, char** board);
void check_close(int i, int pX, int pY, int size, int shipY, int shipX, char shipDirection, int shipNumber, char shipClass[],char player,char side,char segments[], char command[],int* endGame, int* x, int* y, char** board);
void other_ships(char shipClass[], char shipDirection, int pNumber, int shipNumber, int shipY, int shipX,char player,char side,char segments[], char command[],struct  Player players[], int* endGame, int* x, int* y, char** board);
void error_placing_close(int shipY, int shipX, char shipDirection, int shipNumber, char shipClass[], char player, char side,char segments[], char command[], int* endGame);