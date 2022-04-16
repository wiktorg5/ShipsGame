#include<iostream>
#include "Shooting.h"
#include "Board.h"
#include "Player.h"
#include "placement_conditions.h"
#include "Fleet.h"

#define alive_sign '+'
#define dead_sign 'x'
#define radar_sign '@'
#define cannon_sign '!'
#define engine_sign '%'
//player signs
#define player_A_sign 'A'
#define player_B_sign 'B'

#define NUMBER_OF_CLASSES 4
#define NUMBER_OF_PLAYERS 2

//ship sizes
#define CAR_SIZE 5
#define BAT_SIZE 4
#define CRU_SIZE 3
#define DES_SIZE 2

#define CLASS_NAME_LENGTH 4

using namespace std;

//giving appropriate amount of shoots for each ship
void allocate_shoots(struct Player players[])
{
	for(int k = 0;k< NUMBER_OF_PLAYERS;k++)
	{ 
		for (int i = 0; i < NUMBER_OF_CLASSES; i++)
		{
			for (int p = 0; p < players[k].ship[i].number; p++)
			{
				if (i == 0)
					players[k].ship[i].shoots[p] = CAR_SIZE;
				if (i == 1)
					players[k].ship[i].shoots[p] = BAT_SIZE;
				if (i == 2)
					players[k].ship[i].shoots[p] = CRU_SIZE;
				if (i == 3)
					players[k].ship[i].shoots[p] = DES_SIZE;
			}
		}
	}
	
}

//checking if shoot destroyed any radar, if so changing it's range to 1
void if_radar_destroyed(int shootX, int shootY, struct Player players[])
{
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		for (int p = 0; p < players[i].r; p++)
		{
			if (shootX == players[i].radars[p].radarX && shootY == players[i].radars[p].radarY)
			{
				players[i].radars[p].range = 1;
			}
		}
	}
}
void basic_shoot(int shootY, int shootX, char command[], struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced)
{
	if (all_ships_placed(players) == 0)
	{
		if (shootX < *x && shootX >= 0 && shootY < *y && shootY >= 0)
		{
			if (point_alive(shootY, shootX,board)==1)
			{
				if (*current_player == player_A_sign)
					players[1].alive_fragments--;
				else if (*current_player == player_B_sign)
					players[0].alive_fragments--;

				board[shootY][shootX] = dead_sign;
				board_advanced[shootY][shootX] = dead_sign;

				if_radar_destroyed(shootX, shootY,players);
			}
		}
		else
		{
			cout << "INVALID OPERATION " << '"' << command << " " << shootY << " " << shootX << '"' << ": FIELD DOES NOT EXIST" << endl;
			*endGame = 1;
		}
	}
	else
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shootY << " " << shootX << '"' << ": NOT ALL SHIPS PLACED" << endl;
		*endGame = 1;
	}
}

//shoot for basic version
void shoot(char command[], struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced)
{
	int shootY, shootX;
	cin >> shootY >> shootX;

	basic_shoot(shootY, shootX,command,players,current_player,endGame,x,y,board,board_advanced);
}

//calculating distance between 2 points
double distance(double y1, double x1, double y2, double x2)
{
	double dist = sqrt((abs(y2 - y1)) * (abs(y2- y1)) + (abs(x2 - x1)) * (abs(x2 - x1)));
	return dist;
}

//checking if given point is alive part of ship
int point_alive(int pointY, int pointX,char** board)
{
	if(board[pointY][pointX] == alive_sign)
		return 1;

	return 0;
}

//shoot version for extended ships
void shoot_extended(char command[], struct Player players[], char* current_player, int* endGame, int* x, int* y, char** board, char** board_advanced)
{
	int shipNumber,shootX,shootY;
	char shipClass[CLASS_NAME_LENGTH];

	cin >> shipNumber >> shipClass >> shootY >> shootX;

	int pNumber = choose_pNumber(*current_player);
	int cl_number = choose_class_number(shipClass);
	double cannonX =(double) players[pNumber].ship[cl_number].positions[shipNumber].cannonX;
	double cannonY = (double)players[pNumber].ship[cl_number].positions[shipNumber].cannonY;
	int size = choose_ship_length(shipClass);
	
	if (max_shoots(pNumber, cl_number, shipNumber,players,endGame) == 0)
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << shootY << " " << shootX << '"' << ": TOO MANY SHOOTS" << endl;
		*endGame = 1;
	}
	else
	{
		if (point_alive(cannonY, cannonX,board) == 1)
		{
			if (distance(cannonY, cannonX, (double)shootY, (double)shootX) <= (double)size)
			{
				basic_shoot(shootY, shootX,command,players, current_player,endGame,x,y,board,board_advanced);
				players[pNumber].ship[cl_number].shoots[shipNumber]--;
			}
			else
			{
				if (cl_number == 0)
				{
					basic_shoot(shootY, shootX,command,players,current_player,endGame,x,y,board,board_advanced);
					players[pNumber].ship[cl_number].shoots[shipNumber]--;
				}
				else
				{
					cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << shootY << " " << shootX << '"' << ": SHOOTING TOO FAR" << endl;
					*endGame = 1;
				}
			}


		}
		else
		{
			cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << shootY << " " << shootX << '"' << ": SHIP CANNOT SHOOT" << endl;
			*endGame = 1;
		}
	}
	
		
}

//checking if ship shooted maximum number of shoots
int max_shoots(int pNumber,int cl_number,int shipNumber, struct Player players[], int* endGame)
{
	if (players[pNumber].ship[cl_number].shoots[shipNumber] > 0)
		return 1;
	else
	{
		*endGame = 1;
		return 0;
	}
		
}

