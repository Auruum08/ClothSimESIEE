#pragma once
#include <glm/glm.hpp>
#include "configuration.h"

class myParticle
{
public:
	glm::vec3 old_position;
	glm::vec3 position;

	glm::vec3 velocity;
	glm::vec3 old_velocity;

	glm::vec3 acceleration;
	glm::vec3 old_acceleration;

	float mass;

	glm::vec3 force;

	glm::vec3 normal;

	bool movable;

	myParticle();
	myParticle(float x1, float y1, float z1, float inputmass = DEFAULT_MASS);
	~myParticle();

	void addForce(glm::vec3 f);
	void calculateNextPosition(); 

	// fonctions nat
	glm::vec3 getNextPosition();
	void computeNewVelocity();
};

