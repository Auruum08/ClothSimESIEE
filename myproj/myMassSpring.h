#pragma once
#include "configuration.h"
#include "myParticle.h"
#include "mySpring.h"
#include "mySphere.h"
#include "PerlinNoise.hpp"

#include "Wind.h"
#include "bvh.h"
#include <vector>
#include <list>


using namespace std;

class myMassSpring
{
public:
	std::vector< std::vector<myParticle> > particles;
	std::vector<mySpring> springs;

	// nat modifs
		std::vector< std::vector<vec3*> > triangles;

		Aabb myfirstbox;
		std::vector< Node* > nodes;
		int depth;
		siv::PerlinNoise mynoise;
	
	
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

			/* respect des contraintes de position par applications d'impulsions
			(changement instantané dans la vitesse, mais pas dans la position, 
			changement de vitesse à l'instant t pour la correction de la position à l'instant t+h)
			*/
			void epicUnnamedFunction();

			/* fonction temporaire 
				initialise le tableau (temporaire) leaves de bounding boxes pour les triangles*/
			void makeTree();

			/*remplit le tab de triangles (utile pour les bounding boxes j'imagine)*/
			void initTriangles();

			/*dessiner les bounding boxes, récursif */
			void drawNodes(Node *n, int depth);
			void updateNodes();
			
			// affiche la bvh en entier ( appuyer sur F1 et F2 pour voir une profondeur au dessus/dessous)
			void drawBVH(); 

			// affiche quels noeuds sont explorés pour la collision
			void drawBVH2(Node *n, int depth, mySphere *s); 

			// crée l'arbre à partir des feuilles créées dans "makeTree", étage par étage. ne marche que pour un bout de tissu rectangulaire,
			// attention, il faut pour que ca marche que les hauteurs et lageurs soient (puissance de 2) +1 (ouais, OK déso je changerai)
			void creerParents(int i, int j, int w, int h, int d); 
			



	~myMassSpring();
};

