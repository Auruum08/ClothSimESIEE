#include "mySphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

mySphere::mySphere(glm::vec3 s_center, float s_radius, glm::vec4 rgba, float s_velocity)
	:center(s_center), radius(s_radius), kd(rgba), velocity(s_velocity)
{
}

mySphere::~mySphere()
{
}

void mySphere::translate(glm::vec3 t)
{
	center += t;
}