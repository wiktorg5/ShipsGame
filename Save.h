#pragma once

void save_state(struct Player players[], char* current_player, int* x, int* y, char** board, int* srand_seed, int* srand_set, char command[], char AI, int* reefNumber, reef reefs[], int extended);
const char* number_to_class(int num);
void save_ships(int i, int p, char player, struct Player players[], char** board);
char player_from_number(int i);