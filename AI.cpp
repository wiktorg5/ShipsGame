#include <iostream>
#include "Move.h"
#include "Fleet.h"
#include "Player.h"
#include "Board.h"
#include "placement_conditions.h"
#include "Command.h"
#include "Save.h"
#include "AI.h"

#define NORTH 'N'
#define SOUTH 'S'
#define WEST 'W'
#define EAST 'E'

#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'

#define alive_sign '+'
#define dead_sign 'x'

using namespace std;

#define NUMBER_OF_SIDES 3
#define NUMBER_OF_CLASSES 4
#define CLASS_NAME_LENGTH 4
#define NUMBER_OF_DIRECTIONS 4
#define MAX_COMMAND_LENGTH 20
#define state_id "[state]"

void set_srand(int* srand_seed,int* srand_set)
{
	*srand_set = 1;
	cin >> *srand_seed;
}
void get_AI(char* AI,int* is_ai)
{
	cin >> *AI;
	*is_ai = 1;
}
void set_AI(struct Player players[], int* x, int* y, char** board, char* AI, char* current_player, int* srand_seed, int* srand_set, char command[], int extended,int* reefNumber, reef reefs[])
{
	int AI_player;//player A or B is AI

	//printing which playeris AI
	char player_now[MAX_COMMAND_LENGTH];

	if (*AI == 'A')
	{
		strcpy_s(player_now, "[playerA]");
		AI_player = 0;
	}	
	else
	{
		strcpy_s(player_now, "[playerB]");
		AI_player = 1;
	}
		

	//saving current state
	save_state(players, current_player, x, y, board, srand_seed, srand_set,command,*AI,reefNumber,reefs,extended);

	//starting random generator with given seed
	srand(*srand_seed);

	//printing AI generated commands
	cout << state_id << endl << "PRINT " << 0 << endl << state_id << endl;
	cout << player_now << endl;
	random_placing(players,x, y, board,AI_player);
	random_move(players, x, y, board,AI_player,extended);

	if(all_ships_placed(players)==0)
		random_shoot(players, x, y, board, AI_player,extended);

	cout << player_now << endl;
	cout << state_id << endl << "PRINT " << 0 << endl << state_id << endl;
}
char number_to_direction(int direction)
{
	if (direction == 0)
		return NORTH;
	else if (direction == 1)
		return SOUTH;
	else if (direction == 2)
		return WEST;
	else
		return EAST;

}
int check_placement(int instances,int placement_possible,int cl_number, struct Player players[],int AI_player)
{
	if (instances == 0 && placement_possible == 1 && players[AI_player].ships[cl_number]!=0)
		return 1;
	else 
		return 0;
}
void random_placing(struct Player players[], int* x, int* y, char** board,int AI_player)
{
	int randomX = (rand() % (players[AI_player].initBotX - players[AI_player].initTopX - 1) + players[AI_player].initTopX);
	int randomY = (rand() % (players[AI_player].initBotY - players[AI_player].initTopY - 1) + players[AI_player].initTopY);

	
	
	int cl_number = (rand() % NUMBER_OF_CLASSES);;
	
	while (true)
	{
		if (players[AI_player].ship[cl_number].number != 0)
		{
			int shipNumber = (rand() % (players[AI_player].ship[cl_number].number));
			char shipClass[CLASS_NAME_LENGTH];
			int direction_number = (rand() % NUMBER_OF_DIRECTIONS);;
			char direction;
			int placement_possible;

			for (int t = 0; t < NUMBER_OF_CLASSES; t++)
			{
				int break1 = 0;
				for (int k = 0; k < players[AI_player].ship[cl_number].number; k++)
				{
					for (int j = 0; j < NUMBER_OF_DIRECTIONS; j++)
					{
						strcpy_s(shipClass, number_to_class(cl_number));

						int instances = players[AI_player].ship[cl_number].instances[shipNumber];

						//checking if this placement is possible
						direction = number_to_direction(direction_number);
						int ship_size = choose_ship_length((char*)shipClass);

						placement_possible = is_placement_possible(randomX, ship_size, randomY, direction, x, y, board);


						if (check_placement(instances, placement_possible, cl_number, players, AI_player) == 1)
						{
							cout << "PLACE_SHIP " << randomY << ' ' << randomX << ' ' << direction << ' ' << shipNumber << ' ' << shipClass << endl;
							break1 = 1;
							break;
						}

						if (direction_number == NUMBER_OF_DIRECTIONS-1)
							direction_number = 0;
						else
							direction_number++;

					}
					if (break1 == 1)
						break;

					if (shipNumber >= (players[AI_player].ship[cl_number].number) - 1)
						shipNumber = 0;
					else
						shipNumber++;
				}
				if (break1 == 1)
					break;
			}
			break;
		}
		else
		{
			if (cl_number == NUMBER_OF_CLASSES-1)
				cl_number = 0;
			else
				cl_number++;
		}
	}

}
char side_to_char(int sideNumber)
{
	if (sideNumber == 0)
		return FORWARD;
	else if (sideNumber == 1)
		return LEFT;
	else
		return RIGHT;
}
int check_move(int engine, int placement_possible, int moves_max,int cl_number,struct Player players[],int instances,int AI_player)
{
	if (engine==1 && placement_possible == 1 && moves_max == 1 && players[AI_player].ships[cl_number] != 0 && instances==1)
		return 1;
	else
		return 0;
}
void change_position_random(char side,char* direction,int* fX, int* fY,int size)
{
	if (side == FORWARD)
	{
		if (*direction == WEST)
		{
			--(*fX);
		}
		else if (*direction == EAST)
		{
			++(*fX);
		}
		else if (*direction == SOUTH)
		{
			++(*fY);
		}
		else if (*direction == NORTH)
		{
			--(*fY);
		}
	}
	if (side == LEFT)
	{
		if (*direction == WEST)
		{
			--(*fX);
			*direction = SOUTH;
			*fY += size - 1;
		}
		else if (*direction == EAST)
		{
			++(fX);
			*direction = NORTH;
			*fY -= size + 1;
		}
		else if (*direction == SOUTH)
		{
			++(*fY);
			*direction = EAST;
			*fX += size - 1;
		}
		else if (*direction == NORTH)
		{
			--(*fY);
			*direction = WEST;
			*fX -= size + 1;
		}
	}
	if (side == RIGHT)
	{
		if (*direction == WEST)
		{
			--(*fX);
			*direction = NORTH;
			*fY -= size + 1;
		}
		else if (*direction == EAST)
		{
			++fX;
			*direction = SOUTH;
			fY += size - 1;
		}
		else if (*direction == SOUTH)
		{
			++(*fY);
			*direction = WEST;
			*fX -= size + 1;
		}
		else if (*direction == NORTH)
		{
			--(*fY);
			*direction = EAST;
			*fX += size - 1;
		}
	}
}
void random_move(struct Player players[], int* x, int* y, char** board,int AI_player,int extended)
{
	
	int cl_number = rand() % NUMBER_OF_CLASSES;;
	

		//choosing random ship

		if (players[AI_player].ship[cl_number].number != 0)
		{
			int shipNumber = rand() % (players[AI_player].ship[cl_number].number);
			char direction;
			int placement_possible;
			char side;
			int side_int = rand() % NUMBER_OF_SIDES;

			for (int t = 0; t < NUMBER_OF_CLASSES; t++)
			{
				int break1 = 0;
				for (int k = 0; k < players[AI_player].ship[cl_number].number; k++)
				{
					for (int j = 0; j < NUMBER_OF_SIDES; j++)
					{
						int	instances = players[AI_player].ship[cl_number].instances[shipNumber];

						char shipClass[CLASS_NAME_LENGTH];
						strcpy_s(shipClass, number_to_class(cl_number));


						side = side_to_char(side_int);

						//checking move possibility
						int moves_max = max_moves_AI(AI_player, cl_number, shipNumber, players);
						int engine = 1;
						placement_possible = 1;

						int size = choose_ship_length(shipClass);
						direction = players[AI_player].ship[cl_number].positions[shipNumber].direction;

						//ship position after proposed move
						int fX = players[AI_player].ship[cl_number].positions[shipNumber].frontX;
						int fY = players[AI_player].ship[cl_number].positions[shipNumber].frontY;
						
						//what will be position after such a move
						change_position_random(side, &direction, &fX, &fY, size);

						//checking if this position change is feasible
						if (is_placement_possible(fX, size, fY, direction, x, y, board) == 0 || is_placement_possible(fX, size, fY, direction, x, y, board) == -1)
						{
							placement_possible = 0;
						}

						if (check_move(engine, placement_possible, moves_max, cl_number, players, instances, AI_player) == 1)
						{
							if (board[players[AI_player].ship[cl_number].positions[shipNumber].backY][players[AI_player].ship[cl_number].positions[shipNumber].backX] == 'x' && extended == 1)
							{
								engine = 0;
							}
							if (check_move(engine, placement_possible, moves_max, cl_number, players, instances, AI_player) == 1)
							{
								cout << "MOVE " << shipNumber << ' ' << shipClass << ' ' << side << endl;
								break1 = 1;
								break;
							}

						}
						if (side_int == NUMBER_OF_SIDES-1)
							side_int = 0;
						else
							side_int++;
					}
					if (break1 == 1)
						break;

					if (shipNumber >= (players[AI_player].ship[cl_number].number) - 1)
						shipNumber = 0;
					else
						shipNumber++;
				}
				if (break1 == 1)
					break;

				if (cl_number == NUMBER_OF_CLASSES-1)
					cl_number = 0;
				else
					cl_number++;
			}
				
		}
			
}
int max_moves_AI(int pNumber, int cl_number, int shipNumber, struct Player players[])
{
	if (cl_number != 0)
	{
		if (players[pNumber].ship[cl_number].moves[shipNumber] >= 3)
		{
			return 0;
		}
	}
	else
	{
		if (players[pNumber].ship[cl_number].moves[shipNumber] >= 2)//checking for carrier, because it has only 2 moves
		{
			return 0;
		}
	}
	return 1;
}
void shoot_random_enemy(struct Player players[],int enemyNumber,int extended,char** board, int* break1)
{
	for (int i = 0; i < NUMBER_OF_CLASSES; i++)
	{

		for (int k = 0; k < players[enemyNumber].ship[i].number; k++)
		{
			if (players[enemyNumber].ship[i].instances[k] == 1)
			{
				char shipClass[CLASS_NAME_LENGTH];
				strcpy_s(shipClass, number_to_class(i));
				int size = choose_ship_length(shipClass);
				int fY = players[enemyNumber].ship[i].positions[k].frontY;
				int fX = players[enemyNumber].ship[i].positions[k].frontX;

				if (extended == 0)
				{
					for (int z = 0; z < size; z++)
					{
						char direction = players[enemyNumber].ship[i].positions[k].direction;

						if (direction == WEST)
						{
							if (board[fY][fX + z] == alive_sign)
							{
								cout << "SHOOT " << fY << ' ' << fX + z << endl;
								*break1 = 1;
								break;
							}
						}
						else if (direction == EAST)
						{
							if (board[fY][fX - z] == alive_sign)
							{
								cout << "SHOOT " << fY << ' ' << fX - z << endl;
								*break1 = 1;
								break;
							}
						}
						else if (direction == SOUTH)
						{
							if (board[fY - z][fX] == alive_sign)
							{
								cout << "SHOOT " << fY - z << ' ' << fX << endl;
								*break1 = 1;
								break;
							}
						}
						else if (direction == NORTH)
						{
							if (board[fY + z][fX] == alive_sign)
							{
								cout << "SHOOT " << fY + z << ' ' << fX << endl;
								*break1 = 1;
								break;
							}
						}



					}
				}
				else
					*break1 = 1;

			}
			if (*break1 == 1)
				break;
		}
		if (*break1 == 1)
			break;
	}
}
void random_shoot_not_enemy(struct Player players[], int extended,int AI_player,int* randomX,int* randomY, int* x, int* y)
{
	while (true)
	{

		//checking if not shooting to ourselves
		if (extended == 0)
		{
			int no_shoot = 0;
			for (int i = 0; i < NUMBER_OF_CLASSES; i++)
			{

				for (int k = 0; k < players[AI_player].ship[i].number; k++)
				{
					if (players[AI_player].ship[i].instances[k] == 1)
					{
						char shipClass[CLASS_NAME_LENGTH];
						strcpy_s(shipClass, number_to_class(i));
						int size = choose_ship_length(shipClass);
						int fY = players[AI_player].ship[i].positions[k].frontY;
						int fX = players[AI_player].ship[i].positions[k].frontX;

						for (int z = 0; z < size; z++)
						{
							char direction = players[AI_player].ship[i].positions[k].direction;
							if (direction == WEST)
							{
								if ((fX + z) == *randomX && (fY) == *randomY)
								{
									no_shoot = 1;
								}
							}
							else if (direction == EAST)
							{
								if ((fX - z) == *randomX && (fY) == *randomY)
								{
									no_shoot = 1;
								}
							}
							else if (direction == SOUTH)
							{
								if ((fX) == *randomX && (fY - z) == *randomY)
								{
									no_shoot = 1;
								}
							}
							else if (direction == NORTH)
							{
								if ((fX) == *randomX && (fY + z) == *randomY)
								{
									no_shoot = 1;
								}
							}
						}
					}
				}
			}
			if (no_shoot == 0)
			{
				cout << "SHOOT " << randomY << ' ' << randomX << endl;
				break;
			}
		}
		else
			break;

		if (*randomX == (*x) - 1)
			*randomX = 0;
		else (*randomX)++;

		if (*randomY == (*y) - 1)
			*randomY = 0;
		else (*randomY)++;
	}
}
void random_shoot(struct Player players[], int* x, int* y, char** board, int AI_player,int extended)
{

		//selecting oponent's ships
		int enemyNumber;
		if (AI_player == 0)
			enemyNumber = 1;
		else
			enemyNumber = 0;


		int break1 = 0;

		//searching for alive parts of enemy ships
		shoot_random_enemy(players, enemyNumber, extended, board, &break1);

		if (break1 == 0)//if AI have not shooted already
		{
			int randomX = rand() % *x;
			int randomY = rand() % *y;
		
			random_shoot_not_enemy(players, extended, AI_player, &randomX, &randomY, x, y);
		}
	
}
