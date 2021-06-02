#pragma once
#include "configuration.h"
#include <glm/glm.hpp>
using namespace glm;


/*
*	###############################################
*	###############################################
	class Aabb
*/

class Aabb{
	public:

		vec3 p1,p2;
		vec3 *t1, *t2, *t3, c;

		float l, L, h;

		bool isLeaf;

		Aabb();
		Aabb(vec3* t1, vec3* t2, vec3* t3);
		Aabb(vec3 center);

		void updateBox();				 // pour les feuilles les triangles ne changent pas de forme, mais changent d'orientation donc on doit recalculer la boite?
		void unir(Aabb *a, Aabb *b);
		void updatePos();
		
		bool overlapBox(Aabb *a);
		bool overlapPoint(vec3 p);

		void draw();
};


/*
*	###############################################
*	###############################################
	class node
*/

class Node{
	public:
		Aabb box;

		Node* child1;
		Node* child2;
		
		bool isLeaf;

		Node();
		Node(vec3* p1, vec3* p2, vec3* p3); // --> créer une feuille (p1, p2, p3 sont les 3 points du triangle)
		Node(Node* c1, Node* c2);			// --> créer un parent 

		void update();
};


///  il faut appeler "update" à chaque frame pour avoir un AAbb dynamique 
///  (le bout de tissu change de forme) 



