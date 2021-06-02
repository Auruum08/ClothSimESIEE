#include "mySphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "bvh.h"

mySphere::mySphere(glm::vec3 s_center, float s_radius, glm::vec4 rgba, float s_velocity)
	:center(s_center), radius(s_radius), kd(rgba), velocity(s_velocity)
{
	Aabb box(center);
}

mySphere::~mySphere()
{
}

void mySphere::translate(glm::vec3 t)
{
	center += t;

	// nat à enlever ( tout) (uniquement pour afficher le test )
	box.p1 = center - vec3(radius/2,radius/2,radius/2);
	box.p2 = center + vec3(radius/2,radius/2,radius/2);
	box.updatePos();
}