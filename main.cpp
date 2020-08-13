#include "arcomage.h"
#include "versions.h"


#define NOT_SAME_TURN 0
#define MAX_LENGTH 256
using namespace SpaceArcomage;

int main()
{
	int retVal = 0;
	const int FIRST_PLAYER = 0;
	int repeatFlag = 0;
	int towerSizeToWin = 70, numOfPlayers = 2, resourceAmountToWin = 250;
	int turn = 0, sameTurn = 0;
/*	cout << "Please enter the tower size to win: ";
	cin >> towerSizeToWin;
	cout << "Please enter the number of players: ";
	cin >> numOfPlayers;
	cout << "Please enter the resource size to win: ";
	cin >> resourceAmountToWin;
*/
	cout << "Tower size to win: " << towerSizeToWin << ", Number of players: " << 
		numOfPlayers << ", Resource amount to win: " << resourceAmountToWin << endl;
	char** names = new char*[numOfPlayers];
	
	for (int i = 0; i < numOfPlayers; ++i) {
		cout << "Enter name of player number " << i + 1 << ": ";
		char *name = new char[MAX_LENGTH];
		cin >> name;
		names[i] = name;
	}
	try{
		Arcomage game(names, towerSizeToWin, numOfPlayers, resourceAmountToWin);
		const int ILLEGAL_VALUE = -100;
		int prevTurn = ILLEGAL_VALUE;	
		while(!game.IsThereWinnerInGame())
		{
			if(repeatFlag == 0)
			{
				cout << game << endl;
				cout << "It is player " << turn+1 << "'s turn (" << names[turn] << "):";
				game.SavePrevValues();
			}
			sameTurn = NOT_SAME_TURN;

			cin >> game;	// inserts the next card to throw
			try {
				game.CalculateGameChanges(turn, &sameTurn);
			}
			catch(const Player::NotEnoughBeasts&)
			{
				cout << "Not Enough Beasts, try again: ";
				repeatFlag = 1;
				continue;
			}
			catch(const Player::NotEnoughBricks&)
			{
				cout << "Not Enough Bricks, try again: ";
				repeatFlag = 1;
				continue;
			}
			catch(const Player::NotEnoughGems&)
			{
				cout << "Not Enough Gems, try again: ";
				repeatFlag = 1;
				continue;
			}
			catch(const Player::NoSuchCardExists&)
			{
				cout << "No Such Card Exists, try again: ";
				repeatFlag = 1;
				continue;
			}
			catch(...){
				cout << "An Unknown Error occured, exitting..." << endl;
				return 1;
			}
			if(sameTurn == SAME_TURN){
				prevTurn = turn;
				game.CorrectGameValues();
				continue;
			}
			if(sameTurn != UNDO){
				game.FinishTurn(turn);
			}
			if(sameTurn == NOT_SAME_TURN){
				turn++;
			}
			if(sameTurn == UNDO){
				if(prevTurn == turn){ // it meens we had a "same turn" card and an UNDO one
					prevTurn = ILLEGAL_VALUE;
				}
				else{
					if(turn == 0){
						turn = numOfPlayers-1;
					}
					else{
						turn--;
					}
				}
			}
			if(turn == game.GetNumberOfPlayers())
			{
				turn = FIRST_PLAYER;
			}
			game.CorrectGameValues();
			repeatFlag = 0;
		}
	}
	catch(const bad_alloc&){
		cout << "Not enough memory to allocate, exitting... " << endl;
		retVal = 1;
	}
	if(turn != FIRST_PLAYER){
		cout << "Player " << turn << " is the winner!" << endl;
	}
	else {
		cout << "Player " << numOfPlayers << " is the winner!" << endl;
	}
	
	for (int i = 0; i < numOfPlayers; ++i) {
		delete names[i];
		names[i] = NULL;
	}
	delete[] names;
	return retVal;
}


/*
int main(){
	int towerSizeToWin = 70, numOfPlayers = 3;
	int j = 1, sameTurn = 0;
	bool repeatFlag = false;
	Arcomage game(towerSizeToWin, numOfPlayers);
	while(j){
		for(int i=0; i<numOfPlayers; ++i){
			if(sameTurn != UNDO){
				game.SavePrevValues();
			}
			if(sameTurn == UNDO){
				sameTurn = 0;
				i--;
				if(i == -1){
					i = numOfPlayers-1;
				}
			}
			if(repeatFlag){
				i--;
				if(i == -1){
					i = numOfPlayers-1;
				}
				repeatFlag = false;
			}
			else{
				cout << game << endl;
				cout << "It is player " << i+1 << "'s turn : ";
			}
			cin >> game;
			try {
				game.CalculateGameChanges(i, &sameTurn);
			}
			catch(const Player::NotEnoughBeasts&)
			{
				cout << "Not Enough Beasts, try again: ";
				repeatFlag = true;
				continue;
			}
			catch(const Player::NotEnoughBricks&)
			{
				cout << "Not Enough Bricks, try again: ";
				repeatFlag = true;
				continue;
			}
			catch(const Player::NotEnoughGems&)
			{
				cout << "Not Enough Gems, try again: ";
				repeatFlag = true;
				continue;
			}
			catch(const Player::NoSuchCardExists&)
			{
				cout << "No Such Card Exists, try again: ";
				repeatFlag = true;
				continue;
			}
			catch(...){
				cout << "An Unknown Error occured, exitting..." << endl;
				return 1;
			}
			if(sameTurn == 1){
				sameTurn = 0;
				repeatFlag = true;
				cout << game << endl;
				cout << "It is player " << i+1 << "'s turn : ";
			}
			game.CorrectGameValues();
			
			if(game.IsThereWinnerInGame()){
				return 0;
			}
		}
	}
	return 0;
}

*/
