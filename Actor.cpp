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

			if (getY() <60 && getDirection()==up)
			{
				moveTo(getX(), getY() + 1);
				getWorld()->removeDirt(getX(), getY());
			}
			else
			{
				setDirection(up);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getY() >0 && getDirection() == down)
			{
				moveTo(getX(), getY() - 1);
				getWorld()->removeDirt(getX(), getY());
			}
			else
			{
				setDirection(down);
			}
			break;
		case KEY_PRESS_LEFT:
			if (getX() >0 && getDirection() == left)
			{
				moveTo(getX() - 1, getY());
				getWorld()->removeDirt(getX(), getY());
			}
			else
			{
				setDirection(left);
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getX() <60 && getDirection() == right)
			{
				moveTo(getX() + 1, getY());
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
			shootWater(getDirection(), getX(), getY());
			break;
		}
		case 'Z':
		case 'z':
		{
			if (m_sonarCharge <= 0)
				break;
			//getWorld()->revealSonar(getX(), getY());
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
void TunnelMan::shootWater(Direction dir, int x, int y)
{
	if (getAmmo() <= 0)
	{
		return;
	}

	if (dir == right)
	{
		if (x + 4 <= 60 && getWorld()->getContentsOf(x+4,y)==0)
		{
			getWorld()->setObject(x + 4, y, TID_WATER_SPURT);
		}
	}
	else if (dir == left)
	{
		if (x - 4 >= 0 && getWorld()->getContentsOf(x - 4, y) == 0)
		{
			getWorld()->setObject(x - 4, y, TID_WATER_SPURT);
		}
	}
	else if (dir == up)
	{
		if (y + 4 <= 60 && getWorld()->getContentsOf(x, y+4) == 0)
		{
			getWorld()->setObject(x, y+4, TID_WATER_SPURT);
		}
	}
	else if (dir == down)
	{
		if (y - 4 >= 0 && getWorld()->getContentsOf(x, y - 4) == 0)
		{
			getWorld()->setObject(x, y - 4, TID_WATER_SPURT);
		}
	}

	getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}
WaterSquirt::WaterSquirt(int x, int y, StudentWorld * world, Direction dir) :
	Actor(TID_WATER_SPURT, x, y, world, dir, 1.0, 1)
{

}