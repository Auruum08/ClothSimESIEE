#pragma once
#include <glm/glm.hpp>
#include "configuration.h"
#include "myParticle.h"

class Wind {
public:	
	glm::vec3 speed;
	float wcount, angle, amplitude;
	double rho, drag_coeff, lift_coeff;

	Wind();
	void updateSpeed();
	glm::vec3 basicForce(myParticle p);
	glm::vec3 disneyForce(myParticle p);

private:
	void move();
};