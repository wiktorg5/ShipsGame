#pragma once
#include "Player.h"

void shoot(char command[], struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced);
void shoot_extended(char command[], struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced);
void basic_shoot(int shootY, int shootX, char command[], struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced);
int point_alive(int pointY, int pointX, char** board);
double distance(double y1, double x1, double y2, double x2);
void allocate_shoots(struct Player players[]);
int max_shoots(int pNumber, int cl_number, int shipNumber, struct Player players[], int* endGame);
void if_radar_destroyed(int shootX, int shootY, struct Player players[]);
