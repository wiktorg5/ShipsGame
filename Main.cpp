#include<iostream>
#include "Board.h"
#include "Command.h"
#include "Fleet.h"
#include "Player.h"
#include "placement_conditions.h"

#define state_sign 0

using namespace std;


int main()
{
		char command[20];
		struct Player players[2];
		char current_player = 'A';
		int endGame = 0;
		int srand_seed;
		int srand_set;
		char AI; 

		int extended = 0;

		//player expected variables
		int number_of_A_command = 0;
		int number_of_B_command = 0;

		char last_command[20];
		int current_state = state_sign;//1 means player ,0 means state

		int x = 10, y = 21;

		char** board=(char**)malloc(y * sizeof(char*));;
		char** board_advanced= (char**)malloc(y * sizeof(char*));

		int reefNumber = 0;
		reef* reefs = (reef*)malloc(reefNumber * sizeof(reef));

		int is_ai = 0;

		//allocating memory at the beginning of the game
		alloc_radars(players);
		default_board(players, &x, &y, &board, &board_advanced);
		default_init_position(players);
		ship_allocating(players,0);
		ship_allocating(players, 1);
		
		while (endGame == 0 && cin>>command)
		{
			 if (strcmp(command, "BOARD_SIZE") == 0)
				{
					set_the_board(players, &x, &y, &board, &board_advanced);
				}

			command_reaction(&is_ai,command,players,&current_player,&endGame,&x,&y,board,board_advanced,&srand_seed,&srand_set,&AI, &extended, &current_state, last_command, &number_of_A_command, &number_of_B_command,&reefNumber,&reefs);

			if(all_ships_placed(players)==0 && strcmp(command,"SHOOT")==0)
				check_for_win(players,&endGame);

			
			memset(command, 0, 20);
			
		}
		


		clean_board(players, &y, board, board_advanced);

	return 0;
}