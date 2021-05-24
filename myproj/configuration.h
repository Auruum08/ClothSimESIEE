#pragma once
 
const double GRAVITY = 1;				// Gravity constant
const double DT = 0.005;				// Refreshing step

/*Particles*/
const double DEFAULT_MASS = 0.01;		// Mass of the particles
const double DEF_RATE = 0.1;			// degré de deformation autorisé


/*Springs*/
const float DEFAULT_K = 10;					// Stiffness hooke's constant.
const float M_KD_1 = 1;              // Springs damping (scalaire)
const float M_KD_2 = 0.01;				// smooth springs damping

/*Mass-spring*/
const int DEFAULT_WIDTH = 30;			// Width of the mass-spring system
const int DEFAULT_HEIGHT = 20;				// Height of the mass-spring system 
const float SPACE_P1_TO_P2 = 0.5;	// Space between masses (particles)

const int EULER = 0;
const int VERLET = 1;
const int INTEGRATOR = VERLET;

/*
	n:
	ajout: 
		* verlet integrator
		* contrainte facile de longueur des ressorts
		* correction d'une erreur débile dans le massSpring.cpp (ds initSpring)

*/