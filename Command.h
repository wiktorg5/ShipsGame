#pragma once
#include "Player.h"
#include "Board.h"

//functions
void command_reaction(int* is_ai,char command[], struct Player* players, char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced, int* srand_seed, int* srand_set, char* AI, int* extended, int* current_state, char last_command[], int* number_of_A_command, int* number_of_B_command, int* reefNumber, reef** reefs);
