#include "myMassSpring.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mySpring.h"
#include <vector>
#include "mySphere.h"
#include <iostream>
#include <math.h>   

#include "Wind.h"
#include <list>

/// ce QUE j'ai modifié:
/// mymassSpring():
///		* initTriangles() 
///		* makeTree() (dépend du tableau que j'ai fait "triangles", donc ptet remplacer par le tien )
/// 
///	calculateNextPosition():
///		*updateNodes()
///		*epicUnnamedFunction() => c'est la correction par impulsions, mais ça marche pas super, +- correction directe de pos



myMassSpring::myMassSpring(unsigned int width, unsigned int height)
{
	unsigned int i, j;

	//Creating width*height particles.
	particles.assign(width, std::vector<myParticle>());

	// créer les particles
	for (i = 0; i < width; i++)
	{
		particles[i].assign(height, myParticle());
		for (j = 0; j < height; j++)
		{
			particles[i][j] = myParticle( static_cast<float>(i * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * width / 2.0f),10.0f,
				static_cast<float>(j * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * height / 2.0f + 2.0f));
		}
	}

	// créer les ressorts...
	int profondeurLiens=2;
	initSprings(width, height,profondeurLiens);

	//création des triangles
	int nbTri = (width - 1) * 2 * (height - 1);
	triangles.assign(nbTri, std::vector<vec3*>());
	initTriangles();

	// création de la bvh
	makeTree();

	

	for (i=0;i<2; i++)
		particles[0][i].movable = false;
	for (i = (int)(width-2); i < width; i++)
		particles[0][i].movable = false;


	//Giving color values.
	kd[0] = 0.8f;		kd[1] = 0.8f;		kd[2] = 0.8f;		kd[3] = 1.0f;
	depth = 2;
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


void myMassSpring::addForces(Wind wind)
{

	glm::vec3 gravity(0.0f, -DEFAULT_MASS * GRAVITY, 0.0f);

	for (auto& row : particles) {
		for (auto& col : row) {
			col.addForce(gravity);

			//col.addForce(wind.disneyForce(col));
			//col.addForce(wind.basicForce(col));
		}
	}
	for (auto& sp : springs) {
		sp.addForce();
	}

}

void myMassSpring::calculateNextPosition()
{
	
	updateNodes();
	//epicUnnamedFunction();
	
	for (auto& row : this->particles) {
		for (auto& col : row) {
			col.calculateNextPosition();
		}
	}

	for (auto& sp : springs) {
		sp.corrigepos();
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


/* nat functions */
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
				if (glm::length(dist) <= s->radius) {
					//particles[i][j].velocity -= dist;
					float erreur = glm::length(dist) - s->radius;
					glm::vec3 impulse = (dist/(1.0f/particles[i][j].mass))*(erreur/(float)DT);
					particles[i][j].velocity += (impulse)*(1.0f/particles[i][j].mass);
				}
			}
		}
	}

	void myMassSpring::epicUnnamedFunction() {
	
		bool allConstraintsSatisfied = true;
		int j = 0;
		do {
			allConstraintsSatisfied = true;
			// trouver l'erreur par intégration? 
			// ==> calculer la position à partir des vitesses?
			for (int i = 0; i < springs.size(); i++) {
				mySpring s = springs[i];
				float e = s.getConstraint(); //get Error
			
				if (abs(e) >= (DEF_RATE * s.restLength)) { // si l'erreur est supérieure à 10% de la longueur du ressort

					glm::vec3 posdist = s.p2->position - s.p1->position;
					//glm::vec3 spddist = s.p2->velocity - s.p1->velocity;

					float ka = 0, kb =0; 
					if (s.p1->movable) ka = 1.0f / s.p1->mass;
					if (s.p2->movable) kb = 1.0f / s.p2->mass;

					// compute impulse
					glm::vec3 p = (posdist / (ka + kb)) * (e / (float)DT);
				
					// correction impulses (je ne crois pas..)
					//s.p1->addForce(p); s.p2->addForce(-p);

					// new velocity
					s.p1->velocity += ka*p;
					s.p2->velocity -= kb*p;
					allConstraintsSatisfied = false; j++;
				}
			}
		}while (!allConstraintsSatisfied && j<3);
	
	}

	void myMassSpring::initTriangles() {
		int nbt = 0;
		for (size_t i = 0; i < particles.size() - 1; i++) {
			for (size_t j = 0; j < particles[i].size() - 1; j++)
			{
				///    __	
				///	   |/		/|
				///	   '	   '--
				/// ( voir agencement des triangles : "draw springs" donne les vertices)
				triangles[nbt] = {  
					&particles[i][j].position,
					&particles[i+1][j].position,
					&particles[i+1][j+1].position
				};
				triangles[nbt+1] = {
					&particles[i][j+ 1].position,
					&particles[i + 1][j].position,
					&particles[i+1][j+1].position
				};

				nbt += 2;
			}
		}
	}

	void myMassSpring::makeTree() {

		// make leaves:
		for (int i = 0; i < triangles.size(); i++) {		
			Node* n = new Node(triangles[i][0], triangles[i][1], triangles[i][2]);
			nodes.push_back(n);
		}
		
		creerParents(0,nodes.size(),(DEFAULT_WIDTH-1)*2, (DEFAULT_HEIGHT - 1) ,0);
	}
	void myMassSpring::creerParents(int i1, int j1, int w, int h, int d) {
		cout << d;
		int taille = abs(j1-i1);
		if (taille < 1) return;

		int first = nodes.size();
			for (int i = 0; i < w; i += 2) {
				for (int j = 0; j < h; j += 1) {
				
					Node* n = new Node(nodes[i1 + i + j * w], nodes[i1 + i + j * w + 1]);
					nodes.push_back(n);
				}
			}
			int last = nodes.size() - 1;
			creerParents(first, last, h, w / 2, d + 1);
		
	}
	
	void myMassSpring::drawNodes(Node *n, int d) {
		// fonction à mettre dans drawBVH
		if (d == 0) { n->box.draw(); return; }

		drawNodes(n->child1, d - 1);
		drawNodes(n->child2, d - 1);
	}
	void myMassSpring::drawBVH() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
			drawNodes(nodes[nodes.size()-1], depth);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	void myMassSpring::drawBVH2(Node *n,int depth,mySphere *s){
		
		if (n->box.overlapBox(&s->box) && (!n->isLeaf)) {
			drawBVH2(n->child1, depth, s);
			drawBVH2(n->child2, depth, s);
			return;
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
			n->box.draw();
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	void myMassSpring::updateNodes() {
		for (Node *n : nodes) n->update();
	}
	
myMassSpring::~myMassSpring()
{
	
}


