#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <cassert>
#include <exception>

namespace SpaceArcomage{

class Player
{
protected:
	char *name;
	int towerSize;
	int wallSize;
	int querry;
	int magic;
	int dungeon;
	int bricks;
	int gems;
	int beasts;
	
	typedef enum { TOWER_SIZE = 20, WALL_SIZE = 15, QUERRY = 2,
		MAGIC = 2, DUNGEON = 2, BRICKS = 10, GEMS = 10, BEASTS = 10};
public:

//	explicit Player(){}
	explicit Player(char* _name, int _towerSize = TOWER_SIZE, int _wallSize = WALL_SIZE) :
		name(_name), towerSize(_towerSize), wallSize(_wallSize), querry(QUERRY),
		magic(MAGIC), dungeon(DUNGEON), bricks(BRICKS), gems(GEMS),
		beasts(BEASTS){} // empty c'tor

	const char* GetPlayerName() const {
		return name;
	}
	int GetTowerSize() const
	{
		return towerSize;
	}
	int GetWallSize() const
	{
		return wallSize;
	}
	int GetQuerry() const
	{
		return querry;
	}
	int GetMagic() const
	{
		return magic;
	}
	int GetDungeon() const
	{
		return dungeon;
	}
	int GetBricks() const
	{
		return bricks;
	}
	int GetGems() const
	{
		return gems;
	}
	int GetBeasts() const
	{
		return beasts;
	}
	void SetTowerSize(int newTowerSize)
	{
		towerSize += newTowerSize;
	}
	void SetWallSize(int newWallSize)
	{
		wallSize += newWallSize;
	}
	void SetQuerry(int newQuerry)
	{
		querry += newQuerry;
	}
	void SetMagic(int newMagic)
	{
		magic += newMagic;
	}
	void SetDungeon(int newDungeon)
	{
		dungeon += newDungeon;
	}
	void SetBricks(int newBricks)
	{
		bricks += newBricks;
	}
	void SetGems(int newGems)
	{
		gems += newGems;
	}
	void SetBeasts(int newBeasts)
	{
		beasts += newBeasts;
	}
	void ClearTowerSize()
	{
		towerSize = 0;
	}
	void ClearWallSize()
	{
		wallSize = 0;
	}
	void ClearQuerry()
	{
		querry = 0;
	}
	void ClearMagic()
	{
		magic = 0;
	}
	void ClearDungeon()
	{
		dungeon = 0;
	}
	void ClearBricks()
	{
		bricks = 0;
	}
	void ClearGems()
	{
		gems = 0;
	}
	void ClearBeasts()
	{
		beasts = 0;
	}
	void Damage(int damage)
	{
		assert(damage > 0);
		if(GetWallSize() >= damage)
		{
			SetWallSize(-damage);
			return;
		}
		assert(GetWallSize() - damage < 0);
		SetTowerSize(GetWallSize() - damage);
		SetWallSize(-GetWallSize());
	}
	
	class NotEnoughBricks{};
	class NotEnoughGems{};
	class NotEnoughBeasts{};
	class NoSuchCardExists{};
};

} //SpaceArcomage
#endif