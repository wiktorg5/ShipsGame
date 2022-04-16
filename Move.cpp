#include<iostream>
#include "Move.h"
#include "Fleet.h"
#include "Player.h"
#include "Board.h"
#include "placement_conditions.h"
#include "Command.h"

using namespace std;
#define alive_sign '+'
#define dead_sign 'x'
#define radar_sign '@'
#define cannon_sign '!'
#define engine_sign '%'
//directions
#define NORTH 'N'
#define SOUTH 'S'
#define WEST 'W'
#define EAST 'E'
//sides
#define FORWARD 'F'
#define LEFT 'L'
#define RIGHT 'R'

#define CLASS_NAME_LENGTH 4


//main moving function
void move(char command[], struct Player players[], char* current_player, int* endGame,int* x,int* y,char** board,char** board_advanced, int extended)//main moving function
{
	int shipNumber;
	char shipClass[CLASS_NAME_LENGTH];
	char side;
	cin >>shipNumber>>shipClass>> side;

	int pNumber = choose_pNumber(*current_player);
	int ship_class_number = choose_class_number(shipClass);
	char direction = players[pNumber].ship[ship_class_number].positions[shipNumber].direction;

	int fY = players[pNumber].ship[ship_class_number].positions[shipNumber].frontY;
	int fX = players[pNumber].ship[ship_class_number].positions[shipNumber].frontX;
	int ship_radar_number = radar_number(pNumber, fX, fY,players);

	if (max_moves(pNumber, ship_class_number, shipNumber,players, endGame) == 1)//checking if ship have already moved maximum times
	{
		if (board[players[pNumber].ship[ship_class_number].positions[shipNumber].backY][players[pNumber].ship[ship_class_number].positions[shipNumber].backX] == dead_sign && extended == 1)//checking if engine is destroyed
		{
			cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP CANNOT MOVE" << endl;
			*endGame = 1;
		}
		else
		{
			if (side == FORWARD)
			{
				move_f(direction, pNumber, shipNumber, shipClass, ship_class_number, side,command,players, endGame,x,y,board, board_advanced);
			}
			else if (side == RIGHT)
			{
				move_r(direction, pNumber, shipNumber, shipClass, ship_class_number, side, command,players, endGame,x,y,board, board_advanced);
			}
			else if (side == LEFT)
			{
				move_l(direction, pNumber, shipNumber, shipClass, ship_class_number, side, command,players, endGame,x,y, board, board_advanced);
			}
			 fY = players[pNumber].ship[ship_class_number].positions[shipNumber].frontY;
			 fX = players[pNumber].ship[ship_class_number].positions[shipNumber].frontX;

			 change_radar_position(pNumber, fX, fY, ship_radar_number,players);//change radar position to new front of a ship

			++(players[pNumber].ship[ship_class_number].moves[shipNumber]);//marking that the ship has moved
			direction = players[pNumber].ship[ship_class_number].positions[shipNumber].direction;//changing ship direction, because it moved

			char nothing[6];
			other_ships(shipClass, direction, pNumber, shipNumber, fY, fX, *current_player, side, nothing,command, players, endGame,x,y,board);
		}
			
	}
	else
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP MOVED ALREADY" << endl;
		*endGame = 1;
	}		
}

//find ship radar that has to change position
int radar_number(int pNumber,int fX, int fY, struct Player players[])
{
	int n=1;
	for (int i = 0; i < players[pNumber].r; i++)
	{
		if (players[pNumber].radars[i].radarX == fX && players[pNumber].radars[i].radarY == fY)
			n = i;
	}

	return n;
}

//chaning moved ship radar position in radars array
void change_radar_position(int pNumber, int fX, int fY,int i, struct Player players[])
{
	players[pNumber].radars[i].radarX = fX;
	players[pNumber].radars[i].radarY = fY;
}

//checking if ship already moved maximum number of times
int max_moves(int pNumber, int cl_number, int shipNumber, struct Player players[], int* endGame)
{
	if (cl_number != 0)
	{
		if (players[pNumber].ship[cl_number].moves[shipNumber] >= 3)
		{
			*endGame = 1;
			return 0;
		}		
	}
	else
	{
		if (players[pNumber].ship[cl_number].moves[shipNumber] >= 2)//checking for carrier, because it has only 2 moves
		{
			*endGame = 1;
			return 0;
		}
	}
	return 1;
}

//clearing ship from it's previous position
void clear_ship(int shipNumber, char shipClass[], char direction, int pNumber, int cl_number, struct Player players[],char** board, char** board_advanced)
{
	int size = choose_ship_length(shipClass);

	int fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	int fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	for (int i = 0; i < size; i++)
	{
		if (direction == WEST)
		{
				board[fY][fX + i] = ' ';
				board_advanced[fY][fX + i] = ' ';
		}
		else if (direction == EAST)
		{
				board[fY][fX - i] = ' ';
				board_advanced[fY][fX - i] = ' ';
		}
		else if (direction == SOUTH)
		{
				board[fY - i][fX] = ' ';
				board_advanced[fY - i][fX] = ' ';
		}
		else if (direction == NORTH)
		{
				board[fY + i][fX] = ' ';
				board_advanced[fY + i][fX] = ' ';
		}
	}
	
}
void move_forward_W(char segments[],char** board_advanced,char command[],int shipNumber,int cl_number,struct Player players[],int pNumber,int size,char shipClass[], int* x, int* y, char** board,char side,int*endGame,int* fX, int *fY)
{
	--players[pNumber].ship[cl_number].positions[shipNumber].backX;
	--players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY][*fX + i] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY][*fX + i] = radar_sign;
				else if (i == 1)
					board_advanced[*fY][*fX + i] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY][*fX + i] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY][*fX + i] = alive_sign;
			}
			else
				board_advanced[*fY][*fX + i] = dead_sign;
		}
	}
}
void move_forward_E(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	++players[pNumber].ship[cl_number].positions[shipNumber].backX;
	++players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY][*fX - i] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY][*fX - i] = radar_sign;
				else if (i == 1)
					board_advanced[*fY][*fX - i] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY][*fX - i] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY][*fX - i] = alive_sign;
			}
			else
				board_advanced[*fY][*fX - i] = dead_sign;
		}
	}
}
void move_forward_S(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	++players[pNumber].ship[cl_number].positions[shipNumber].backY;
	++players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY - i][*fX] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY - i][*fX] = radar_sign;
				else if (i == 1)
					board_advanced[*fY - i][*fX] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY - i][*fX] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY - i][*fX] = alive_sign;
			}
			else
				board_advanced[*fY - i][*fX] = dead_sign;
		}
	}
}
void move_forward_N(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	--players[pNumber].ship[cl_number].positions[shipNumber].backY;
	--players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY + i][*fX] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY + i][*fX] = radar_sign;
				else if (i == 1)
					board_advanced[*fY + i][*fX] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY + i][*fX] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY + i][*fX] = alive_sign;
			}
			else
				board_advanced[*fY + i][*fX] = dead_sign;
		}
	}
}
void move_f(char direction,int pNumber,int shipNumber,char shipClass[],int cl_number,char side,char command[], struct Player players[], int* endGame,int*x,int*y,char** board, char** board_advanced)
{
	
	int size = choose_ship_length(shipClass);
	char segments[6];
	int fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	int fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	copy_segments(size, direction, fX, fY, segments,board);

	clear_ship(shipNumber, shipClass, players[pNumber].ship[cl_number].positions[shipNumber].direction, pNumber, cl_number,players,board, board_advanced);

	if (direction == WEST)
	{
		move_forward_W(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
					
	}
	else if (direction == EAST)
	{
		move_forward_E(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	else if (direction == SOUTH)
	{
		move_forward_S(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	else if (direction == NORTH)
	{
		move_forward_N(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	
	set_ship_positon(pNumber, shipClass, fX, fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, shipNumber, players);
	
}
void move_left_W(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = SOUTH;
	players[pNumber].ship[cl_number].positions[shipNumber].backX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	players[pNumber].ship[cl_number].positions[shipNumber].frontY = players[pNumber].ship[cl_number].positions[shipNumber].frontY + size - 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY - i][*fX] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY - i][*fX] = radar_sign;
				else if (i == 1)
					board_advanced[*fY - i][*fX] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY - i][*fX] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY - i][*fX] = alive_sign;
			}
			else
				board_advanced[*fY - i][*fX] = dead_sign;

		}
	}
}
void move_left_E(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = NORTH;
	players[pNumber].ship[cl_number].positions[shipNumber].backX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	players[pNumber].ship[cl_number].positions[shipNumber].frontY = players[pNumber].ship[cl_number].positions[shipNumber].frontY - size + 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY + i][*fX] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY + i][*fX] = radar_sign;
				else if (i == 1)
					board_advanced[*fY + i][*fX] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY + i][*fX] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY + i][*fX] = alive_sign;
			}
			else
				board_advanced[*fY + i][*fX] = dead_sign;
		}
	}
}
void move_left_S(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = EAST;

	players[pNumber].ship[cl_number].positions[shipNumber].backY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	players[pNumber].ship[cl_number].positions[shipNumber].frontX = players[pNumber].ship[cl_number].positions[shipNumber].frontX + size - 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY][*fX - i] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY][*fX - i] = radar_sign;
				else if (i == 1)
					board_advanced[*fY][*fX - i] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY][*fX - i] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY][*fX - i] = alive_sign;
			}
			else
				board_advanced[*fY][*fX - i] = dead_sign;
		}
	}
}
void move_left_N(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = WEST;

	players[pNumber].ship[cl_number].positions[shipNumber].backY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	players[pNumber].ship[cl_number].positions[shipNumber].frontX = players[pNumber].ship[cl_number].positions[shipNumber].frontX - size + 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY][*fX + i] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY][*fX + i] = radar_sign;
				else if (i == 1)
					board_advanced[*fY][*fX + i] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY][*fX + i] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY][*fX + i] = alive_sign;
			}
			else
				board_advanced[*fY][*fX + i] = dead_sign;
		}
	}
}
void move_l(char direction, int pNumber, int shipNumber, char shipClass[], int cl_number,char side, char command[], struct Player players[], int* endGame, int* x, int* y, char** board, char** board_advanced)
{
	
	move_f(direction, pNumber, shipNumber, shipClass,cl_number,side,command,players, endGame,x,y,board, board_advanced);
	
	int size = choose_ship_length(shipClass);
	char segments[6];
	int fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	int fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	copy_segments(size, direction, fX, fY, segments,board);

	clear_ship(shipNumber, shipClass, players[pNumber].ship[cl_number].positions[shipNumber].direction, pNumber, cl_number,players,board, board_advanced);

	if (direction == WEST)
	{
		move_left_W(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	else if (direction == EAST)
	{
		move_left_E(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	else if (direction == SOUTH)
	{
		move_left_S(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	else if (direction == NORTH)
	{
		move_left_N(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	set_ship_positon(pNumber, shipClass, fX, fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, shipNumber, players);
}

void move_right_W(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = NORTH;
	players[pNumber].ship[cl_number].positions[shipNumber].backX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	players[pNumber].ship[cl_number].positions[shipNumber].frontY = players[pNumber].ship[cl_number].positions[shipNumber].frontY - size + 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY + i][*fX] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY + i][*fX] = radar_sign;
				else if (i == 1)
					board_advanced[*fY + i][*fX] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY + i][*fX] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY + i][*fX] = alive_sign;
			}
			else
				board_advanced[*fY + i][*fX] = dead_sign;
		}
	}
}
void move_right_E(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = SOUTH;
	players[pNumber].ship[cl_number].positions[shipNumber].backX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	players[pNumber].ship[cl_number].positions[shipNumber].frontY = players[pNumber].ship[cl_number].positions[shipNumber].frontY + size - 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass), *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY - i][*fX] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY - i][*fX] = radar_sign;
				else if (i == 1)
					board_advanced[*fY - i][*fX] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY - i][*fX] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY - i][*fX] = alive_sign;
			}
			else
				board_advanced[*fY - i][*fX] = dead_sign;
		}
	}
}
void move_right_S(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = WEST;

	players[pNumber].ship[cl_number].positions[shipNumber].backY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	players[pNumber].ship[cl_number].positions[shipNumber].frontX = players[pNumber].ship[cl_number].positions[shipNumber].frontX - size + 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX, choose_ship_length(shipClass),* fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY][*fX + i] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY][*fX + i] = radar_sign;
				else if (i == 1)
					board_advanced[*fY][*fX + i] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY][*fX + i] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY][*fX + i] = alive_sign;
			}
			else
				board_advanced[*fY][*fX + i] = dead_sign;
		}
	}
}
void move_right_N(char segments[], char** board_advanced, char command[], int shipNumber, int cl_number, struct Player players[], int pNumber, int size, char shipClass[], int* x, int* y, char** board, char side, int* endGame, int* fX, int* fY)
{
	players[pNumber].ship[cl_number].positions[shipNumber].direction = EAST;

	players[pNumber].ship[cl_number].positions[shipNumber].backY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	players[pNumber].ship[cl_number].positions[shipNumber].frontX = players[pNumber].ship[cl_number].positions[shipNumber].frontX + size - 1;
	*fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	*fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;

	if (is_placement_possible(*fX,size,*fY ,players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == 0)//is placement inside the board
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": SHIP WENT FROM BOARD" << endl;
		*endGame = 1;
	}
	else if (is_placement_possible(*fX, size, *fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, x, y, board) == -1)//if not moving ship on reef
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << shipClass << " " << side << '"' << ": PLACING SHIP ON REEF" << endl;
		*endGame = 1;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[*fY][*fX - i] = segments[i];

			if (segments[i] == alive_sign)
			{
				if (i == 0)
					board_advanced[*fY][*fX - i] = radar_sign;
				else if (i == 1)
					board_advanced[*fY][*fX - i] = cannon_sign;

				if (i == size - 1)
					board_advanced[*fY][*fX - i] = engine_sign;

				if (i != 0 && i != 1 && i != size - 1)
					board_advanced[*fY][*fX - i] = alive_sign;
			}
			else
				board_advanced[*fY][*fX - i] = dead_sign;
		}
	}
}
void move_r(char direction, int pNumber, int shipNumber, char shipClass[], int cl_number,char side, char command[], struct Player players[], int* endGame, int* x, int* y, char** board, char** board_advanced)
{

	move_f(direction, pNumber, shipNumber, shipClass, cl_number, side, command,players, endGame,x,y,board, board_advanced);
	int size = choose_ship_length(shipClass);

	char segments[6];
	int fX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
	int fY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
	copy_segments(size, direction, fX, fY, segments,board);

	clear_ship(shipNumber, shipClass, players[pNumber].ship[cl_number].positions[shipNumber].direction, pNumber, cl_number,players,board, board_advanced);

	if (direction == WEST)
	{
		move_right_W(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame,&fX,&fY);
	}
	else if (direction == EAST)
	{
		move_right_E(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	else if (direction == SOUTH)
	{
		move_right_S(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	else if (direction == NORTH)
	{	
		move_right_N(segments, board_advanced, command, shipNumber, cl_number, players, pNumber, size, shipClass, x, y, board, side, endGame, &fX, &fY);
	}
	set_ship_positon(pNumber, shipClass, fX, fY, players[pNumber].ship[cl_number].positions[shipNumber].direction, shipNumber,players);
}
void copy_segments(int ship_size, char direction, int shipX, int shipY,char seg[], char** board)//copying ship segments from board to segments array
{
	char segments[6] = { '0','0','0','0','0','0' };

	for (int i = 0; i < ship_size; i++)
	{
		if (direction == WEST)
		{
				segments[i] = board[shipY][shipX + i];
		}
		else if (direction == EAST)
		{
				segments[i] = board[shipY][shipX - i];
		}
		else if (direction == SOUTH)
		{
				segments[i] = board[shipY - i][shipX];
		}
		else if (direction == NORTH)
		{
				segments[i] = board[shipY + i][shipX];
		}
	}

	for (int i = 0; i < ship_size; i++)
	{
		seg[i] = segments[i];
	}
	
}