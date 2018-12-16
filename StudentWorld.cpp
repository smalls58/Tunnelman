#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
int StudentWorld::init()
{
	ticks_elapsed = 0;
	barrels_collected = 0;
	for (int i = 0; i<64; i++)
	{
		for (int j = 0; j<61; j++)
		{
			grid[i][j] = 0;
		}
	}
	barrels_collected = 0;
	m_tunnelman = new TunnelMan(this);

	//Initializing Earth
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			//Shaft
			if ((i >= 30 && i <= 33) && (j >= 4 && j <= 59))
			{

			}
			else {
				m_field[i][j] = new Earth(i, j,this);
				grid[i][j] = 1;
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

void StudentWorld::textDisplay()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = m_tunnelman->getHealth() * 10;
	int gold = m_tunnelman->getGold();
	int squirts = m_tunnelman->getAmmo();
	int sonar = m_tunnelman->getSonar();

	//change oil number
	int oil = 0;
	ostringstream oss;
	oss.setf(ios::fixed);
	// “Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2”
	oss << "Scr: " << setw(8) << setfill('0') << score << "  Lives " << lives << "  Lvl: " 
		<< setw(2) << setfill(' ') << level << "  Hlth: " << setw(3) << setfill(' ') << health 
		<< "%  Water:" << setw(2) << setfill(' ') << squirts << "  Gld: " << setw(2) << setfill(' ') 
		<< gold << "  Sonar: " << setw(2) << setfill(' ') << sonar << "  Oil Left" << setw(2) << setfill(' ') << oil;
	string text;
	text = oss.str();
	GameWorld::setGameStatText(text);
}
int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	ticks_elapsed++;
	StudentWorld::textDisplay();
	m_tunnelman->doSomething();

	if (!m_actor.empty())
	{
		vector<Actor*>::iterator i;


		for (i = m_actor.begin(); i != m_actor.end(); i++)
		{
			if ((*i)->isAlive())
			{
				(*i)->doSomething();
				//If tunnelman dies
			}
		}

		//Remove dead objects
		for (i = m_actor.begin(); i != m_actor.end();)
		{
			if (!(*i)->isAlive())
			{
				delete *i;
				i = m_actor.erase(i);
			}
			else i++;
		}
	}

	if (!m_tunnelman->isAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::getContentsOf(int x, int y)
{
	return grid[x][y];
}
StudentWorld::~StudentWorld() {
	vector<Actor*>::iterator i;

	for (i = m_actor.begin(); i != m_actor.end();)
	{
		delete *i;
		i = m_actor.erase(i);
	}
	for (int k = 0; k<64; k++)
	{
		for (int l = 0; l<60; l++)
		{
			delete m_field[k][l];
		}
	}

	delete m_tunnelman;
}
void StudentWorld::cleanUp() {
	vector<Actor*>::iterator i;
	for (i = m_actor.begin(); i != m_actor.end();)
	{
		delete *i;
		i = m_actor.erase(i);
	}
	for (int k = 0; k<64; k++)
	{
		for (int l = 0; l<60; l++)
		{
			delete m_field[k][l];
		}
	}

	delete m_tunnelman;
}
void StudentWorld::removeDirt(int x, int y)
{

	for (int i = x; i<x + 4; i++)
	{
		for (int j = y; j<y + 4; j++)
		{
			if (m_field[i][j] != nullptr)
			{
				delete m_field[i][j];
				m_field[i][j] = nullptr;
				grid[i][j] = 0;
				playSound(SOUND_DIG);
			}
		}
	} 
}
void StudentWorld::shootWater(int x, int y)
{
	if (m_tunnelman->getAmmo() <= 0)
	{
		return;
	}
	enum GraphObject::Direction dir = m_tunnelman->getDirection();
	WaterSquirt *temp;
	if (dir == 4)
	{
		if (x + 1 <= 60 && this->getContentsOf(x + 1, y) == 0)
		{
			temp = new WaterSquirt(x+1, y, this, dir);
			m_actor.push_back(temp);
		}
	}
	else if (dir == 3)
	{
		if (x - 1 >= 0 && this->getContentsOf(x - 1, y) == 0)
		{
			temp = new WaterSquirt(x - 1, y, this, dir);
			m_actor.push_back(temp);
		}
	}
	else if (dir == 1)
	{
		if (y + 1 <= 60 && this->getContentsOf(x, y + 1) == 0)
		{
			temp = new WaterSquirt(x, y+1, this, dir);
			m_actor.push_back(temp);
		}
	}
	else if (dir == 2)
	{
		if (y - 1 >= 0 && this->getContentsOf(x, y - 1) == 0)
		{
			temp = new WaterSquirt(x, y - 1, this, dir);
			m_actor.push_back(temp);
		}
	}

	this->playSound(SOUND_PLAYER_SQUIRT);
	m_tunnelman->decrementAmmo();
}