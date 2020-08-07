#ifndef _ARCOMAGE_H_
#define _ARCOMAGE_H_
#if defined NULL
#undef NULL
#endif

#define NULL (0)

#define UNDO		999
#define SAME_TURN	1

#include "Player.h"
#include <iostream>
#include <cassert>

using namespace std;
//using std::cout;
//using std::cin;
//using std::endl;

namespace SpaceArcomage{

class Arcomage
{
protected:
	Player** players;
	Player** prevValues;
	Player** oldValues;
	int cardId;
	int numOfPlayers;
	int towerSizeToWin;
	int arrayFlag;
	int resourceAmountToWin;

	typedef enum {NO_CARD_VALUE = 0, TOWER_SIZE_TO_WIN = 70};
	// for now:
	//-------------------------------------------------
	Arcomage(const Arcomage& ){}
	Arcomage& operator=(const Arcomage&){}
	//------------- copy c'tor and operator= should be avoided at this time

	void RestorePreviousValues(){
		if(!arrayFlag){
			for(int i=0; i<numOfPlayers; ++i){
				*players[i] = *oldValues[i]; // default Player::operator= is good enough
			}
			return;
		}
		for(int i=0; i<numOfPlayers; ++i){
			*players[i] = *prevValues[i]; // default Player::operator= is good enough
		}
	}
	
	int SwitchFlag(int value){
		return value ? 0 : 1;
	}

public:
	Arcomage(char** names, int towerSizeToWin = 70,
			int _numOfPlayers = 2, int _resourceAmountToWin = 250):
		players(new Player*[_numOfPlayers]),
		prevValues(new Player*[_numOfPlayers]),
		oldValues(new Player*[_numOfPlayers]),
		cardId(NO_CARD_VALUE),
		numOfPlayers(_numOfPlayers),
		towerSizeToWin(towerSizeToWin),
		arrayFlag(0),
		resourceAmountToWin(_resourceAmountToWin)
		{
			for(int i=0; i<numOfPlayers; ++i)
			{
				players[i] = new Player(names[i]);
				prevValues[i] = new Player(names[i]);
				oldValues[i] = new Player(names[i]);
			}
		}

	~Arcomage(){
		for(int i=0; i<numOfPlayers; ++i)
		{
			delete players[i];
			players[i] = NULL;
			delete prevValues[i];
			prevValues[i] = NULL;
			delete oldValues[i];
			oldValues[i] = NULL;
		}
		delete[] players;
		players = NULL;
		delete[] prevValues;
		prevValues = NULL;
		delete[] oldValues;
		oldValues = NULL;
	}

	void CalculateGameChanges(int playingPlayer, int* sameTurn);

	void SavePrevValues(){	
		if(!arrayFlag){
			for(int i=0; i<numOfPlayers; ++i){
				*oldValues[i] = *players[i];
			}
		}
		else{
			for(int i=0; i<numOfPlayers; ++i){
				*prevValues[i] = *players[i];
			}
		}
		arrayFlag = SwitchFlag(arrayFlag);
	}
	void FinishTurn(int playingPlayer)
	{
		assert(playingPlayer >= 0 && playingPlayer < numOfPlayers);
		Player* player = players[playingPlayer];
		player->SetBricks(player->GetQuerry());
		player->SetGems(player->GetMagic());
		player->SetBeasts(player->GetDungeon());
	}
	
	void CorrectGameValues()
	{
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(players[i]->GetTowerSize() < 0)
			{
				players[i]->ClearTowerSize();
			}
			if(players[i]->GetBeasts() < 0)
			{
				players[i]->ClearBeasts();
			}
			if(players[i]->GetBricks() < 0)
			{
				players[i]->ClearBricks();
			}
			if(players[i]->GetDungeon() < 0)
			{
				players[i]->ClearDungeon();
			}
			if(players[i]->GetGems() < 0)
			{
				players[i]->ClearGems();
			}
			if(players[i]->GetMagic() < 0)
			{
				players[i]->ClearMagic();
			}
			if(players[i]->GetQuerry() < 0)
			{
				players[i]->ClearQuerry();
			}
			if(players[i]->GetWallSize() < 0)
			{
				players[i]->ClearWallSize();
			}
		}
	}

	int GetNumberOfPlayers() const
	{
		return numOfPlayers;
	}

	bool IsThereWinnerInGame() const
	{
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(players[i]->GetTowerSize() >= TOWER_SIZE_TO_WIN ||
				players[i]->GetTowerSize() <= 0 || 
				(players[i]->GetBricks() >= resourceAmountToWin &&
				players[i]->GetGems() >= resourceAmountToWin &&
				players[i]->GetBeasts() >= resourceAmountToWin) )
			{
				return true;
			}
		}
		return false;
	}

	ostream& Print(ostream& os) const
	{
		os << "************************************************" << endl;
		for(int i=0; i<numOfPlayers; ++i)
		{
			Player* player = players[i];
			os << "************************************************" << endl;
			os << "Statistics of " << player->GetPlayerName() << ":" << endl;
			os << "Tower size = " << player->GetTowerSize() << endl;
			os << "Wall size = " << player->GetWallSize() << endl;
			os << "Quarry size = " << player->GetQuerry() << endl;
			os << "\tBricks amount = " << player->GetBricks() << endl;
			os << "Magic amount = " << player->GetMagic() << endl;
			os << "\tGems size = " << player->GetGems() << endl;
			os << "Dungeon size = " << player->GetDungeon() << endl;
			os << "\tBeasts amount = " << player->GetBeasts() << endl;
		}
		return os;
	}

	void SetNextCard(int nextCardId)
	{
		cardId = nextCardId;
	}
};


// global functions:
inline istream& operator>>(istream& in, Arcomage& game)
{
	int nextCardId = 0;
	in >> nextCardId;
	game.SetNextCard(nextCardId);
	return in;
}

inline ostream& operator<<(ostream& os, const Arcomage& game)
{
	return game.Print(os);
}


} // SpaceArcomage


#endif