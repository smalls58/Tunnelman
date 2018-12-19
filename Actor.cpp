#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int x, int y, StudentWorld* world, Direction dir, double size, unsigned int depth):
	GraphObject(imageID,x,y,dir,size,depth),m_alive(true),worldPtr(world)
{
	setVisible(true);
}

Earth::Earth(int x, int y, StudentWorld* world):
	Actor(TID_EARTH,x,y,world,right,.25,3)
{

}
Boulder::Boulder(int x, int y, StudentWorld* world) :
	Actor(TID_BOULDER, x, y, world, down, 1.0, 1),m_fallState(0)
{

}

void Boulder::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	//0=stable, 1=waiting, 2=falling 
	if (dirtBelow() && m_fallState==0)
	{
		return;
	}
	else if (m_fallState == 0)
	{
		m_fallState = 1;
	}
	else if (m_fallState == 1)
	{
		tickTime++;
		if (tickTime >= 30)
		{
			getWorld()->playSound(SOUND_FALLING_ROCK);
			m_fallState = 2;
		}
	}
	else if (m_fallState == 2)
	{
		int protesterX;
		int protesterY;
		int tunnelManX=getWorld()->getTunnelMan()->getX();
		int tunnelManY=getWorld()->getTunnelMan()->getY();
		double xSide;
		double ySide;
		double radius;
		for (size_t i = 0; i < getWorld()->getProtestorVec().size(); i++)
		{
			protesterX = getWorld()->getProtestorVec().at(i)->getX();
			protesterY = getWorld()->getProtestorVec().at(i)->getY();
			xSide = getX() - protesterX;
			ySide = getY() - protesterY;
			radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));

			if (radius <= 3)
			{
				getWorld()->getProtestorVec().at(i)->setAnnoyance(100);
				getWorld()->increaseScore(500);
			}
		}
		xSide = getX() - tunnelManX;
		ySide = getY() - tunnelManY;
		radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));
		if (radius <= 3)
		{
			getWorld()->getTunnelMan()->setDead();
		}

		if (getY() == 0 || getWorld()->getContentsOf(getX(), getY() - 1) == TID_EARTH || getWorld()->isBoulder(getX(),getY()-4))
		{
			setDead();
		}
		else
		{
			moveTo(getX(), getY() - 1);
		}
	}

}
bool Boulder::dirtBelow()
{
	for (int x = getX(); x<getX() + 4; x++)
	{
		if (getWorld()->getContentsOf(x, getY() - 1) == TID_EARTH)
		{
			return true;
		}
	}

	return false;
}
TunnelMan::TunnelMan(StudentWorld* world):
	Actor(TID_PLAYER,30,60,world),m_health(10),m_ammo(5),m_sonarCharge(1),m_goldNuggets(0)
{

}
void TunnelMan::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	if (!isAlive())
	{
		return;
	}

	int KeyPressed = -1;
	if (getWorld()->getKey(KeyPressed))
	{
		switch (KeyPressed)
		{
		case  KEY_PRESS_UP:

			if (getY() <60 && getDirection()==up && !getWorld()->isBoulder(getX(),getY()+1))
			{
				moveTo(getX(), getY() + 1);
				//getWorld()->setGridContent(getX(), getY(), 10);
				//getWorld()->setGridContent(getX(), getY()+1, TID_PLAYER);
				getWorld()->removeDirt(getX(), getY());
			}
			else
			{
				setDirection(up);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getY() >0 && getDirection() == down && !getWorld()->isBoulder(getX(), getY() - 1))
			{
				moveTo(getX(), getY() - 1);
				//getWorld()->setGridContent(getX(), getY(), 10);
				//getWorld()->setGridContent(getX(), getY()-1, TID_PLAYER);
				getWorld()->removeDirt(getX(), getY());
			}
			else
			{
				setDirection(down);
			}
			break;
		case KEY_PRESS_LEFT:
			if (getX() >0 && getDirection() == left && !getWorld()->isBoulder(getX()-1, getY()))
			{
				moveTo(getX() - 1, getY());
				//getWorld()->setGridContent(getX(), getY(), 10);
				//getWorld()->setGridContent(getX()-1, getY(), TID_PLAYER);
				getWorld()->removeDirt(getX(), getY());
			}
			else
			{
				setDirection(left);
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getX() <60 && getDirection() == right && !getWorld()->isBoulder(getX()+1, getY()))
			{
				moveTo(getX() + 1, getY());
				//getWorld()->setGridContent(getX(), getY(), 10);
				//getWorld()->setGridContent(getX()+1, getY(), TID_PLAYER);
				getWorld()->removeDirt(getX(), getY());
			}
			else
			{
				setDirection(right);
			}
			break;
		case KEY_PRESS_ESCAPE:
			setDead();
			//kill player
			break;
		case KEY_PRESS_SPACE:
		{
			getWorld()->shootWater(getX(), getY());
			break;
		}
		case 'Z':
		case 'z':
		{
			if (m_sonarCharge <= 0)
				break;
			getWorld()->revealSonar(getX(), getY());
			m_sonarCharge--;
		}

		}
	}

}
int TunnelMan::getHealth()const
{
	return m_health;
}
int TunnelMan::getAmmo()const
{
	return m_ammo;
}
int TunnelMan::getSonar()const
{
	return m_sonarCharge;
}
int TunnelMan::getGold()const
{
	return m_goldNuggets;
}
void TunnelMan::decrementAmmo()
{
	if (m_ammo > 0)
	{
		m_ammo--;
	}
}
void TunnelMan::incrementAmmo()
{
	m_ammo += 5;
}
void TunnelMan::incrementSonar()
{
	m_sonarCharge++;
}
void TunnelMan::incrementGold()
{
	m_goldNuggets++;
}
void TunnelMan::decrementGold()
{
	m_goldNuggets--;
}
WaterSquirt::WaterSquirt(int x, int y, StudentWorld * world, Direction dir) :
	Actor(TID_WATER_SPURT, x, y, world, dir, 1.0, 1),m_ticksLeft(0)
{

}
void WaterSquirt::doSomething()
{
	m_ticksLeft++;
	int x = getX();
	int y = getY();
	if (m_ticksLeft == 8)
	{
		setDead();
	}
	else if (getDirection() == right)
	{
		if (x + 1 <= 60 && getWorld()->getContentsOf(x + 1, y) == 10)
		{
			moveTo(x + 1, y);
		}
		else
		{
			setDead();
		}
	}
	else if (getDirection() == left)
	{
		if (x - 1 >= 0 && getWorld()->getContentsOf(x - 1, y) == 10)
		{
			moveTo(x - 1, y);
		}
		else
		{
			setDead();
		}
	}
	else if (getDirection() == up)
	{
		if (y + 1 <= 60 && getWorld()->getContentsOf(x, y + 1) == 10)
		{
			moveTo(x, y+1);
		}
		else
		{
			setDead();
		}
	}
	else if (getDirection() == down)
	{
		if (y - 1 >= 0 && getWorld()->getContentsOf(x, y - 1) == 10)
		{
			moveTo(x, y-1);
		}
		else
		{
			setDead();
		}
	}

}
Goodie::Goodie(int imageID, int x, int y, StudentWorld * world, Direction dir, double size, unsigned int depth) :
	Actor(imageID, x, y, world, dir, size, depth)
{

}
WaterPool::WaterPool(int x, int y, StudentWorld * world):
	Goodie(TID_WATER_POOL, x, y, world, right, 1.0, 2),m_ticksLeft(std::max(100, int(300 - 10 * world->getLevel())))
{

}
void WaterPool::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	m_ticksLeft--;
	if (m_ticksLeft == 0)
	{
		setDead();
	}

	int tunnelManX = getWorld()->getTunnelMan()->getX();
	int tunnelManY = getWorld()->getTunnelMan()->getY();
	double xSide = getX() - tunnelManX;
	double ySide = getY() - tunnelManY;
	double radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));
	if (radius <= 3)
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getTunnelMan()->incrementAmmo();
		getWorld()->increaseScore(500);
	}
}
BarrelOfOil::BarrelOfOil(int x, int y, StudentWorld * world):
	Goodie(TID_BARREL, x, y, world, right, 1.0, 2)
{
	setVisible(false);
}
void BarrelOfOil::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	int tunnelManX = getWorld()->getTunnelMan()->getX();
	int tunnelManY = getWorld()->getTunnelMan()->getY();
	double xSide = getX() - tunnelManX;
	double ySide = getY() - tunnelManY;
	double radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));

	if (!isVisible() && radius <= 4)
	{
		setVisible(true);
		return;
	}

	if (radius <= 3)
	{
		setDead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decrementBarrelsNeeded();
	}
}
GoldNugget::GoldNugget(int x, int y, StudentWorld *world, bool pickState, bool permState) :
	Goodie(TID_GOLD, x, y, world, right, 1.0, 2), m_ticksLeft(std::max(100, int(300 - 10 * world->getLevel()))),m_pickableState(pickState),m_permanentState(permState)
{
	setVisible(false);
}
void GoldNugget::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	if (!m_permanentState)
	{
		m_ticksLeft--;
		if (m_ticksLeft <= 0)
		{
			setDead();
		}
	}

	int tunnelManX = getWorld()->getTunnelMan()->getX();
	int tunnelManY = getWorld()->getTunnelMan()->getY();
	double xSide = getX() - tunnelManX;
	double ySide = getY() - tunnelManY;
	double radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));
	if (radius <= 4 && !isVisible())
	{
		setVisible(true);
		return;
	}

	if (radius <= 3 && m_pickableState)
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->getTunnelMan()->incrementGold();
	}

	for (size_t i = 0; i < getWorld()->getProtestorVec().size(); i++)
	{
		xSide = getX() - getWorld()->getProtestorVec().at(i)->getX();
		ySide = getY() - getWorld()->getProtestorVec().at(i)->getY();
		radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));

		if (radius <= 3 && !m_pickableState)
		{
			setDead();
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			getWorld()->getProtestorVec().at(i)->setBribe(true);
			getWorld()->increaseScore(25);
			return;
		}
	}
}
bool GoldNugget::getPickableState()const
{
	return m_pickableState;
}
void GoldNugget::setPickableState(bool state)
{
	m_pickableState = state;
}
bool GoldNugget::getPermanentState()const
{
	return m_permanentState;
}
void GoldNugget::setPermanentState(bool permState)
{
	m_permanentState = permState;
}
SonarKit::SonarKit(int x, int y, StudentWorld * world) :
	Goodie(TID_SONAR,x,y,world,right,1.0,2),m_ticksLeft(std::max(100, int(300 - 10 * world->getLevel())))
{

}
void SonarKit::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	m_ticksLeft--;
	if (m_ticksLeft <= 0)
	{
		setDead();
	}

	int tunnelManX = getWorld()->getTunnelMan()->getX();
	int tunnelManY = getWorld()->getTunnelMan()->getY();
	double xSide = getX() - tunnelManX;
	double ySide = getY() - tunnelManY;
	double radius = sqrt(pow(xSide, 2.0) + pow(ySide, 2.0));

	if (radius <= 3)
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(75);
		getWorld()->getTunnelMan()->incrementSonar();
	}
}
Protester::Protester(int imageID, int x, int y, StudentWorld* world, int hitPoints) :
	Actor(imageID, x, y, world, left, 1.0, 0),m_health(hitPoints),m_leaveField(false),
	m_ticksToWaitBetweenMoves(std::max(0, int(3 - world->getLevel() / 4))),m_annoyance(0),m_bribed(false)
{

}
int Protester::numSquaresToMoveInCurrentDirection()const
{
	return (int)(rand() % 53 + 8);
}
void Protester::setAnnoyance(int annoy)
{
	m_annoyance = annoy;
}
void Protester::setBribe(bool bribed)
{
	m_bribed = bribed;
}
void Protester::setHealth(int health)
{
	m_health = health;
}
void Protester::setFieldStatus(bool leave)
{
	m_leaveField = leave;
}
int Protester::getHealth()const
{
	return m_health;
}
bool Protester::getFieldStatus()const
{
	return m_leaveField;
}
int Protester::getTicksToWait()const
{
	return m_ticksToWaitBetweenMoves;
}
int Protester::getAnnoyance()const
{
	return m_annoyance;
}
RegularProtester::RegularProtester(int x, int y, StudentWorld* world):
	Protester(TID_PROTESTER, x, y, world, 5)
{

}
void RegularProtester::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	if (getWorld()->getTicks()%getTicksToWait() != 0)
	{
		return;
	}

	if (getAnnoyance() >= 100)
	{
		setFieldStatus(true);
	}
	if (getX() == 60 && getY() == 60 && getFieldStatus())
	{
		setDead();
	}

	std::string listDirections;
	if (getFieldStatus())
	{
		listDirections = getWorld()->shortestPath(getX(), getY(), 60, 60);
	}
	else
	{
		listDirections = getWorld()->shortestPath(getX(), getY(), getWorld()->getTunnelMan()->getX(), getWorld()->getTunnelMan()->getY());
	}

	char KeyPressed = listDirections[0];
	switch (KeyPressed)
	{
	case  '1':

		if (getY() < 60 && getDirection() == up && !getWorld()->isBoulder(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
		}
		else
		{
			setDirection(up);
		}
		break;
	case '2':
		if (getY() > 0 && getDirection() == down && !getWorld()->isBoulder(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
		}
		else
		{
			setDirection(down);
		}
		break;
	case '3':
		if (getX() > 0 && getDirection() == left && !getWorld()->isBoulder(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
		}
		else
		{
			setDirection(left);
		}
		break;
	case '4':
		if (getX() < 60 && getDirection() == right && !getWorld()->isBoulder(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
		}
		else
		{
			setDirection(right);
		}
		break;
	}



}
HardcoreProtester::HardcoreProtester(int x, int y, StudentWorld* world):
	Protester(TID_HARD_CORE_PROTESTER, x, y, world, 20)
{

}
void HardcoreProtester::doSomething()
{

}