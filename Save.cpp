#include <iostream>
#include "Move.h"
#include "Fleet.h"
#include "Player.h"
#include "Board.h"
#include "placement_conditions.h"
#include "Command.h"
#include "Save.h"

#define alive_sign '+'
//player signs
#define player_A_sign 'A'
#define player_B_sign 'B'
//ships signs
#define CAR "CAR"
#define BAT "BAT"
#define CRU "CRU"
#define DES "DES"
//directions
#define NORTH 'N'
#define SOUTH 'S'
#define WEST 'W'
#define EAST 'E'

#define NUMBER_OF_CLASSES 4
#define NUMBER_OF_PLAYERS 2
#define state_id "[state]"

using namespace std;

//changing player number to letter
char player_from_number(int i)
{
    if (i == 0)
        return player_A_sign;
    else
        return player_B_sign;
}

//main saving function
void save_state(struct Player players[], char* current_player, int* x, int* y,char** board, int* srand_seed, int* srand_set,char command[],char AI, int* reefNumber, reef reefs[],int extended)
{
    cout << state_id << endl;
    cout << "BOARD_SIZE " << *y << ' ' << *x << endl;
    cout << "NEXT_PLAYER " << *current_player << endl;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        char player = player_from_number(i);

        //printing players init position
        cout << "INIT_POSITION " << player << ' ' << players[i].initTopY << ' ' << players[i].initTopX << ' ' << players[i].initBotY << ' ' << players[i].initBotX << endl;

        //printing fleet of player
        cout << "SET_FLEET " << player << ' ';
        for (int p = 0; p < NUMBER_OF_CLASSES; p++)
        {
            cout << players[i].ships[p] << ' ';
        }
        cout << endl;
        for (int p = 0; p < NUMBER_OF_CLASSES; p++)
        {
            save_ships(i, p, player,players,board);
        }
        
    }
    //saving reefs positions
    save_reefs(reefNumber,reefs);

    //saving srand seed
    if (*srand_set == 1)
        cout << "SRAND " << (*srand_seed) + 1 << endl;

    //saving extended ships if chosen
    if (extended == 1)
        cout << "EXTENDED_SHIPS" << endl;

    //saving AI player information
    if (strcmp(command, "SET_AI_PLAYER") == 0)
        cout << "SET_AI_PLAYER " << AI << endl;

    cout << state_id << endl;
}

//changing number to class name
const char* number_to_class(int num)
{
    if (num == 0)
        return CAR;
    else if (num == 1)
        return BAT;
    else if (num == 2)
        return CRU;
    else
        return DES;
}

//function saving ships positions of given player
void save_ships(int i, int p, char player, struct Player players[], char** board)
{
    for (int k = 0; k < players[i].ship[p].counter; k++)
    {
        char direction = players[i].ship[p].positions[k].direction;

        int fY = players[i].ship[p].positions[k].frontY;
        int fX = players[i].ship[p].positions[k].frontX;

        cout << "SHIP " << player << ' ' << fY << ' ' << fX;
        cout << ' ' << direction << ' ' << k << ' ' << number_to_class(p) << ' ';

        int ship_size = choose_ship_length((char*)number_to_class(p));


        for (int t = 0; t < ship_size; t++)
        {
            if (direction == WEST)
            {
                if (board[fY][fX + t] == alive_sign)
                    cout << 1;
                else
                    cout << 0;
            }
            else if (direction == EAST)
            {
                if (board[fY][fX - t] == alive_sign)
                    cout << 1;
                else
                    cout << 0;
            }
            else if (direction == SOUTH)
            {
                if (board[fY - t][fX] == alive_sign)
                    cout << 1;
                else
                    cout << 0;
            }
            else if (direction == NORTH)
            {
                if (board[fY + t][fX] == alive_sign)
                    cout << 1;
                else
                    cout << 0;
            }
        }
        cout << endl;
    }
}