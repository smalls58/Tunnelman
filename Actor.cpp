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