#include<iostream>
#include "Fleet.h"
#include "Board.h"
#include "Player.h"
#include "placement_conditions.h"
#include "Shooting.h"

using namespace std;
#define alive_sign '+'
#define dead_sign 'x'
#define radar_sign '@'
#define cannon_sign '!'
#define engine_sign '%'
//ship sizes
#define CAR_SIZE 5
#define BAT_SIZE 4
#define CRU_SIZE 3
#define DES_SIZE 2
//player signs
#define player_A_sign 'A'
#define player_B_sign 'B'
//directions
#define NORTH 'N'
#define SOUTH 'S'
#define WEST 'W'
#define EAST 'E'
//ships signs
#define CAR "CAR"
#define BAT "BAT"
#define CRU "CRU"
#define DES "DES"

#define NUMBER_OF_PLAYERS 2
#define NUMBER_OF_CLASSES 4
#define CLASS_NAME_LENGTH 4
#define MAXIMUM_SHIP_LENGTH 6

//allocating memory for choosen number of ships
void ship_allocating(struct Player players[],int p)
{
	for (int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
			players[p].ship[i].number = players[p].ships[i];
			players[p].ship[i].instances = (int*)calloc(players[p].ship[i].number, sizeof(int));	
			players[p].ship[i].moves = (int*)calloc(players[p].ship[i].number, sizeof(int));	
			players[p].ship[i].shoots = (int*)calloc(players[p].ship[i].number, sizeof(int));
			players[p].ship[i].positions = (position*)malloc(players[p].ship[i].number * sizeof(position));	
		
	}
}

//setting number of ships for choosen player
void set_the_fleet(struct Player players[])
{
	char p;
	cin >> p;
	int pl = choose_pNumber(p);

	for (int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		cin >> players[pl].ships[i];
	}
	ship_allocating(players,pl);
}
int choose_pNumber(char player)
{
	int pNumber;
	if (player == player_A_sign) //which player is currrent ( A-0, B-1 )
		pNumber = 0;
	else
		pNumber = 1;

	return pNumber;
}

//main function for placing ships
void place_the_ship(char command[], struct Player players[], char *current_player, int* endGame, int* x, int* y, char** board,char** board_advanced)
{
	int can_set = 1;

	int pNumber = choose_pNumber(*current_player);

	int shipY, shipX, shipNumber;
	char shipClass[CLASS_NAME_LENGTH];
	char shipDirection;
	
	cin >> shipY >> shipX >> shipDirection >> shipNumber >> shipClass;
	char playing = *current_player;

	if (is_placement_in_init_position(shipY, shipX,playing,shipDirection, choose_ship_length(shipClass),players) == 0)//is x and y in players initial position
	{
		cout << "INVALID OPERATION " <<'"' << command <<" " << shipY << " " << shipX << " " << shipDirection << " " << shipNumber << " " << shipClass << '"' << ": NOT IN STARTING POSITION" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(shipX, choose_ship_length(shipClass), shipY, shipDirection,x,y,board) == 0)//is placement inside the board
	{
		cout << "Placement outside of the board!";
		*endGame = 1;
	}
	else if (is_placement_possible(shipX, choose_ship_length(shipClass), shipY, shipDirection,x,y,board) == -1)//if placing on a reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipY << " " << shipX << " " << shipDirection << " " << shipNumber << " " << shipClass << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		char nothing[MAXIMUM_SHIP_LENGTH];
		choose_ship_of_class(shipClass, shipNumber, shipX, shipY, shipDirection,*current_player,command,players, endGame,&can_set);
		choose_ship_position(shipClass, shipDirection, shipX, shipY,board,board_advanced);
		set_ship_positon(pNumber, shipClass, shipX, shipY, shipDirection, shipNumber,players);
		other_ships(shipClass,shipDirection,pNumber, shipNumber,shipY,shipX,*current_player,'P',nothing,command,players,endGame,x,y,board);
		radar_to_add(shipNumber, choose_class_number(shipClass), choose_pNumber(*current_player),choose_ship_length(shipClass),players);
		int count_alive_parts = choose_ship_length(shipClass);

		players[pNumber].alive_fragments += count_alive_parts;
		players[pNumber].ship[choose_class_number(shipClass)].counter++;
	}

}

//calculating ship size for every class
int choose_ship_length(char shipClass[])
{
	if (strcmp(shipClass, CAR) == 0)
		return CAR_SIZE;
	else if (strcmp(shipClass, BAT) == 0)
		return BAT_SIZE;
	else if (strcmp(shipClass, CRU) == 0)
		return CRU_SIZE;
	else if (strcmp(shipClass, DES) == 0)
		return DES_SIZE;
	else
		return 0;
}

// what is the place in array for every class
int choose_class_number(char shipClass[])
{
	int i = 0;
	if (strcmp(shipClass, CAR) == 0)
		i = 0;
	else if (strcmp(shipClass, BAT) == 0)
		i = 1;
	else if (strcmp(shipClass, CRU) == 0)
		i = 2;
	else if (strcmp(shipClass, DES) == 0)
		i = 3;

	return i;
}

//choosing i-th ship of chosen class from array and marking it existance in instances array
void choose_ship_of_class(char shipClass[], int shipNumber, int shipX, int shipY, char shipDirection,char player,char command[], struct Player players[], int* endGame,int* can_set)
{
	int i = choose_class_number(shipClass);
	int pNumber = choose_pNumber(player);

	if (players[pNumber].ships[i] > shipNumber && shipNumber >= 0)
	{
		if (players[pNumber].ship[i].instances[shipNumber] == 0) //checking if ship has been already taken
			players[pNumber].ship[i].instances[shipNumber]++;
		else
		{
			cout << "INVALID OPERATION " << '"' << command << " " << shipY << " " << shipX << " " << shipDirection << " " << shipNumber << " " << shipClass << '"' << ": SHIP ALREADY PRESENT" << endl;
			*can_set = 0;
			*endGame = 1;
		}
	}
	else
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipY << " " << shipX << " " << shipDirection << " " << shipNumber << " " << shipClass << '"' << ": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
		*can_set = 0;
		*endGame = 1;
	}
}

//setting position for ship and saving it to positions array in player structure
void set_ship_positon(int pNumber,char shipClass[],int shipX, int shipY, char shipDirection, int shipNumber, struct Player players[])
{
	int i = choose_class_number(shipClass);
	int ship_size = choose_ship_length(shipClass);

	players[pNumber].ship[i].positions[shipNumber].frontX = shipX;
	players[pNumber].ship[i].positions[shipNumber].frontY = shipY;
	
	if (shipDirection == WEST)
	{
		players[pNumber].ship[i].positions[shipNumber].direction = WEST;
		players[pNumber].ship[i].positions[shipNumber].backX = shipX+ship_size-1;
		players[pNumber].ship[i].positions[shipNumber].backY = shipY;
		players[pNumber].ship[i].positions[shipNumber].cannonX = players[pNumber].ship[i].positions[shipNumber].frontX + 1;
		players[pNumber].ship[i].positions[shipNumber].cannonY = players[pNumber].ship[i].positions[shipNumber].frontY;
	}
	else if (shipDirection == EAST)
	{
		players[pNumber].ship[i].positions[shipNumber].direction = EAST;
		players[pNumber].ship[i].positions[shipNumber].backX = shipX-ship_size+1;
		players[pNumber].ship[i].positions[shipNumber].backY = shipY;
		players[pNumber].ship[i].positions[shipNumber].cannonX = players[pNumber].ship[i].positions[shipNumber].frontX -1;
		players[pNumber].ship[i].positions[shipNumber].cannonY = players[pNumber].ship[i].positions[shipNumber].frontY;
	}
	else if (shipDirection == SOUTH)
	{
		players[pNumber].ship[i].positions[shipNumber].direction = SOUTH;
		players[pNumber].ship[i].positions[shipNumber].backX = shipX;
		players[pNumber].ship[i].positions[shipNumber].backY = shipY-ship_size+1;
		players[pNumber].ship[i].positions[shipNumber].cannonX = players[pNumber].ship[i].positions[shipNumber].frontX;
		players[pNumber].ship[i].positions[shipNumber].cannonY = players[pNumber].ship[i].positions[shipNumber].frontY-1;
	}
	else if (shipDirection == NORTH)
	{
		players[pNumber].ship[i].positions[shipNumber].direction = NORTH;
		players[pNumber].ship[i].positions[shipNumber].backX = shipX;
		players[pNumber].ship[i].positions[shipNumber].backY = shipY+ship_size-1;
		players[pNumber].ship[i].positions[shipNumber].cannonX = players[pNumber].ship[i].positions[shipNumber].frontX;
		players[pNumber].ship[i].positions[shipNumber].cannonY = players[pNumber].ship[i].positions[shipNumber].frontY+1;
	}
	allocate_shoots(players);
}

//ship position for normal ship placement
void choose_ship_position(char shipClass[], char direction, int shipX, int shipY, char** board, char** board_advanced)
{
	int ship_size = choose_ship_length(shipClass);

	for (int i = 0; i < ship_size; i++)
	{
		if (direction == WEST)
		{
				board[shipY][shipX + i] = alive_sign;
				if (i == 0)
					board_advanced[shipY][shipX + i] = radar_sign;
				else if(i==1)
					board_advanced[shipY][shipX + i] = cannon_sign;

				if(i==ship_size-1)
					board_advanced[shipY][shipX + i] = engine_sign;

				if (i != 0 && i != 1 && i != ship_size - 1)
					board_advanced[shipY][shipX + i] = alive_sign;
		}
		else if (direction == EAST)
		{
				board[shipY][shipX - i] = alive_sign;
				if (i == 0)
					board_advanced[shipY][shipX - i] = radar_sign;
				else if (i == 1)
					board_advanced[shipY][shipX - i] = cannon_sign;

				if (i == ship_size - 1)
					board_advanced[shipY][shipX - i] = engine_sign;

				if (i != 0 && i != 1 && i != ship_size - 1)
					board_advanced[shipY][shipX - i] = alive_sign;
		}
		else if (direction == SOUTH)
		{
				board[shipY - i][shipX] = alive_sign;
				if (i == 0)
					board_advanced[shipY-i][shipX] = radar_sign;
				else if (i == 1)
					board_advanced[shipY - i][shipX] = cannon_sign;

				if (i == ship_size - 1)
					board_advanced[shipY - i][shipX] = engine_sign;

				if (i != 0 && i != 1 && i != ship_size - 1)
					board_advanced[shipY - i][shipX] = alive_sign;
		}
		else if (direction == NORTH)
		{
				board[shipY + i][shipX] = alive_sign;
				if (i == 0)
					board_advanced[shipY + i][shipX] = radar_sign;
				else if (i == 1)
					board_advanced[shipY + i][shipX] = cannon_sign;

				if (i == ship_size - 1)
					board_advanced[shipY + i][shipX] = engine_sign;

				if (i != 0 && i != 1 && i != ship_size - 1)
					board_advanced[shipY + i][shipX] = alive_sign;
		}
	}
	
}

//ship position for ship pre-allocating
void choose_ship_position_advanced(char shipClass[], char direction, int shipX, int shipY,char segments[], char** board, char** board_advanced)
{
	int ship_size = choose_ship_length(shipClass);

	for (int i = 0; i < ship_size; i++)
	{
		if (direction == WEST)
		{
				board[shipY][shipX + i] = segments[i];
				if (segments[i] == alive_sign)
				{
					if (i == 0)
						board_advanced[shipY][shipX + i] = radar_sign;
					else if (i == 1)
						board_advanced[shipY][shipX + i] = cannon_sign;

					if (i == ship_size - 1)
						board_advanced[shipY][shipX + i] = engine_sign;

					if (i != 0 && i != 1 && i != ship_size - 1)
						board_advanced[shipY][shipX + i] = alive_sign;
				}
				else
					board_advanced[shipY][shipX + i] = dead_sign;
		}
		else if (direction == EAST)
		{
				board[shipY][shipX - i] = segments[i];

				if (segments[i] == alive_sign)
				{
					if (i == 0)
						board_advanced[shipY][shipX - i] = radar_sign;
					else if (i == 1)
						board_advanced[shipY][shipX - i] = cannon_sign;

					if (i == ship_size - 1)
						board_advanced[shipY][shipX - i] = engine_sign;

					if (i != 0 && i != 1 && i != ship_size - 1)
						board_advanced[shipY][shipX - i] = alive_sign;
				}
				else
					board_advanced[shipY][shipX - i] = dead_sign;
		}
		else if (direction == SOUTH)
		{
				board[shipY - i][shipX] = segments[i];

				if (segments[i] == alive_sign)
				{
					if (i == 0)
						board_advanced[shipY - i][shipX] = radar_sign;
					else if (i == 1)
						board_advanced[shipY - i][shipX] = cannon_sign;

					if (i == ship_size - 1)
						board_advanced[shipY - i][shipX] = engine_sign;

					if (i != 0 && i != 1 && i != ship_size - 1)
						board_advanced[shipY - i][shipX] = alive_sign;
				}
				else
					board_advanced[shipY - i][shipX] = dead_sign;
		}
		else if (direction == NORTH)
		{
				board[shipY + i][shipX] = segments[i];

				if (segments[i] == alive_sign)
				{
					if (i == 0)
						board_advanced[shipY + i][shipX] = radar_sign;
					else if (i == 1)
						board_advanced[shipY + i][shipX] = cannon_sign;

					if (i == ship_size - 1)
						board_advanced[shipY + i][shipX] = engine_sign;

					if (i != 0 && i != 1 && i != ship_size - 1)
						board_advanced[shipY + i][shipX] = alive_sign;
				}
				else
					board_advanced[shipY + i][shipX] = dead_sign;
		}
	}
	
}

//function for ship pre-allocating
void pre_ship(char command[], struct Player players[], int* endGame, int* x, int* y, char** board, char** board_advanced)
{
	int can_set = 1;

	char p;
	int shipY, shipX, shipNumber;
	char shipClass[CLASS_NAME_LENGTH];
	char shipDirection;

	char segments[MAXIMUM_SHIP_LENGTH];
	cin >> p >> shipY >> shipX >> shipDirection >> shipNumber >> shipClass >> segments;
	int size = choose_ship_length(shipClass);
	int pNumber = choose_pNumber(p);

	char new_segments[MAXIMUM_SHIP_LENGTH];
	int count_alive_parts = 0;
	for (int i = 0; i < size; i++)

	{
		if (segments[i] == '1')
		{
			count_alive_parts++;
			new_segments[i] = alive_sign;
		}
		else if (segments[i] == '0')
			new_segments[i] = dead_sign;
	}

	if (is_placement_possible(shipX, size, shipY, shipDirection,x,y,board) == 0)//is placement inside the board
	{
		cout << "Placement outside of the board!";
		*endGame = 1;
	}
	else//if placement is possible
	{
		choose_ship_of_class(shipClass, shipNumber, shipX, shipY, shipDirection,p,command,players, endGame,&can_set);
		choose_ship_position_advanced(shipClass, shipDirection, shipX, shipY, new_segments,board,board_advanced);
		set_ship_positon(pNumber, shipClass, shipX, shipY, shipDirection, shipNumber,players);
		other_ships(shipClass, shipDirection, pNumber, shipNumber, shipY, shipX,p,'P',segments,command,players, endGame,x,y,board);

		//checking if we have to add radar to player's radars array
		if (new_segments[0] == alive_sign)
		{
			int player = choose_pNumber(p);
			radar_to_add(shipNumber, choose_class_number(shipClass), player , choose_ship_length(shipClass), players);
		}
			

		players[pNumber].alive_fragments += count_alive_parts;
		players[pNumber].ship[choose_class_number(shipClass)].counter++;
	}

	
}



