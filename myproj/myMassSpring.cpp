#include "myMassSpring.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mySpring.h"
#include <vector>
#include "mySphere.h"
#include <iostream>
#include <math.h>   
#include "PerlinNoise.hpp"

myMassSpring::myMassSpring(unsigned int width, unsigned int height)
{
	unsigned int i, j;

	//Creating width*height particles.

	//int nbTri = (width - 1) * 2 * (height - 1);
	//triangles.assign(nbTri);

	particles.assign(width, std::vector<myParticle>());
	for (i = 0; i < width; i++)
	{
		particles[i].assign(height, myParticle());
		for (j = 0; j < height; j++)
		{
			particles[i][j] = myParticle( static_cast<float>(i * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * width / 2.0f),10.0f,
				static_cast<float>(j * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * height / 2.0f + 2.0f));
			
		}
	}
	int profondeurLiens=3;
	initSprings(width, height,profondeurLiens);
	
	for (i=0;i<2; i++)
		particles[0][i].movable = false;
	for (i = (int)(width-2); i < width; i++)
		particles[0][i].movable = false;

	/*TODO: add springs*/


	//Giving color values.
	kd[0] = 0.8f;		kd[1] = 0.8f;		kd[2] = 0.8f;		kd[3] = 1.0f;

	computeNormals();
}

void myMassSpring::clearForces()
{
	for (auto& row : particles) {
		for (auto& col : row) {
			col.force = glm::vec3 (0.0f, 0.0f, 0.0f);

		}
	}
}

void myMassSpring::addForces()
{
	siv::PerlinNoise noise;
	wcount += 0.00001f;
	double rho = 1.225;


	double drag_coeff = 0.06;
	double lift_coeff = 0.03;

	glm::vec3 gravity(0.0f, -DEFAULT_MASS * GRAVITY, 0.0f);
	

	float windDir = 10*noise.noise1D(wcount);
	float windAmpl = (float)(-20.0f * (1.0f - 0.2 * (0.5f + 0.5f*windDir)));
	glm::vec3 windAmp = (float)(50.0f * noise.noise1D(wcount)) * glm::vec3(cos(windDir),0.0f,sin(windDir));
	//glm::vec3 windSpd( windAmp.x,0.0f,windAmp.z);
	glm::vec3 windSpd(10.0f,0.0f,0.0f);

	for (auto& row : particles) {
		for (auto& col : row) {
			col.addForce(gravity);
			float aera = (SPACE_P1_TO_P2 * SPACE_P1_TO_P2) / 2;
		
			//col.addForce(0.09f*(glm::dot(col.normal,(windSpd-col.velocity)))*col.normal);
			glm::vec3 winforce = (float)(0.5f * rho * aera * (drag_coeff - lift_coeff) * (glm::dot(windSpd, col.normal) + lift_coeff * glm::length(windSpd) * glm::length(windSpd))) * col.normal;
			col.addForce(winforce);
		}
	}

	for (auto& sp : springs) {
		
			sp.addForce();
	}
	

   /*TODO: add gravity/spring forces*/
}

void myMassSpring::calculateNextPosition(int p_integrator)
{
	for (auto& row : this->particles) {
		for (auto& col : row) {
			col.calculateNextPosition();
		}
	}
	/*TODO*/
}

void myMassSpring::ballCollision(mySphere* s)
{
	/*TODO: move cloth if collides with ball*/
	badcollisionNat(s);
	
}

void myMassSpring::groundCollision(float floor_y)
{
	float l;
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
		{
			l = (particles[i][j].position.y - floor_y);
			if (l < 0)
				//particles[i][j].position.y = floor_y;
				particles[i][j].movable = false;
		}
}

void myMassSpring::computeNormals()
{
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].normal = glm::vec3(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < particles.size() - 1; i++)
		for (size_t j = 0; j < particles[i].size() - 1; j++)
		{
			glm::vec3 face_normal = glm::cross(particles[i + 1][j].position - particles[i][j].position,
				particles[i + 1][j + 1].position - particles[i + 1][j].position);
			particles[i][j].normal += face_normal;
			particles[i + 1][j].normal += face_normal;
			particles[i + 1][j + 1].normal += face_normal;

			face_normal = glm::cross(particles[i][j + 1].position - particles[i + 1][j + 1].position,
				particles[i][j].position - particles[i][j + 1].position);
			particles[i][j].normal += face_normal;
			particles[i][j + 1].normal += face_normal;
			particles[i + 1][j + 1].normal += face_normal;
		}

	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].normal = glm::normalize(particles[i][j].normal);
}

void myMassSpring::drawSpring()
{
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < particles.size() - 1; i++)
		for (size_t j = 0; j < particles[i].size() - 1; j++)
		{
			glNormal3fv(&particles[i][j].normal[0]);
			glVertex3fv(&particles[i][j].position[0]);
			

			glNormal3fv(&particles[i + 1][j].normal[0]);
			glVertex3fv(&particles[i + 1][j].position[0]);
			

			glNormal3fv(&particles[i + 1][j + 1].normal[0]);
			glVertex3fv(&particles[i + 1][j + 1].position[0]);
			

			glNormal3fv(&particles[i][j].normal[0]);
			glVertex3fv(&particles[i][j].position[0]);
			

			glNormal3fv(&particles[i + 1][j + 1].normal[0]);
			glVertex3fv(&particles[i + 1][j + 1].position[0]);
			

			glNormal3fv(&particles[i][j + 1].normal[0]);
			glVertex3fv(&particles[i][j + 1].position[0]);
			
		}
	glEnd();
}

void myMassSpring::initSprings(unsigned int width, unsigned int height, int profondeurLiens)
{
	int taille=0;
	int compt=0;

	/* initialiser le tableau:
	   1: liens (horizontaux et verticaux) */
	for (int i=1;i<profondeurLiens;i++){
		taille += (width - profondeurLiens) * (height)+ (width)*(height - profondeurLiens)
			;
    }
	taille += 2 * (width - 1) * (height - 1) ;
	/* 2: liens (diagonales) */

	springs.assign(taille,mySpring());


	/* créer les springs (horizontal et vertical)*/
	for(int n=1;n<profondeurLiens;n++)
	{
		// springs horizontaux 
		for (int i = 0; i < width-profondeurLiens; i++)
		{
			for (int j = 0; j < height; j++)
			{
				springs[compt] = mySpring(&particles[i][j], &particles[i+n][j],DEFAULT_K,n*SPACE_P1_TO_P2);
				compt += 1;
			}
		}
		// springs verticaux
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height-profondeurLiens; j++)
			{
				springs[compt] = mySpring(&particles[i][j], &particles[i][j+n],DEFAULT_K,n*SPACE_P1_TO_P2);
				compt += 1;
			}
		}
		/* créer les springs (diagonales) */
		
    }
	for (int n = 1; n < 2; n++) {
		for (int i = 0; i < width - n; i++)
		{
			for (int j = 0; j < height - n; j++)
			{
				springs[compt] = mySpring(&particles[i][j], &particles[i + n][j + n], DEFAULT_K, sqrt(pow(SPACE_P1_TO_P2 * n, 2.0) * 2.0));
				springs[compt + 1] = mySpring(&particles[i][j + n], &particles[i + n][j], DEFAULT_K, sqrt(pow(SPACE_P1_TO_P2 *n, 2.0) * 2.0));

				compt += 2;
			}
		}
	}
	
	
}

void myMassSpring::badcollisionNat(mySphere* s) {
	for (size_t i = 0; i < particles.size(); i++) {
		for (size_t j = 0; j < particles[i].size(); j++)
		{
			glm::vec3 dist = s->center - particles[i][j].position;
			if (glm::length(dist) < s->radius) particles[i][j].velocity -= dist;

		}
	}
}

myMassSpring::~myMassSpring()
{

}

