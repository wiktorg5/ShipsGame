#include<iostream>
#include "Command.h"
#include "Board.h"
#include "Fleet.h"
#include "Shooting.h"
#include "Player.h"
#include "Move.h"
#include "Save.h"
#include "AI.h"

#define state_sign 0
#define player_sign 1

#define player_A_id "[playerA]"
#define player_B_id "[playerB]"
#define state_id "[state]"

using namespace std;

void command_reaction(int* is_ai, char command[], struct Player* players, char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced, int* srand_seed, int* srand_set, char* AI, int* extended, int* current_state, char last_command[], int* number_of_A_command, int* number_of_B_command, int* reefNumber, reef** reefs) //choosing command
{
	
	if (strcmp(command, player_A_id) == 0 || strcmp(command, player_B_id) == 0)
	{
		*current_state = player_sign;
		good_player_choosing(number_of_A_command,number_of_B_command,command,current_player,endGame,players);
	}
	else if (strcmp(command, state_id) == 0)
	{
		*current_state = state_sign;
	}
	else if (strcmp(command,"PRINT")==0)
	{
		//if we print from players perspective or not
		if (*current_state == player_sign)
			print_command_player(players,current_player, x, y,board,board_advanced,*current_state);
		else
			print_command(players, x, y, board, board_advanced,*current_state);

	}
	else if (strcmp(command, "INIT_POSITION") == 0)
	{
		set_the_init_position(players);
	}
	
	else if (strcmp(command, "SET_FLEET") == 0)
	{
		set_the_fleet(players);
	}
	else if (strcmp(command, "PLACE_SHIP") == 0)
	{
		place_the_ship(command,players,current_player,endGame,x,y,board,board_advanced);
	}
	else if (strcmp(command, "SHOOT") == 0)
	{
		//is extended ships on
		if (*extended == 0)
			shoot(command, players, current_player,endGame,x,y,board,board_advanced);
		else
			shoot_extended(command,players,current_player,endGame,x,y,board,board_advanced);
	}
	else if (strcmp(command, "NEXT_PLAYER") == 0)
	{
		next_player(number_of_A_command, number_of_B_command,current_player);
	}
	else if (strcmp(command, "REEF") == 0)
	{
		place_reef(command,endGame, x, y,board,reefNumber,reefs);
	}
	else if (strcmp(command, "SHIP") == 0)
	{
		pre_ship(command, players, endGame,x,y,board,board_advanced);
	}
	else if (strcmp(command, "MOVE") == 0)
	{
		move(command, players,current_player, endGame,x,y,board,board_advanced,*extended);
	}
	else if (strcmp(command, "EXTENDED_SHIPS") == 0)
	{
		*extended = 1;
	}
	else if (strcmp(command, "SPY") == 0)
	{
		place_spy(command, players,current_player,endGame,board);
	}
	else if (strcmp(command, "SAVE") == 0)
	{
		save_state(players,current_player,x,y,board,srand_seed,srand_set,command,*AI,reefNumber,*reefs,*extended);
	}
	else if (strcmp(command, "SET_AI_PLAYER") == 0)
	{
		get_AI(AI,is_ai);	
	}
	else if (strcmp(command, "SRAND") == 0)
	{
		set_srand(srand_seed,srand_set);
	}

	if(*is_ai==1 && strcmp(command, state_id) == 0)
		set_AI(players, x, y, board, AI, current_player, srand_seed, srand_set, command, *extended, reefNumber, *reefs);

	strcpy_s(last_command,20,command);
	
}

