#include<iostream>
#include "Player.h"
#include "Shooting.h"
#include "Fleet.h"

using namespace std;

//player signs
#define player_A_sign 'A'
#define player_B_sign 'B'
#define player_A_id "[playerA]"
#define player_B_id "[playerB]"

#define NUMBER_OF_PLAYERS 2
#define NUMBER_OF_CLASSES 4

//setting next expeced player
void next_player(int *number_of_A_command, int *number_of_B_command, char* current_player)
{
	char now_player;
	cin >> now_player;
	*current_player = now_player;

	if (*current_player == player_A_sign)
		*number_of_A_command = 0;
	else if (*current_player == player_B_sign)
		*number_of_B_command = 0;
}

//setting init position for given player
void set_the_init_position(struct Player players[])
{
	char p;
	int  y1, x1, y2, x2;
	cin >> p >> y1 >> x1 >> y2 >> x2;
	int pl = choose_pNumber(p);
	
		players[pl].initTopY = y1;
		players[pl].initTopX = x1;
		players[pl].initBotY = y2;
		players[pl].initBotX = x2;
}

//setting default init position for players
void default_init_position(struct Player players[])
{
	players[0].initTopY = 0;
	players[0].initTopX = 0;
	players[0].initBotY = 9;
	players[0].initBotX = 9;

	players[1].initTopY = 11;
	players[1].initTopX = 0;
	players[1].initBotY = 20;
	players[1].initBotX = 9;
}

//checking if good player chosen and changing current player
int good_player_choosing(int *number_of_A_command, int *number_of_B_command,char command[], char* current_player, int* endGame,struct Player players[])
{


	if (*number_of_A_command == 2 && strcmp(command, player_A_id) == 0)
	{
		cout <<"INVALID OPERATION "<<'"'<<command<<' ' << '"' << ": THE OTHER PLAYER EXPECTED" << endl;
		*endGame = 1;
		return 1;
	}
	if (*number_of_B_command == 2 && strcmp(command, player_B_id) == 0)
	{
		cout << "INVALID OPERATION "<< '"' << command <<' ' << '"' << ": THE OTHER PLAYER EXPECTED" << endl;
		*endGame = 1;
		return 1;
	}
		

	if (strcmp(command, player_A_id) == 0)
	{
		*number_of_B_command = 0;
		(*number_of_A_command)++;
		*current_player = player_A_sign;
		if (*number_of_A_command == 2)
		{
			*current_player = player_B_sign;
			next_round(players);
		}
			
		return 0;
	}
	if (strcmp(command, player_B_id) == 0)
	{
		*number_of_A_command = 0;
		*current_player = player_B_sign;
		(*number_of_B_command)++;
		if (*number_of_B_command == 2)
		{
			*current_player = player_A_sign;
			next_round(players);
		}
		return 0;
	}
		
	return 0;
}

//zeroing shoots and moves for next round
void next_round(struct Player players[])
{
	clear_moves_array(players);
	allocate_shoots(players);
}

//function for zeroing moves array for next round
void clear_moves_array(struct Player players[])
{
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		for (int k = 0; k < NUMBER_OF_CLASSES; k++)
		{
			for (int p = 0; p < players[i].ship[k].number; p++)
			{
				players[i].ship[k].moves[p] = 0;
			}

		}

	}
}