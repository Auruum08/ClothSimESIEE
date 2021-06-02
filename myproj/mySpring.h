#pragma once
#include "myParticle.h"


class mySpring
{
public:
	float restLength;
	float springConstant;
	
	glm::vec3 dist;
	glm::vec3 dir;

	myParticle *p1;
	myParticle *p2;

	mySpring();
	mySpring(myParticle *x1, myParticle *x2, float spring = DEFAULT_K, float springl = SPACE_P1_TO_P2);
	~mySpring();

	void mySpring::addForce();
	float mySpring::getConstraint();
	void mySpring::corrigepos();
};

