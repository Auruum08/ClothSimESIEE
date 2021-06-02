#pragma once
#include "Wind.h"
#include <glm/glm.hpp>
#include "configuration.h"
#include "PerlinNoise.hpp"
#include "myParticle.h"


Wind::Wind(){
	speed = glm::vec3(5.0f,0.0f,0.0f);
	amplitude = 3.0f;

	rho = 1.225;
	drag_coeff = 0.06; //0.06;//0.1
	lift_coeff = 0.01;	  //0.01;//0
}
void Wind::updateSpeed() {
	move();
	speed = (amplitude) * glm::vec3(cos(angle), 0.0f, sin(angle));
}
void Wind::move() { // pour avoir un vent qui tourne de manière un peu naturelle

	siv::PerlinNoise noise;
	wcount += 0.00006f;
	angle = 3.14 *noise.noise1D(wcount);

	//amplitude = 30.0f * noise.noise1D(wcount);
}
glm::vec3 Wind::disneyForce(myParticle p) {
	// formule: force du vent	
	// Fw = Pd * A	* Cd	         -- Pd: pression dynamique, A: aire projetée, Cd: coefficient de trainée
	// Fw = 0.5 rho* v^2 * A * Cd    -- rho: densité de l'air, v: vitesse du vent, A: aire
	// v doit être la vitesse relative donc v= vVent - vParticle
	// 
	// a finir

	float aera = (SPACE_P1_TO_P2 * SPACE_P1_TO_P2) / 2;
	glm::vec3 rel_vel = speed - p.velocity; // relative velocity des particles

	glm::vec3 winforce = (float)(0.5f * rho * aera * (drag_coeff - lift_coeff) * (glm::dot(rel_vel, p.normal) + 
								lift_coeff * glm::length(rel_vel) * glm::length(speed))) * p.normal;
	return winforce;
}

glm::vec3 Wind::basicForce(myParticle p) {

	// Cv  = coefficient de viscosité 
	// si vous avez compris cette formule expliquez moi svp :(

	float Cv = 0.02f; // 0.0075 , 0.02
	return Cv * (glm::dot(p.normal, (speed - p.velocity))) * p.normal;
}