#pragma once
#include "configuration.h"
#include "myParticle.h"
#include "mySpring.h"
#include "mySphere.h"
#include "PerlinNoise.hpp"
#include "Wind.h"
#include <vector>

using namespace std;

class myMassSpring
{
public:
	std::vector< std::vector<myParticle> > particles;
	std::vector< std::vector<myParticle> > triangles;

	std::vector<mySpring> springs;

	siv::PerlinNoise mynoise;
	int integrator;
	float wcount;
	glm::vec4 kd;

	myMassSpring(unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_WIDTH);

	void clearForces();

	void addForces(Wind wind);

	void calculateNextPosition();

	void ballCollision(mySphere *s);

	void groundCollision(float floor_y);

	void computeNormals();

	void drawSpring();


	// fonctions nat:

			/*initialise les ressorts. */
			void initSprings(unsigned int width, unsigned int height, int profondeurLiens);

			/* mauvaise implémentation des collisions, (avec des sphères)*/
			void badcollisionNat(mySphere* s);

	~myMassSpring();
};

