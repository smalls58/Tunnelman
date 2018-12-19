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
	for (int i = 0; i<64; i++)
	{
		for (int j = 0; j<61; j++)
		{
			grid[i][j] = 10;
		}
	}
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
				grid[i][j] = TID_EARTH;
			}
		}
	}
	//boulders
	int B = min(int(getLevel() / 2 + 2), 9);

	//gold nuggets
	int G = max(int(5 - getLevel() / 2), 2);

	//barrels of oil
	int L = min(int(2 + getLevel()), 21);
	m_barrelsNeeded = L;

	for (int i = 0; i < B; i++)
	{
		int x;
		int y;
		makeCoordinate(x, y);
		for (int j = x; j < x + 4; j++)
		{
			for (int k = y; k < y + 4; k++)
			{
				if (m_field[j][k] != nullptr)
				{
					delete m_field[j][k];
					m_field[j][k] = nullptr;
				}
			}
		}
		Boulder* boulder = new Boulder(x, y, this);
		m_actor.push_back(boulder);
	}

	for (int i = 0; i < G; i++)
	{
		int x;
		int y;
		makeCoordinate(x, y);
		GoldNugget* gold = new GoldNugget(x, y, this,true,true);
		m_actor.push_back(gold);
	}

	for (int i = 0; i < L; i++)
	{
		int x;
		int y;
		makeCoordinate(x, y);
		BarrelOfOil* barrel = new BarrelOfOil(x, y, this);
		m_actor.push_back(barrel);
	}

	int probabilityOfHardcore = std::min(90, int(getLevel() * 10 + 30));
	if (rand() % 100 > probabilityOfHardcore)
	{
		RegularProtester* protester = new RegularProtester(60,60,this);
		m_actor.push_back(protester);
		m_protestorVec.push_back(protester);
	}
	else {
		HardcoreProtester* protester = new HardcoreProtester(60, 60,this);
		m_actor.push_back(protester);
		m_protestorVec.push_back(protester);
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
	int oil = m_barrelsNeeded;
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
	if (m_barrelsNeeded == 0)
	{
		advanceToNextLevel();
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	m_tunnelman->doSomething();
	
	int G = getLevel() * 25 + 300;

	int itemProb = (int)((double)rand() / (RAND_MAX)* G);

	if (itemProb == 1)
	{
		int x;
		int y;
		makeCoordinate(x, y);
		if (getRandomNum(5) == 1)
		{
			SonarKit* sonar = new SonarKit(x,y,this);
			m_actor.push_back(sonar);
		}
		else
		{
			int x = 0, y = 0;
			WaterPool* pool = new WaterPool(x, y, this);
			m_actor.push_back(pool);
		}
	}

	if (!m_actor.empty())
	{
		vector<Actor*>::iterator i;


		for (i = m_actor.begin(); i != m_actor.end(); i++)
		{
			if ((*i)->isAlive())
			{
				(*i)->doSomething();
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
		playSound(SOUND_PLAYER_GIVE_UP);
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
void StudentWorld::setGridContent(int x, int y, int ID)
{
	grid[x][y] = ID;
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
				grid[i][j] = 10;
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
		if (x + 1 <= 60 && this->getContentsOf(x + 1, y) == 10)
		{
			temp = new WaterSquirt(x+1, y, this, dir);
			m_actor.push_back(temp);
		}
	}
	else if (dir == 3)
	{
		if (x - 1 >= 0 && this->getContentsOf(x - 1, y) == 10)
		{
			temp = new WaterSquirt(x - 1, y, this, dir);
			m_actor.push_back(temp);
		}
	}
	else if (dir == 1)
	{
		if (y + 1 <= 60 && this->getContentsOf(x, y + 1) == 10)
		{
			temp = new WaterSquirt(x, y+1, this, dir);
			m_actor.push_back(temp);
		}
	}
	else if (dir == 2)
	{
		if (y - 1 >= 0 && this->getContentsOf(x, y - 1) == 10)
		{
			temp = new WaterSquirt(x, y - 1, this, dir);
			m_actor.push_back(temp);
		}
	}

	this->playSound(SOUND_PLAYER_SQUIRT);
	m_tunnelman->decrementAmmo();
}
TunnelMan* StudentWorld::getTunnelMan()const
{
	return m_tunnelman;
}
std::vector<Protester*> StudentWorld::getProtestorVec()
{
	return m_protestorVec;
}
void StudentWorld::decrementBarrelsNeeded()
{
	if (m_barrelsNeeded > 0)
	{
		m_barrelsNeeded--;
	}
}
int StudentWorld::getRandomNum(int max)const
{
	return int(rand() % max);
}
void StudentWorld::makeCoordinate(int &x, int&y)
{
	int actorX;
	int actorY;
	double xSide;
	double ySide;
	double radius;
	bool done = false;

	while (!done) 
	{
		x = getRandomNum(59) + 1;
		y = getRandomNum(59) + 1;
		if (m_actor.size() == 0)
		{
			done = true;
		}
		for (size_t i = 0; i < m_actor.size(); i++)
		{
			actorX = m_actor[i]->getX();
			actorY = m_actor[i]->getY();
			xSide = x - actorX;
			ySide = y - actorY;
			radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));
			if (radius <= 4.0)
			{
				done = false;
				break;
			}
			done = true;
		}
	}
}
bool StudentWorld::isBoulder(int x, int y)const
{
	int boulderX;
	int boulderY;
	double xSide;
	double ySide;
	double radius=4;
	for (size_t i = 0; i < m_actor.size(); i++)
	{
		if (m_actor[i]->getID() == TID_BOULDER)
		{

			boulderX = m_actor[i]->getX();
			boulderY = m_actor[i]->getY();
			xSide = x - boulderX;
			ySide = y - boulderY;
			radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));

			if (radius <= 3)
			{
				return true;
			}
		}
	}
	return false;
}
void StudentWorld::revealSonar(int x, int y)
{
	int goodieX;
	int goodieY;
	double xSide;
	double ySide;
	double radius;
	for (size_t i = 0; i < m_actor.size(); i++)
	{
		if (m_actor[i]->getID() == TID_BARREL || m_actor[i]->getID() == TID_GOLD)
		{

			goodieX = m_actor[i]->getX();
			goodieY = m_actor[i]->getY();
			xSide = x - goodieX;
			ySide = y - goodieY;
			radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));

			if (radius <= 12)
			{
				m_actor[i]->setVisible(true);
			}
		}
	}
}
int StudentWorld::getTicks()const
{
	return ticks_elapsed;
}
class QItem {
public:
	int row;
	int col;
	string dir;
	QItem(int x, int y, string w)
		: row(x), col(y), dir(w)
	{
	}
};
string StudentWorld::shortestPath(int startX, int startY, int endX, int endY)const
{
	QItem source(0, 0, "");

	// To keep track of visited QItems. Marking 
	// blocked cells as visited. 
	bool visited[61][61];
	for (int i = 0; i < 61; i++) {
		for (int j = 0; j < 61; j++)
		{
			if (grid[i][j] == 10)
				visited[i][j] = true;
			else
				visited[i][j] = false;
		}
	}
	source.row = startX;
	source.col = startY;

	// applying BFS on matrix cells starting from source 
	queue<QItem> q;
	q.push(source);
	visited[source.row][source.col] = true;
	while (!q.empty()) {
		QItem p = q.front();
		q.pop();

		// Destination found; 
		if (p.row==endX && p.col == endY)
			return p.dir;

		// moving up 
		if (p.row - 1 >= 0 &&
			visited[p.row - 1][p.col] == false) {
			q.push(QItem(p.row - 1, p.col, p.dir + "1"));
			visited[p.row - 1][p.col] = true;
		}

		// moving down 
		if (p.row + 1 < 61 &&
			visited[p.row + 1][p.col] == false) {
			q.push(QItem(p.row + 1, p.col, p.dir + "2"));
			visited[p.row + 1][p.col] = true;
		}

		// moving left 
		if (p.col - 1 >= 0 &&
			visited[p.row][p.col - 1] == false) {
			q.push(QItem(p.row, p.col - 1, p.dir + "3"));
			visited[p.row][p.col - 1] = true;
		}

		// moving right 
		if (p.col + 1 < 61 &&
			visited[p.row][p.col + 1] == false) {
			q.push(QItem(p.row, p.col + 1, p.dir + "4"));
			visited[p.row][p.col + 1] = true;
		}
	}
	return "0";
}