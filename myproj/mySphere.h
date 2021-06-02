#pragma once
#include <glm/glm.hpp>
#include "bvh.h"
class mySphere
{
public:
	glm::vec3 center;
	float radius;
	glm::vec4 kd;
	float velocity;

	// nat
	Aabb box;


	void translate(glm::vec3);

	mySphere(glm::vec3 s_center, float s_radius, glm::vec4 rgba, float);
	~mySphere();
};

