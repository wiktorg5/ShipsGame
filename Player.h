#pragma once


void next_player(int* number_of_A_command, int* number_of_B_command, char* current_player);
void set_the_init_position(struct Player players[]);
void default_init_position(struct Player players[]);
int good_player_choosing(int* number_of_A_command, int* number_of_B_command,char command[], char* current_player, int* endGame, struct Player players[]);
void clear_moves_array(struct Player players[]);
void next_round(struct Player players[]);

typedef struct //position of a single ship
{
	int backX, backY, frontX, frontY;
	char direction;
	int cannonX, cannonY;
	int engineX, engineY;

}position;
typedef struct//radar coordinates
{
	int radarX, radarY;
	double range;
}radar;
struct ships_alloc
{
	int number = 0;
	int counter = 0;
	int* instances = (int*)calloc(number, sizeof(int));//0 means ship not present, 1 means ship present	
	int* moves = (int*)calloc(number, sizeof(int));//0 have not moved, 1 moved
	int* shoots = (int*)calloc(number, sizeof(int));
	position* positions = (position*)malloc(number * sizeof(position));//positions of every ship of this class
};
struct Player
{
	int ships[4] = { 1,2,3,4 };//number of carrier,battleship,cruiser,destroyer
	int r = 0;
	char** boardPlayer;
	radar* radars = (radar*)malloc(r * sizeof(radar));//list of all player's radars

	struct ships_alloc ship[4];//every spot for class of ship (CAR,BAT,CRU,DES)

	int alive_fragments = 0;
	int initTopX, initTopY, initBotY, initBotX;//player's initial position
};







