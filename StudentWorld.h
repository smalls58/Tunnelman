#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void textDisplay();

	int getContentsOf(int x, int y);

	void setObject(int x, int y, int ID);

	void removeDirt(int a, int b);

	~StudentWorld();

private:
	std::vector<Actor*> m_actor;
	TunnelMan* m_tunnelman;
	Actor* m_field[64][64];
	int grid[64][64];
	int barrels_collected;
	int ticks_elapsed;
};

#endif // STUDENTWORLD_H_
