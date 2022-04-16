#include<iostream>
#include "Board.h"
#include "Player.h"
#include "Fleet.h"
#include "Command.h"
#include "Shooting.h"

#define reef_sign '#'
#define not_visible_sign '?'

#define NORTH 'N'
#define SOUTH 'S'
#define WEST 'W'
#define EAST 'E'

#define NUMBER_OF_PLAYERS 2
#define NUMBER_OF_CLASSES 4


using namespace std;

//allocating radars arrays
void alloc_radars(struct Player players[])
{
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		players[i].radars = (radar*)malloc(players[i].r * sizeof(radar));
	}
}

//initializing default board size
void default_board(struct Player players[], int* x, int* y, char*** board, char*** board_advanced)
{
	players[0].boardPlayer = (char**)malloc(*y * sizeof(char*));
	players[1].boardPlayer = (char**)malloc(*y * sizeof(char*));


	for (int i = 0; i < *y; i++)
	{
		(*board)[i] = (char*)malloc(*x * sizeof(char));
		(*board_advanced)[i] = (char*)malloc(*x * sizeof(char));
		players[0].boardPlayer[i] = (char*)malloc(*x * sizeof(char));
		players[1].boardPlayer[i] = (char*)malloc(*x * sizeof(char));
	}
		

	for (int i = 0; i < *y; i++)
	{
		for (int k = 0; k < *x; k++)
		{
			(*board)[i][k] = ' ';
			(*board_advanced)[i][k] = ' ';
			players[0].boardPlayer[i][k] = not_visible_sign;
			players[1].boardPlayer[i][k] = not_visible_sign;
		}

	}
}

void get_board_size(int* x, int* y)
{
	cin >> *y >> *x;
}

//function for setting choosen x and y
void set_the_board(struct Player players[], int* x, int* y, char*** board, char*** board_advanced)
{
	
		
		get_board_size(x, y);

		//allocating main board and boards for both players
		*board = (char**)malloc((*y) * sizeof(char*));
		(*board_advanced) = (char**)malloc(*y * sizeof(char*));
		players[0].boardPlayer = (char**)malloc(*y * sizeof(char*));
		players[1].boardPlayer = (char**)malloc(*y * sizeof(char*));


		for (int i = 0; i < *y; i++)
		{
			(*board)[i] = (char*)malloc((*x) * sizeof(char));
			(*board_advanced)[i] = (char*)malloc(*x * sizeof(char));
			players[0].boardPlayer[i] = (char*)calloc(*x, sizeof(char));
			players[1].boardPlayer[i] = (char*)calloc(*x, sizeof(char));
		}
		blank_board(*x, *y,*board, *board_advanced, players);

		
	
}
void blank_board(int x,int y,char** board,char** board_advanced,struct Player players[])
{
	for (int i = 0; i < y; i++)
	{
		for (int k = 0; k < x; k++)
		{
			if (board[i][k] == NULL)
				cout << "null" << endl;
			else
				board[i][k] = 32;

			board_advanced[i][k] = 32;
			players[0].boardPlayer[i][k] = not_visible_sign;
			players[1].boardPlayer[i][k] = not_visible_sign;
		}

	}
}

//choosing print version
void print_command(struct Player players[], int* x, int* y, char** board, char** board_advanced,int current_state)
{
	int printX;
	cin >> printX;

	if (printX == 0)
	{
		print_the_board_simple(players,*x,*y,board,current_state);
	}
	else if(printX==1)
	{
		print_the_board_advanced(players,x,y,board_advanced,current_state);
	}
}

//printing for x=0
void print_the_board_simple(struct Player players[], int x, int y, char** board, int current_state)
{
	for (int i = 0; i < y; i++)
	{
		for(int k=0;k<x;k++)
			cout << board[i][k];

		cout << endl;
	}
	if (current_state == 0)
		parts_remaining(players);
}

//printing for x=1
void print_the_board_advanced(struct Player players[], int* x, int* y, char** board_advanced,int current_state)
{
	int is_above_ten_y = 1;
	int is_above_ten_x = 1;
	if (*y > 10)
		is_above_ten_y = 2;
	if (*x > 10)
		is_above_ten_x = 2;

	int fch = -1;
	int fch2 = -1;
	for (int i = 0; i < *y + is_above_ten_x; i++)
	{
		//printing rows numbers
		if (i > is_above_ten_x-1)
		{
			if (is_above_ten_y == 2)
			{
				if ((i - is_above_ten_x) % 10 == 0)
				{
					fch++;
				}
				cout << fch;
			}
			cout << (i - (is_above_ten_x)) % 10;
		}

		if (i > is_above_ten_x-1)
		{
			for (int k = 0; k < *x; k++)
			{
				cout << board_advanced[i - is_above_ten_x][k];
			}
		}
		else//printing columns numbers
		{
			for (int k = 0; k < *x + is_above_ten_y; k++)
			{
				if (k > is_above_ten_y-1)
				{
					if (is_above_ten_x == 2)
					{
						if (i == 0)
						{
							if ((k - 2) % 10 == 0)
							{
								fch2++;
							}
							cout << fch2;
						}
						else if (i == 1)
						{
							cout << (k - 2) % 10;
						}
					}
					else
						cout << (k - is_above_ten_y) % 10;
				}
				else
					cout << ' ';
			}
		}
		
		cout << endl;
	}

	//if it is printing for state we print Parts remaining
	if(current_state==0)
		parts_remaining(players);
}

//print command from players perspective
void print_command_player(struct Player players[], char* current_player, int* x, int* y, char** board, char** board_advanced, int current_state)
{
	int printX;
	cin >> printX;

	int pNumber=choose_pNumber(*current_player);;
	
	for (int i = 0; i < *y; i++)
	{
		for (int k = 0; k < *x; k++)
		{
			players[0].boardPlayer[i][k]= not_visible_sign;
			players[1].boardPlayer[i][k] = not_visible_sign;
		}
	}

	//printing players board for given x
	if (printX == 0)
	{
		check_visibility(pNumber, 0, players, x, y, board, board_advanced);
		print_the_board_simple(players, *x, *y, players[pNumber].boardPlayer,current_state);
	}	
	else if (printX == 1)
	{
		check_visibility(pNumber, 1, players, x, y, board, board_advanced);
		print_the_board_advanced(players, x, y, players[pNumber].boardPlayer,current_state);
	}
		
	
	
}

//marking visible cells on player's board
void check_visibility(int pNumber,int advanced, struct Player players[], int* x, int* y, char** board, char** board_advanced)
{
	for (int i = 0; i < players[pNumber].r; i++)
	{
		int rX = players[pNumber].radars[i].radarX;
		int rY = players[pNumber].radars[i].radarY;

		for (int p = 0; p < *y; p++)
		{
			for (int k = 0; k < *x; k++)
			{
				//checking fields in radars range
				if (distance((double)rY, (double)rX, (double)p, (double)k) <= players[pNumber].radars[i].range)
				{
					if (advanced == 1)
					{
						players[pNumber].boardPlayer[p][k] = board_advanced[p][k];
					}	
					else 
						players[pNumber].boardPlayer[p][k] = board[p][k];
				}
			}
		}
		
	}
	reefs_visibility(pNumber,players, x, y, board);
	players_ships_visibility(pNumber, advanced,players,board, board_advanced);
}

//marking all reefs visible for player
void reefs_visibility(int pNumber, struct Player players[], int* x, int* y, char** board)
{
	for (int p = 0; p < *y; p++)
	{
		for (int k = 0; k < *x; k++)
		{
			if (board[p][k] == reef_sign)
				players[pNumber].boardPlayer[p][k] = reef_sign;
			
		}
	}
}

//player sees his own ships even without a radar
void players_ships_visibility(int pNumber,int advanced,  struct Player players[],char** board, char** board_advanced)
{
	for (int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		for (int p = 0; p < players[pNumber].ship[i].number; p++)
		{
			int direction = players[pNumber].ship[i].positions[p].direction;
			int fX = players[pNumber].ship[i].positions[p].frontX;
			int fY = players[pNumber].ship[i].positions[p].frontY;
			int size = 5 - i;

			for (int z = 0; z < size; z++)
			{
				if (direction == WEST)
				{
					if(advanced==0)
						players[pNumber].boardPlayer[fY][fX + z] = board[fY][fX + z];
					else
						players[pNumber].boardPlayer[fY][fX + z] = board_advanced[fY][fX + z];
				}
				else if (direction == EAST)
				{
						if (advanced == 0)
							players[pNumber].boardPlayer[fY][fX - z] = board[fY][fX - z];
						else
							players[pNumber].boardPlayer[fY][fX - z] = board_advanced[fY][fX - z];
				}
				else if (direction == SOUTH)
				{
						if (advanced == 0)
							players[pNumber].boardPlayer[fY - z][fX] = board[fY - z][fX];
						else
							players[pNumber].boardPlayer[fY - z][fX] = board_advanced[fY - z][fX];
				}
				else if (direction == NORTH)
				{
						if (advanced == 0)
							players[pNumber].boardPlayer[fY + z][fX] = board[fY + z][fX];
						else
							players[pNumber].boardPlayer[fY + z][fX] = board_advanced[fY + z][fX];
				}
			}
			
		}
	}
}

//freeing memory alocated for board array
void clean_board(struct Player players[], int* y, char** board, char** board_advanced)
{
	for (int i = 0; i < *y; i++) {
		free(board[i]);
		free(board_advanced[i]);
		free(players[0].boardPlayer[i]);
		free(players[1].boardPlayer[i]);
	}
	free(board);
	free(board_advanced);

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		free(players[i].boardPlayer);
	}
	
}

//checking if game is over and printing who won
void check_for_win( struct Player players[],int* endGame)
{
	int PlayerAActive = players[0].alive_fragments;
	int PlayerBActive = players[1].alive_fragments;

	if (PlayerAActive == 0)
	{
		cout << "B won" << endl;
		(*endGame)++;
	}
	else if (PlayerBActive == 0)
	{
		cout << "A won" << endl;
		(*endGame)++;
	}	
}

//counting remaining parts for every player
void parts_remaining(struct Player players[])
{
	int partsA = players[0].alive_fragments;
	int partsB = players[1].alive_fragments;
	
	cout << "PARTS REMAINING:: A : " << partsA << " B : " << partsB << endl;
}

//placing reef on board and adding it to reefs array
void place_reef(char command[], int* endGame, int* x, int* y, char** board,int* reefNumber, reef** reefs)
{
	int reefY, reefX;
	cin >> reefY >> reefX;
	if (reefX < 0 || reefY >= *y || reefY < 0 || reefX >= *x)
	{
		cout << "INVALID OPERATION " << '"' << command << " " << reefY << " " << reefX << '"' << ": REEF IS NOT PLACED ON BOARD" << endl;
		*endGame = 1;
	}	
	else
	{
		board[reefY][reefX] = reef_sign;

		//extending reefs array and adding ne reef
		reef* new_reefs = (reef*)malloc((*reefNumber) * sizeof(reef));
		for (int k = 0; k < (*reefNumber); k++)
		{
			new_reefs[k] = (*reefs)[k];
		}

		(*reefNumber)++;
		*reefs = (reef*)malloc((*reefNumber) * sizeof(reef));


		for (int k = 0; k < (*reefNumber)-1; k++)
		{
			(*reefs)[k].reefX = new_reefs[k].reefX;
			(*reefs)[k].reefY = new_reefs[k].reefY;
		}
		
		(*reefs)[(*reefNumber) - 1].reefX = reefX;
		(*reefs)[(*reefNumber) - 1].reefY = reefY;
		

		free(new_reefs);

	}
}

//printing all reefs
void save_reefs(int* reefNumber, reef reefs[])
{
	for (int i = 0; i < *reefNumber; i++)
	{
		cout << "REEF " << reefs[i].reefY << ' ' << reefs[i].reefX << endl;
	}
}

//placing radar on the board
void radar_to_add(int shipNumber, int i, int pNumber,double size, struct Player players[])
{
	radar* newArray = (radar*)malloc((players[pNumber].r) * sizeof(radar));

	for (int p = 0; p < (players[pNumber].r); p++)
	{
		newArray[p] = players[pNumber].radars[p];
	}

	players[pNumber].r++;
	players[pNumber].radars = (radar*)malloc((players[pNumber].r) * sizeof(radar));

	for (int p = 0; p < (players[pNumber].r)-1; p++)
	{
		players[pNumber].radars[p] = newArray[p];
	}

	players[pNumber].radars[(players[pNumber].r)-1].radarX = players[pNumber].ship[i].positions[shipNumber].frontX;
	players[pNumber].radars[(players[pNumber].r)-1].radarY = players[pNumber].ship[i].positions[shipNumber].frontY;
	players[pNumber].radars[(players[pNumber].r) - 1].range = size;

	free(newArray);
}

//spy planes placing
void place_spy(char command[],  struct Player players[], char* current_player, int* endGame,char** board)
{
	int planeX, planeY,shipNumber;
	cin >>shipNumber>> planeY >> planeX;

	int pNumber = choose_pNumber(*current_player);

	int cannonX = players[pNumber].ship[0].positions[shipNumber].cannonX;
	int cannonY = players[pNumber].ship[0].positions[shipNumber].cannonY;

	if (max_shoots(pNumber, 0, shipNumber,players,endGame) == 1)
	{
		if (point_alive(cannonY, cannonX,board) == 1)
		{
			radar* newArray = (radar*)malloc((players[pNumber].r) * sizeof(radar));

			for (int p = 0; p < (players[pNumber].r); p++)
			{
				newArray[p] = players[pNumber].radars[p];
			}

			players[pNumber].r++;
			players[pNumber].radars = (radar*)malloc((players[pNumber].r) * sizeof(radar));

			for (int p = 0; p < (players[pNumber].r) - 1; p++)
			{
				players[pNumber].radars[p] = newArray[p];
			}

			players[pNumber].radars[(players[pNumber].r) - 1].radarX = planeX;
			players[pNumber].radars[(players[pNumber].r) - 1].radarY = planeY;
			players[pNumber].radars[(players[pNumber].r) - 1].range = 1.5;
			players[pNumber].ship[0].shoots[shipNumber]--;

			free(newArray);
		}
		else
		{
			cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << planeY << " " << planeX << '"' << ": CANNOT SEND PLANE" << endl;
			*endGame = 1;
		}			
	}
	else
	{
		cout << "INVALID OPERATION " << '"' << command << " " << shipNumber << " " << planeY << " " << planeX << '"' << ": ALL PLANES SENT" << endl;
		*endGame = 1;
	}
		
	
}