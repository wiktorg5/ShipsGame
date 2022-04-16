#pragma once
void set_AI(struct Player players[], int* x, int* y, char** board, char* AI, char* current_player, int* srand_seed, int* srand_set, char command[], int extended, int* reefNumber, reef reefs[]);
void set_srand(int* srand_seed, int* srand_set);
void random_placing(struct Player players[], int* x, int* y, char** board,int AI_player);
int check_placement(int instances, int placement_possible, int cl_number, struct Player players[], int AI_player);
int max_moves_AI(int pNumber, int cl_number, int shipNumber, struct Player players[]);
int check_move(int engine, int placement_possible, int moves_max, int cl_number, struct Player players[], int instances, int AI_player);
void random_move(struct Player players[], int* x, int* y, char** board, int AI_player, int extended);
void random_shoot(struct Player players[], int* x, int* y, char** board, int AI_player, int extended);
void get_AI(char* AI, int* is_ai);
void shoot_random_enemy(struct Player players[], int enemyNumber, int extended, char** board, int* break1);
void change_position_random(char side, char* direction, int* fX, int* fY, int size);
void random_shoot_not_enemy(struct Player players[], int extended, int AI_player, int* randomX, int* randomY, int* x, int* y);