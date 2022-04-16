#include<iostream>
#include "Player.h"
#include "Fleet.h"
#include "Board.h"
#include "placement_conditions.h"


#define reef_sign '#'
#define alive_sign '+'
#define dead_sign 'x'
//player signs
#define player_A_sign 'A'
#define player_B_sign 'B'
//directions
#define NORTH 'N'
#define SOUTH 'S'
#define WEST 'W'
#define EAST 'E'

#define NUMBER_OF_PLAYERS 2
#define NUMBER_OF_CLASSES 4

using namespace std;

int is_placement_possible(int shipX, int ship_size, int shipY, char direction, int* x, int* y, char** board)//is placement inside the board and not on reef
{
	int possibility = 1;

	if (shipX < 0 || shipY>=*y || shipY<0 || shipX >= *x)
	{
		possibility = 0;
		return possibility;
	}
		
	for (int i = 0; i < ship_size; i++)
	{
		if (direction == WEST)
		{
				if (shipX + i > *x)
					possibility = 0;
				else if (board[shipY][shipX + i] == reef_sign)
					possibility = -1;
		}
		else if (direction == SOUTH)
		{
				if (shipY - i < 0)
					possibility = 0;
				else if (board[shipY - i][shipX] == reef_sign)
					possibility = -1;
		}
		else if (direction == EAST)
		{
				if (shipX - i < 0)
					possibility = 0;
				else if (board[shipY][shipX - i] == reef_sign)
					possibility = -1;
		}
		else if (direction == NORTH)
		{
				if (shipY + i > *y)
					possibility = 0;
				else if (board[shipY + i][shipX] == reef_sign)
					possibility = -1;
		}
	}
	return possibility;
}
int is_placement_in_init_position(int shipY, int shipX,char playing,char direction, int size,Player players[])//is placement in players initial position
{
	int possible = 1;
	if (playing == player_A_sign)
	{
		possible = placement_in_init_for_player(shipY, shipX, 0,direction,size, players);
	}
	else if (playing == player_B_sign)
	{
		possible = placement_in_init_for_player(shipY, shipX, 1,direction,size, players);
	}
	return possible;
}
int placement_in_init_for_player(int shipY, int shipX, int pNumber,char direction,int ship_size, Player players[])
{
	int possibility = 1;
	if (direction == WEST)
	{
	if (shipX + ship_size-1 > players[pNumber].initBotX)
		possibility = 0;
	}
	else if (direction == SOUTH)
	{
	if (shipY - ship_size +1< players[pNumber].initTopY)
		possibility = 0;
	}
	else if (direction == EAST)
	{
	if (shipX - ship_size+1 < players[pNumber].initTopX)
		possibility = 0;
	}
	else if (direction == NORTH)
	{
	if (shipY + ship_size -1> players[pNumber].initBotY)
		possibility = 0;
	}
	return possibility;
}
int all_ships_placed(struct Player players[])
{
	int placed = 0;

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		for (int p = 0; p < NUMBER_OF_CLASSES; p++)
		{
			for (int k = 0; k < players[i].ship[p].number; k++)
			{
				if (players[i].ship[p].instances[k] == 0)
					placed++;
			}
		}
	}
	return placed;
}

//checking other if there is other ship near
int point(int pointY, int pointX, int* x, int* y, char** board)
{
	if (pointY < 0 || pointY >= *y)
	{
		return 0;
	}
	if (pointX < 0 || pointX >= *x)
	{
		return 0;
	}
	if (board[pointY][pointX] == alive_sign || board[pointY][pointX] == dead_sign)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int check_for_every_segment(int pointY,int pointX,int* x, int* y, char** board)
{
	int oneAbove = point(pointY - 1, pointX,x,y,board);
	int oneBelow = point(pointY + 1, pointX, x, y, board);
	int oneRight = point(pointY, pointX + 1, x, y, board);
	int oneLeft = point(pointY, pointX - 1, x, y, board);
	int leftTop = point(pointY-1, pointX - 1, x, y, board);
	int rightTop = point(pointY - 1, pointX +1, x, y, board);
	int leftBot = point(pointY + 1, pointX - 1, x, y, board);
	int rightBot = point(pointY + 1, pointX + 1, x, y, board);

	int sum = oneAbove + oneBelow + oneRight + oneLeft + leftTop + rightTop + leftBot + rightBot;
	return sum;
}
void check_close(int i,int pX,int pY, int size, int shipY, int shipX, char shipDirection, int shipNumber, char shipClass[],char player,char side,char segments[], char command[],int* endGame, int* x, int* y, char** board)
{
	if (i == 0 || i == size - 1)
	{
		if (check_for_every_segment(pY, pX,x,y,board) > 1)
		{
			error_placing_close(shipY, shipX, shipDirection, shipNumber, shipClass,player,side,segments,command, endGame);
		}
	}
	else
	{
		if (check_for_every_segment(pY, pX,x,y,board) > 2)
		{
			error_placing_close(shipY, shipX, shipDirection, shipNumber, shipClass,player,side,segments,command, endGame);
		}
	}
}
void error_placing_close(int shipY, int shipX, char shipDirection, int shipNumber, char shipClass[],char player,char side,char segments[],char command[], int* endGame)
{
	if (strcmp(command, "PLACE_SHIP") == 0)
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipY << " " << shipX << " " << shipDirection << " " << shipNumber << " " << shipClass << '"' << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
		*endGame = 1;
	}
	else if(strcmp(command, "SHIP") == 0)
	{
		cout << "INVALID OPERATION " << '"' << command << " " <<player<<" " << shipY << " " << shipX << " " << shipDirection << " " << shipNumber << " " << shipClass <<" "<<segments << '"' << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
		*endGame = 1;
	}
	else if (strcmp(command, "MOVE") == 0)
	{
		cout << "INVALID OPERATION " << '"' << command <<" "<<shipNumber <<" "<< shipClass <<" "<<side<<'"' << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
		*endGame = 1;
	}
}
void other_ships(char shipClass[],char shipDirection,int pNumber,int shipNumber, int shipY, int shipX,char player,char side,char segments[], char command[], Player players[], int* endGame, int* x, int* y, char** board)
{
	int cl_number = choose_class_number(shipClass);
	int size = choose_ship_length(shipClass);

	if (shipDirection == WEST)
	{
		for (int i = 0; i < size; i++)
		{
			int pX, pY;
			pX = players[pNumber].ship[cl_number].positions[shipNumber].frontX + i;
			pY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
			check_close(i, pX, pY, size,shipY,shipX,shipDirection,shipNumber,shipClass,player,side,segments,command, endGame,x,y,board);
			if (*endGame == 1)
				break;
		}
	}
	else if (shipDirection == SOUTH)
	{
		for (int i = 0; i < size; i++)
		{
			int pX, pY;
			pX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
			pY = players[pNumber].ship[cl_number].positions[shipNumber].frontY-i;
			check_close(i, pX, pY, size, shipY, shipX, shipDirection, shipNumber, shipClass,player,side,segments, command, endGame,x,y,board);
			if (*endGame == 1)
				break;
		}
	}
	else if (shipDirection == EAST)
	{
		for (int i = 0; i < size; i++)
		{
			int pX, pY;
			pX = players[pNumber].ship[cl_number].positions[shipNumber].frontX -i;
			pY = players[pNumber].ship[cl_number].positions[shipNumber].frontY;
			check_close(i, pX, pY, size, shipY, shipX, shipDirection, shipNumber, shipClass,player,side,segments, command, endGame,x,y,board);
			if (*endGame == 1)
				break;
		}
	}
	else if (shipDirection == NORTH)
	{
		for (int i = 0; i < size; i++)
		{
			int pX, pY;
			pX = players[pNumber].ship[cl_number].positions[shipNumber].frontX;
			pY = players[pNumber].ship[cl_number].positions[shipNumber].frontY+i;
			check_close(i, pX, pY, size, shipY, shipX, shipDirection, shipNumber, shipClass,player,side,segments, command, endGame,x,y,board);
			if (*endGame == 1)
				break;
		}
	}
}
