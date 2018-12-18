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
private:
	int tickTime;
	//0=stable, 1=waiting, 2=falling 
	int m_fallState;
public:
	Boulder(int x, int y, StudentWorld* world);
	virtual ~Boulder() {}
	void virtual doSomething();
	bool dirtBelow();
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
	void incrementSonar();
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
class BarrelOfOil : public Goodie {
public:
	BarrelOfOil(int x, int y, StudentWorld * world);
	virtual ~BarrelOfOil() {}
	void virtual doSomething();
};
class GoldNugget : public Goodie
{
public:
	GoldNugget(int x, int y, StudentWorld * world);
	virtual ~GoldNugget() {}
	void virtual doSomething();
};
class SonarKit : public Goodie {
private:
	int m_ticksLeft;
public:
	SonarKit(int x, int y, StudentWorld * world);
	virtual ~SonarKit() {}
	void virtual doSomething();
};
class Protester : public Actor {
private:
	int m_health;
	bool m_leaveField;
	int m_ticksToWaitBetweenMoves;
	int m_annoyance;
public:
	Protester(int imageID, int x, int y, StudentWorld* world, int hitPoints);
	virtual ~Protester() {}
	void virtual doSomething() {}
	int numSquaresToMoveInCurrentDirection()const;
	void setAnnoyance(int annoy);
};
class RegularProtester : public Protester {
private:
	int m_health;
	bool m_leaveField;
public:
	RegularProtester(int x, int y, StudentWorld* world);
	virtual ~RegularProtester() {}
	void virtual doSomething();
};

#endif // ACTOR_H_
