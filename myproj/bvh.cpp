#include "bvh.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;
/* c'est peut-être mieux d'utiliser des structs pour les aabb,
*  mais je ne sais pas comment gérer les structs entre .h / .c
*  donc, a voir pour plus tard.
*/

/*
*	###############################################
*	###############################################
	class Aabb
*/

Aabb::Aabb() {

	p1 = vec3(0, 0, 0);
	p2 = vec3(0, 0, 0);

}
Aabb::Aabb(vec3* pa,vec3* pb, vec3* pc) {

	 t1 = pa;
	 t2 = pb;
	 t3 = pc;

	 p1 = vec3(0, 0, 0);
	 p2 = vec3(0, 0, 0);

};

// ça sert à rien, à enlever, c'est rien que pour l'exemple de la sphere qui entre en contact avec le tissu
Aabb::Aabb(vec3 center) {
	c = center;
}

// update box
 void Aabb::updateBox() { 

		 float x1 = fminf(fminf(t1->x, t2->x), t3->x);
		 float y1 = fminf(fminf(t1->y, t2->y), t3->y);
		 float z1 = fminf(fminf(t1->z, t2->z), t3->z);

		 float x2 = fmaxf(fmaxf(t1->x, t2->x), t3->x);
		 float y2 = fmaxf(fmaxf(t1->y, t2->y), t3->y);
		 float z2 = fmaxf(fmaxf(t1->z, t2->z), t3->z);

		 p1 = vec3(x1, y1, z1);
		 p2 = vec3(x2, y2, z2);

		 l = abs(x2 - x1);
		 L = abs(y2 - y1);
		 h = abs(z2 - z1);
 }
 void Aabb::unir(Aabb* a, Aabb* b) {
	 float x1 = fminf(a->p1.x, b->p1.x);
	 float y1 = fminf(a->p1.y, b->p1.y);
	 float z1 = fminf(a->p1.z, b->p1.z);

	 float x2 = fmaxf(a->p2.x, b->p2.x);
	 float y2 = fmaxf(a->p2.y, b->p2.y);
	 float z2 = fmaxf(a->p2.z, b->p2.z);

	 p1.x = x1; p1.y = y1; p1.z = z1;
	 p2.x = x2; p2.y = y2; p2.z = z2;

	 l = abs(x2 - x1);
	 L = abs(y2 - y1);
	 h = abs(z2 - z1);
 }
 void Aabb::updatePos() {
	 l = abs(p2.x - p1.x);
	 L = abs(p2.y - p1.y);
	 h = abs(p2.z - p1.z);
 }
 
// check overlappings
 bool Aabb::overlapPoint(vec3 p) {
	 if (abs(p1.x - p.x) < l && abs(p2.x - p.x)<l) {
		 if (abs(p1.y - p.y)<L && abs(p2.y - p.y)<L) {
			 if (abs(p1.z - p.z)<h && abs(p2.z - p.z)<h) {
				 return true;
			 }
		 }
	 }
	 return false;
 }
 bool Aabb::overlapBox(Aabb* a) {
	 if (overlapPoint(a->p1))  return true;
	 if (overlapPoint(a->p1 + vec3(a->l,0,0)))  return true;
	 if (overlapPoint(a->p1 + vec3(a->l, a->L, 0)))  return true;
	 if (overlapPoint(a->p1 + vec3(0, a->L, 0)))  return true;

	 if (overlapPoint(a->p2))  return true;
	 if (overlapPoint(a->p2 - vec3(a->l, 0.0,0)))  return true;
	 if (overlapPoint(a->p2 - vec3(a->l, a->L,0)))  return true;
	 if (overlapPoint(a->p2 - vec3(0, a->L, 0)))  return true;

	 return false;
 }
 
 // draw box
void Aabb::draw() {
	// attention cette fonction <doit> etre entourée de glbegin et glend,
	// je ne le mets pas à l'interieur de cette fonction car trop couteux --> plutot le mettre au début et fin d'une fonction qui
	// affiche toutes les boites
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p1.x + l, p1.y, p1.z);
	glVertex3f(p1.x + l, p1.y + L, p1.z);
	glVertex3f(p1.x, p1.y + L, p1.z);

	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p1.x, p1.y + L, p1.z);
	glVertex3f(p1.x, p1.y + L, p1.z + h);
	glVertex3f(p1.x, p1.y, p1.z + h);

	glVertex3f(p1.x, p1.y, p1.z + h);
	glVertex3f(p1.x + l, p1.y, p1.z + h);
	glVertex3f(p1.x + l, p1.y + L, p1.z + h);
	glVertex3f(p1.x, p1.y + L, p1.z + h);

	glVertex3f(p1.x + l, p1.y, p1.z);
	glVertex3f(p1.x + l, p1.y + L, p1.z);
	glVertex3f(p1.x + l, p1.y + L, p1.z + h);
	glVertex3f(p1.x + l, p1.y, p1.z + h);
}



/*
*	###############################################
*	###############################################
	class node
*/

Node::Node() {};

Node::Node(vec3* a, vec3 *b, vec3 *c) {

	box = Aabb(a,b,c);
	isLeaf = true;
	update();
}

Node::Node(Node* c1, Node* c2) {

	child1 = c1;
	child2 = c2;

	isLeaf = false;
	update();
}

void Node::update() {
	// si c'est une feuille,reformer la aabb a partir des point du triangle, (box.updatebox)
	// sinon, aabb est formé par les plus petites et plus grandes coordonnées des enfants. (box.unir) 
	if (isLeaf) {
		box.updateBox(); 
		return;
	}
	box.unir(&child1->box, &child2->box);
	
}
