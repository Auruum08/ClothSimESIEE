#include "mySpring.h"
#include <glm/glm.hpp>
#include <iostream>
#include <SOIL/SOIL.h>
mySpring::mySpring()
{ 

}

mySpring::mySpring(myParticle *x1, myParticle *x2, float spring, float springl)
{
	restLength = springl;
	p1 = x1;
	p2 = x2;
	springConstant = spring;
	
}

void mySpring::addForce()
{
	glm::vec3 dist = p1->position - p2->position;
	glm::vec3 delta_v = p1->velocity - p2->velocity;
	glm::vec3 dir = glm::normalize(dist);

	
	glm::vec3 springforce = springConstant * (glm::length(dist) - restLength) * dir;
	
	//glm::vec3 damping = -(delta_v * dot((float)M_KD),;

	/*damping produit scalaire*/
	//glm::vec3 damping =- glm::dot(M_KD_1*delta_v, dir)*dir;

	/*damping smooth*/
	glm::vec3 damping = -(float)M_KD_2 * delta_v;
	

	/*ajustement de la longueur des ressorts*/
	glm::vec3 erreur = dir * (float)(glm::length(dist) - restLength);


	/*
		ici, la contrainte est la longueur d'un ressort.
		mais peut-être faut-il traiter la contraite comme étant la position d'une 
	*/
	if ((glm::length(erreur)) > DEF_RATE * restLength) {
		if (p1->movable) {
			if (p2->movable) {
				p1->position -= 0.5f * erreur;
				p2->position += 0.5f * erreur;
			}
			else {
				p1->position -= erreur;
			}
		}
		else {
			if (p2->movable) {
				p2->position += erreur;
			}
		}
	}
	
	
	p2->addForce(springforce);
	p2->addForce(-damping);
	p1->addForce(-springforce);
	p1->addForce(damping);
		
	


		
	
}

mySpring::~mySpring()
{
}
