
#include "definitions.h"
#include "arcomage.h"

#define CHECK_BRICKS(x) if(player->GetBricks() < (x)){		\
							throw Player::NotEnoughBricks(); \
						}									\
						else player->SetBricks((-x))
#define CHECK_GEMS(x)	if(player->GetGems() < (x)){	\
							throw Player::NotEnoughGems(); \
						}	\
						else player->SetGems((-x))
#define CHECK_BEASTS(x)	if(player->GetBeasts() < (x)){	\
							throw Player::NotEnoughBeasts(); \
						}	\
						else player->SetBeasts((-x))
namespace SpaceArcomage{

void Arcomage::CalculateGameChanges(int playingPlayer, int* sameTurn)
{
	Player* player = players[playingPlayer];
	assert(player != NULL);
	switch(cardId)
	{
	case DISCARD:
		return;
	// brown cards:
	case 1:	// brick shortage - costs: 0 bricks
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetBricks(-8);
		}
		return;
	case 2:	// Lucky Cache - costs: 0 bricks
		player->SetBricks(2);
		player->SetGems(2);
		*sameTurn = 1;
		return; // play again
	case 3: //Earthquake - costs: 0 bricks
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetQuerry(-1);
		}
		return;
	case 4:	// Strip Mine - costs: 0 bricks
		player->SetQuerry(-1);
		player->SetWallSize(10);
		player->SetGems(5);
		return;
	case 5: // Friendly Terrain - costs: 1 brick
		CHECK_BRICKS(1);
		player->SetWallSize(1);
		*sameTurn = SAME_TURN;
		return;	// play again
	case 6:	// Rock Garden
		CHECK_BRICKS(1);
		player->SetWallSize(1);
		player->SetTowerSize(1);
		player->SetBeasts(2);
		return;
	case 7:	// Work overtime
		CHECK_BRICKS(2);
		player->SetWallSize(5);
		player->SetGems(-6);
		return;
	case 8:	// Basic Wall
		CHECK_BRICKS(2);
		player->SetWallSize(3);
		return;
	case 9: // Innovations
		CHECK_BRICKS(2);
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetQuerry(1);
		}
		player->SetGems(4);
		return;
	case 10: // Miners
		CHECK_BRICKS(3);
		player->SetQuerry(1);
		return;
	case 11: // Sturdy Wall
		CHECK_BRICKS(3);
		player->SetWallSize(4);
		return;
	case 12: //Foundations
		CHECK_BRICKS(3);
		if(player->GetWallSize() == 0)
		{
			player->SetWallSize(6);
		}
		else
		{
			player->SetWallSize(3);
		}
		return;
	case 13: // Mother Lode
		{
			CHECK_BRICKS(4);
			int maxQuerry = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(players[i]->GetQuerry() > maxQuerry && i != playingPlayer)
				{
					maxQuerry = players[i]->GetQuerry();
				}
			}
			if(player->GetQuerry() < maxQuerry)
			{
				player->SetQuerry(2);
			}
			else
			{
				player->SetQuerry(1);
			}
			return;
		}
	case 14:	// collapse!
		{
			CHECK_BRICKS(4);
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer)
				{
					players[i]->SetQuerry(-1);
				}
			}
			return;
		}
	case 15:	//Copping the Tech
		{
			CHECK_BRICKS(5);
			int maxQuerry = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(players[i]->GetQuerry() > maxQuerry)
				{
					if(i != playingPlayer)
					{
						maxQuerry = players[i]->GetQuerry();
					}
				}
			}
			if(player->GetQuerry() < maxQuerry)
			{
				player->SetQuerry(-player->GetQuerry());
				player->SetQuerry(maxQuerry);
			}
			return;
		}
	case 16:	//Big Wall
		CHECK_BRICKS(5);
		player->SetWallSize(6);
		return;
	case 17:	//New Equipment
		CHECK_BRICKS(5);
		player->SetQuerry(2);
		return;
	case 18:	//Flood Water
		{
			CHECK_BRICKS(6);
			int minWall = player->GetWallSize();
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(players[i]->GetWallSize() < minWall)
				{
					minWall = players[i]->GetWallSize();
				}
			}
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(players[i]->GetWallSize() == minWall)
				{
					players[i]->SetDungeon(-1);
					players[i]->SetTowerSize(-2);
				}
			}
			return;
		}
	case 19:	//Dwarven Miners
		CHECK_BRICKS(7);
		player->SetQuerry(1);
		player->SetWallSize(4);
		return;
	case 20:	//Tremors
		CHECK_BRICKS(7);
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetWallSize(-5);
		}
		*sameTurn = SAME_TURN;
		return;		// play again
	case 21:	//Forced Labor
		CHECK_BRICKS(7);
		player->SetWallSize(9);
		player->SetBeasts(-5);
		return;
	case 22:	//Secret Room
		CHECK_BRICKS(8);
		player->SetMagic(1);
		*sameTurn = SAME_TURN;
		return;		// play again
	case 23:	//Reinforced Wall
		CHECK_BRICKS(8);
		player->SetWallSize(8);
		return;
	case 24:	// Porticulus
		CHECK_BRICKS(9);
		player->SetWallSize(5);
		player->SetDungeon(1);
		return;
	case 25:	//Crystal Rocks
		CHECK_BRICKS(9);
		player->SetWallSize(7);
		player->SetGems(7);
		return;
	case 26:	//Barracks
		CHECK_BRICKS(10);
		player->SetWallSize(6);
		player->SetBeasts(6);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(player->GetDungeon() < players[i]->GetDungeon())
			{
				player->SetDungeon(1);
				return;
			}
		}
		return;
	case 27:	//Harmonic Ore
		CHECK_BRICKS(11);
		player->SetWallSize(6);
		player->SetTowerSize(3);
		return;
	case 28:	//MondoWall
		CHECK_BRICKS(13);
		player->SetWallSize(12);
		return;
	case 29:	//Battlemnets
		CHECK_BRICKS(14);
		player->SetWallSize(7);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetWallSize(-6);
			}
		}
		return;
	case 30:	//Focused Designs
		CHECK_BRICKS(15);
		player->SetWallSize(8);
		player->SetTowerSize(5);
		return;
	case 31:	//Great Wall
		CHECK_BRICKS(16);
		player->SetWallSize(15);
		return;
	case 32:	// Shift
		{
			CHECK_BRICKS(17);
			int maxWall = 0;
			int indexOfPlayer = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(players[i]->GetWallSize() > maxWall && i != playingPlayer)
				{
					maxWall = players[i]->GetWallSize();
					indexOfPlayer = i;
				}
			}
			int tempWall = player->GetWallSize();
			player->SetWallSize(-tempWall + 
				players[indexOfPlayer]->GetWallSize());
			players[indexOfPlayer]->SetWallSize(
				-players[indexOfPlayer]->GetWallSize() + tempWall);
			return;
		}
	case 33:	//Rock Launcher
		CHECK_BRICKS(18);
		player->SetWallSize(6);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(10);
			}
		}
		return;
	case 34:	//Dragon's Heart
		CHECK_BRICKS(24);
		player->SetWallSize(20);
		player->SetTowerSize(8);
		return;
		
		//************************************************************
		//--------------------- blue cards: --------------------------
	case 35:	//Bag of Baubles
		{
			int maxTower = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer && maxTower < players[i]->GetTowerSize())
				{
					maxTower = players[i]->GetTowerSize();
				}
			}
			if(player->GetTowerSize() < maxTower)
			{
				player->SetTowerSize(2);
			}
			else
			{
				player->SetTowerSize(1);
			}
			return;
		}
	case 36:	//Rainbow
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetTowerSize(1);
		}
		player->SetGems(3);
		return;
	case 37:	//Quartz
		CHECK_GEMS(1);
		player->SetTowerSize(1);
		*sameTurn = SAME_TURN;
		return;	// play again
	case 38:	//Smoky Quartz
		CHECK_GEMS(2);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-1);
			}
		}
		*sameTurn = SAME_TURN;
		return;	// play again
	case 39:	//Amethyst
		CHECK_GEMS(2);
		player->SetTowerSize(3);
		return;
	case 40:	//Prism
		CHECK_GEMS(2);
		// do nothing
		*sameTurn = SAME_TURN;
		return; // play again
	case 41:	// Gemstone Flaw
		CHECK_GEMS(2);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-3);
			}
		}
		return;
	case 42:	//Spell Weavers
		CHECK_GEMS(2);
		player->SetMagic(1);
		return;
	case 43:	//Ruby
		CHECK_GEMS(3);
		player->SetTowerSize(5);
		return;
	case 44:	//Power Burn
		CHECK_GEMS(3);
		player->SetTowerSize(-5);
		player->SetMagic(2);
		return;
	case 45:	//Solar Flare
		CHECK_GEMS(4);
		player->SetTowerSize(2);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-2);
			}
		}
		return;
	case 46:	//Gem Spear
		CHECK_GEMS(4);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-5);
			}
		}
		return;
	case 47:	//Quarry's Help
		CHECK_GEMS(4);
		player->SetTowerSize(7);
		player->SetBricks(-10);
		return;
	case 48:	//Lodestone
		CHECK_GEMS(5);
		player->SetTowerSize(3);
		return;
	case 49:	//Discord
		CHECK_GEMS(5);
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetTowerSize(-7);
			players[i]->SetMagic(-1);
		}
		return;
	case 50:	//Apprentice
		CHECK_GEMS(5);
		player->SetTowerSize(4);
		player->SetBeasts(-3);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-2);
			}
		}
		return;
	case 51:	//Crystal Matrix
		CHECK_GEMS(6);
		player->SetMagic(1);
		player->SetTowerSize(3);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(1);
			}
		}
		return;
	case 52:	//Emerald
		CHECK_GEMS(6);
		player->SetTowerSize(8);
		return;
	case 53:	//Harmonic Vibe
		CHECK_GEMS(7);
		player->SetMagic(1);
		player->SetTowerSize(3);
		player->SetWallSize(3);
		return;
	case 54:	//Parity
		{
			CHECK_GEMS(7);
			int maxMagic = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(players[i]->GetMagic() > maxMagic)
				{
					maxMagic = players[i]->GetMagic();
				}
			}
			for(int i=0; i<numOfPlayers; ++i)
			{
				players[i]->SetMagic(-players[i]->GetMagic());
				players[i]->SetMagic(maxMagic);
			}
			return;
		}
	case 55:	//Crumblestone
		CHECK_GEMS(7);
		player->SetTowerSize(5);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetBricks(-6);
			}
		}
		return;
	case 56:	//Shatterer
		CHECK_GEMS(8);
		player->SetMagic(-1);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-9);
			}
		}
		return;
	case 57:	//Crystallize
		CHECK_GEMS(8);
		player->SetTowerSize(11);
		player->SetWallSize(-6);
		return;
	case 58:	//Pearl Of Wisdom
		CHECK_GEMS(9);
		player->SetTowerSize(5);
		player->SetMagic(1);
		return;
	case 59:	//Sapphire
		CHECK_GEMS(10);
		player->SetTowerSize(11);
		return;
	case 60:	//Lightning Shard
		{
			CHECK_GEMS(11);
			int maxWall = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer && players[i]->GetWallSize() > maxWall)
				{
					maxWall = players[i]->GetWallSize();
				}
			}
			if(player->GetTowerSize() > maxWall)
			{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->SetTowerSize(-8);
					}
				}
			}
			else
			{
				CorrectGameValues();
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(8);
					}
				}
			}
			return;
		}
	case 61:	//Crystal Shield
		CHECK_GEMS(12);
		player->SetTowerSize(8);
		player->SetWallSize(3);
		return;
	case 62:	//Fire Ruby
		CHECK_GEMS(12);
		player->SetTowerSize(6);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-4);
			}
		}
		return;
	case 63:	//Empathy Gem
		CHECK_GEMS(14);
		player->SetTowerSize(8);
		player->SetDungeon(1);
		return;
	case 64:	//Sanctuary
		CHECK_GEMS(15);
		player->SetTowerSize(10);
		player->SetWallSize(5);
		player->SetBeasts(5);
		return;
	case 65:	//Diamond
		CHECK_GEMS(16);
		player->SetTowerSize(15);
		return;
	case 66:	//Lava Jewel
		CHECK_GEMS(17);
		player->SetTowerSize(12);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(6);
			}
		}
		return;
	case 67:	//Phase Jewel
		CHECK_GEMS(18);
		player->SetTowerSize(13);
		player->SetBricks(6);
		player->SetBeasts(6);
		return;
	case 68:	//Dragon's Eye
		CHECK_GEMS(21);
		player->SetTowerSize(20);
		return;

	//************************************************************
	//--------------------- Green cards: --------------------------
	case 69:	// MadCowDisease
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetBeasts(-6);
		}
		return;
	case 70:	//Full Moon
		for(int i=0; i<numOfPlayers; ++i)
		{
			players[i]->SetDungeon(1);
		}
		player->SetBeasts(3);
		return;
	case 71:	//Faerie
		CHECK_BEASTS(1);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(player != players[i])
			{
				players[i]->Damage(2);
			}
		}
		*sameTurn = SAME_TURN;
		return;		// play again
	case 72:	//Moody Goblins
		CHECK_BEASTS(1);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(player != players[i])
			{
				players[i]->Damage(4);
			}
		}
		player->SetGems(-3);
		return;
	case 73:	//Elven Scout
		CHECK_BEASTS(2);
		// do nothing
		*sameTurn = SAME_TURN;
		return; // play again
	case 74:	//Spearman
		{
			CHECK_BEASTS(2);
			int maxWall = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(players[i]->GetWallSize() > maxWall && i != playingPlayer)
				{
					maxWall = players[i]->GetWallSize();
				}
			}
			if(player->GetWallSize() > maxWall)
			{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(3);
					}
				}

		 	}
			else
			{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(2);
					}
				}
			}
			return;
		}	// end spearman
	case 75:	//Gnome
		CHECK_BEASTS(2);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(3);
			}
		}
		player->SetGems(1);
		return;
	case 76:	//Minotaur
		CHECK_BEASTS(3);
		player->SetDungeon(1);
		return;
	case 77:	//Goblin Mob
		CHECK_BEASTS(3);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(6);
			}
		}
		player->Damage(3);
		return;
	case 78:	//Orc
		CHECK_BEASTS(3);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(5);
			}
		}
		return;
	case 79:	//Goblin Archers
		CHECK_BEASTS(4);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-3);
			}
		}
		player->Damage(1);
		return;
	case 80:	//Berserker
		CHECK_BEASTS(4);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(8);
			}
		}
		player->SetTowerSize(-3);
		return;
	case 81:	//Dwarves
		CHECK_BEASTS(5);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(4);
			}
		}
		player->SetWallSize(3);
		return;
	case 82:	//Slasher
		CHECK_BEASTS(5);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(6);
			}
		}
		return;
	case 83:	//Imp
		CHECK_BEASTS(5);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(6);
				players[i]->SetBricks(-5);
				players[i]->SetGems(-5);
				players[i]->SetBeasts(-5);
			}
		}
		player->SetBricks(-5);
		player->SetGems(-5);
		player->SetBeasts(-5);
		return;
	case 84:	//Shadow Faerie
		CHECK_BEASTS(6);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-2);
			}
		}
		*sameTurn = SAME_TURN;	// play again
		return;
	case 85:	//Little Snakes
		CHECK_BEASTS(6);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-4);
			}
		}
		return;
	case 86:	//Ogre
		CHECK_BEASTS(6);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(7);
			}
		}
		return;
	case 87:	//Rabid Sheep
		CHECK_BEASTS(6);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(6);
				players[i]->SetBeasts(-3);
			}
		}
		return;
	case 88:	//Troll Trainer
		CHECK_BEASTS(7);
		player->SetDungeon(2);
		return;
	case 89:	//Tower Gremlin
		CHECK_BEASTS(8);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(2);
			}
		}
		player->SetWallSize(4);
		player->SetTowerSize(2);
		return;
	case 90:	//Spizzer
		{
			CHECK_BEASTS(8);
			bool zeroWallSizeFlag = false;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer && players[i]->GetWallSize() == 0)
				{
					zeroWallSizeFlag = true;
					break;
				}
			}
			if(zeroWallSizeFlag)
			{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(10);
					}
				}

			}
			else
			{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(6);
					}
				}
			}
			return;
		}
	case 91:	//Werewolf
		CHECK_BEASTS(9);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(9);
			}
		}
		return;
	case 92:	//Unicorn
		{
			CHECK_BEASTS(9);
			bool magicIsHigher = false;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer)
				{
					if(player->GetMagic() > players[i]->GetMagic()){
						magicIsHigher = true;
						break;
					}
				}
			}
			if(magicIsHigher){
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(12);
					}
				}
			}
			else{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(8);
					}
				}

			}
			return;
		}
	case ELVEN_ARCHERS:	// Elven Archers 93
		{
			CHECK_BEASTS(10);
			bool wallIsHigher = false;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer)
				{
					if(player->GetWallSize() > players[i]->GetWallSize()){
						wallIsHigher = true;
						break;
					}
				}
			}
			if(wallIsHigher){
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->SetTowerSize(-6);
					}
				}
			}
			else{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(6);
					}
				}
			}
			return;
		}
	case CORROSION_CLOUD: //94
		{
			CHECK_BEASTS(11);
			bool wallIsHigher = false;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer)
				{
					if(players[i]->GetWallSize() > 0){
						wallIsHigher = true;
						break;
					}
				}
			}
			if(wallIsHigher){
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(10);
					}
				}
			}
			else{
				for(int i=0; i<numOfPlayers; ++i)
				{
					if(i != playingPlayer)
					{
						players[i]->Damage(7);
					}
				}
			}
			return;
		}
	case ROCK_STOMPERS: // 95
		CHECK_BEASTS(11);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(8);
				players[i]->SetQuerry(-1);
			}
		}
		return;
	case THIEF:	// 96
		{
			CHECK_BEASTS(12);
			int maxGems = 0, maxBricks = 0;
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer)
				{
					if(players[i]->GetGems() > maxGems){
						maxGems = players[i]->GetGems();
					}
					if(players[i]->GetBricks() > maxBricks){
						maxBricks = players[i]->GetBricks();
					}
				}
			}
			if(maxGems > 10){
				maxGems = 10;
			}
			if(maxBricks > 5){
				maxBricks = 5;
			}
			for(int i=0; i<numOfPlayers; ++i)
			{
				if(i != playingPlayer)
				{
					players[i]->SetGems(-maxGems);
					players[i]->SetBricks(-maxBricks);
				}
			}
			if(maxGems % 2 == 0){
				player->SetGems(maxGems / 2);
			}
			else{
				player->SetGems(maxGems / 2 + 1);
			}
			if(maxBricks % 2 == 0){
				player->SetBricks(maxBricks / 2);
			}
			else{
				player->SetBricks(maxBricks / 2 + 1);
			}
			return;
		} // end Thief
	case WARLORD: // 97
		CHECK_BEASTS(13);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(13);
			}
		}
		player->SetGems(-3);
		return;
	case SUCCUBUS: // 98
		CHECK_BEASTS(14);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-5);
				players[i]->SetBeasts(-8);
			}
		}
		return;
	case STONE_GIANT: // 99
		CHECK_BEASTS(15);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(10);
			}
		}
		player->SetWallSize(4);
		return;
	case VAMPIRE:	// 100
		CHECK_BEASTS(17);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(10);
				players[i]->SetBeasts(-5);
				players[i]->SetDungeon(-1);
			}
		}
		return;
	case PEGASUS_LANCER:	// 101
		CHECK_BEASTS(18);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->SetTowerSize(-12);
			}
		}
		return;
	case DRAGON:	// 102
		CHECK_BEASTS(25);
		for(int i=0; i<numOfPlayers; ++i)
		{
			if(i != playingPlayer)
			{
				players[i]->Damage(20);
				players[i]->SetGems(-10);
				players[i]->SetDungeon(-1);
			}
		}
		return;
	case CHEAT:	// 666
		{
			player->SetBeasts(100);
			player->SetBricks(100);
			player->SetGems(100);
			player->SetMagic(10);
			player->SetQuerry(10);
			player->SetDungeon(10);
			return;
		}
	case UNDO:	// 999
		RestorePreviousValues();
		*sameTurn = UNDO;
		return;
	case EXIT:	// 222
		exit(0);

	default:
		throw Player::NoSuchCardExists();
	}	// end switch
}

} //SpaceArcomage