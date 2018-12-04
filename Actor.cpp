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

Boulder::Boulder(int x, int y, StudentWorld* world):
	Actor(TID_BOULDER,x,y,world,down,1.0,1)
{

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

	int ch;

	if (getWorld()->getKey(ch))
	{
		int a;

		switch (ch)
		{
		case KEY_PRESS_LEFT:
			a = getWorld()->getContentsOf(getX() - 1, getY());
			if (getX() - 1 > 0 && a < 2)
				setDirection(left);
				moveTo(getX() - 1, getY());
			break;

		case KEY_PRESS_RIGHT:
			a = getWorld()->getContentsOf(getX() + 1, getY());
			if (getX() < 60 && a < 2)
				setDirection(right);
				moveTo(getX() + 1, getY());
			break;

		case KEY_PRESS_UP:
			a = getWorld()->getContentsOf(getX(), getY() + 1);
			if (getY() + 1 < 60 && a < 2)
				setDirection(up);
				moveTo(getX(), getY() + 1);
			break;

		case KEY_PRESS_DOWN:
			a = getWorld()->getContentsOf(getX(), getY() - 1);
			if (getY() - 1 > 0 && a < 2)
				setDirection(down);
				moveTo(getX(), getY() - 1);
			break;

		default:
			break;
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