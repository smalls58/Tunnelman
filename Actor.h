#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm> 
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
private:
	bool m_alive;
	StudentWorld* worldPtr;
public:
	Actor(int imageID, int x, int y, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0);
	virtual ~Actor() {}
	void virtual doSomething() = 0;
	bool isAlive()const { return m_alive; }
	void setDead() { m_alive = false; }
	StudentWorld* getWorld() { return worldPtr; }
};

class Earth: public Actor
{
public:
	Earth(int x, int y, StudentWorld* world);
	virtual ~Earth() {}
	void virtual doSomething() {}
};

class Boulder : public Actor
{
public:
	Boulder(int x, int y, StudentWorld* world);
	virtual ~Boulder() {}
	void virtual doSomething() {}
};

class TunnelMan : public Actor
{
private:
	int m_health;
	int m_ammo;
	int m_sonarCharge;
	int m_goldNuggets;
public:
	TunnelMan(StudentWorld* world);
	virtual ~TunnelMan() {}
	void virtual doSomething();
	int getHealth()const;
	int getAmmo()const;
	int getSonar()const;
	int getGold()const;
	void decrementAmmo();
	void incrementAmmo();
};
class WaterSquirt : public Actor {
private:
	int m_ticksLeft;
public:
	WaterSquirt(int x, int y, StudentWorld * world, Direction dir);
	virtual ~WaterSquirt() {}
	void virtual doSomething();
};
class Goodie : public Actor {
public:
	Goodie(int imageID, int x, int y, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0);
	virtual ~Goodie() {}
	void virtual doSomething(){}
};
class WaterPool : public Goodie {
private:
	int m_ticksLeft;
public:
	WaterPool(int x, int y, StudentWorld * world);
	virtual ~WaterPool() {}
	void virtual doSomething();
};
class RegularProtester : public Actor {
private:
	int m_health;
	bool m_leaveField;
	int ticksToWaitBetweenMoves;
public:
	RegularProtester(int x, int y, StudentWorld* world);
	virtual ~RegularProtester() {}
	void virtual doSomething();
	int numSquaresToMoveInCurrentDirection()const;
};

#endif // ACTOR_H_
