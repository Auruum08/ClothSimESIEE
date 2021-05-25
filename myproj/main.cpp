#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h> 
#include <vector>
#include <stdlib.h>      

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>   

#include "myShader.h"
#include "helperFunctions.h"
#include "myParticle.h"
#include "mySpring.h"
#include "Wind.h"
#include "myMassSpring.h"
#include "mySphere.h"
#include <iostream>
#include "myCamera.h"
#include "default_constants.h"



using namespace std;

void init();
 
myShader *shader;
myCamera *camera;
 
static int GLUTmouse[2] = { 0, 0 };
static int GLUTbutton[3] = { 0, 0, 0 };

mySphere *sphere;

myMassSpring *my_mass_spring;
 
Wind *wind;
int ang;
void mouse(int button, int state, int x, int y)
{
  int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
  GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;

  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
		camera->moveForward(DEFAULT_MOUSEWHEEL_MOVEMENT_STEPSIZE);
	else
		camera->moveBack(DEFAULT_MOUSEWHEEL_MOVEMENT_STEPSIZE);

	glutPostRedisplay();
}

void mousedrag(int x, int y)
{   
  if (GLUTbutton[0])
  {
	  camera->crystalball_rotateView(x - GLUTmouse[0], y - GLUTmouse[1]);
      glutPostRedisplay(); 
  }

	if (GLUTbutton[2])
  {
	  camera->panView( 16*(x - GLUTmouse[0]), 16 * (y - GLUTmouse[1]) );
	  glutPostRedisplay();
  }
	
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;
}

void keyboard(unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case 'a': 
			glutPostRedisplay();
			break;
		case 'h':
			std::cout << "There is no help. You're on your own :(\n";
			break;
		case 'r':
			init();
			std::cout << "\n";
			break;
		case 27:  
			exit(0) ;
			break ;
	}
	glutPostRedisplay();
}


void keyboard2(int key, int x, int y) 
{
 	int mode = glutGetModifiers();

	switch(key) 
	{
		case GLUT_KEY_UP:
			camera->moveForward(DEFAULT_KEY_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_DOWN:
			camera->moveBack(DEFAULT_KEY_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_LEFT:
			camera->turnLeft(DEFAULT_LEFTRIGHTTURN_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_RIGHT:
			camera->turnRight(DEFAULT_LEFTRIGHTTURN_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_F1:
			wind->amplitude += 1;
			break;
		case GLUT_KEY_F2:
			wind->amplitude -= 1;
			break;
	}
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	camera->window_width = width;
	camera->window_height = height;
}
 
void idle()
{
	if (sphere != nullptr)
		sphere->translate(glm::vec3(-sphere->velocity, 0.0f, 0.0f));


	my_mass_spring->addForces(*wind);
	my_mass_spring->groundCollision(-CUBE_RADIUS);

	my_mass_spring->calculateNextPosition();
	my_mass_spring->clearForces();

	if (sphere != nullptr) my_mass_spring->ballCollision(sphere);

	my_mass_spring->computeNormals();
	
	glutPostRedisplay();

	if (sphere == nullptr || sphere->center.x < -CUBE_RADIUS)
	{
		if (sphere != nullptr) delete sphere;
		//sphere = new mySphere(glm::vec3(CUBE_RADIUS, 2, 0), 6, glm::vec4(0.4, 0.0, 0.4, 0.0), 0.01f);
		float random_y = 4 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.0f;
		float random_z = 4 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 2.0f;
		float random_radius = 6 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) + 0.15f);
		float random_speed = 0.01f * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)+0.1f);

		sphere = new mySphere(glm::vec3(CUBE_RADIUS, random_y, random_z), random_radius, glm::vec4((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), 0.0), random_speed);
	}
	wind->updateSpeed();
}


void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	glViewport(0, 0, camera->window_width, camera->window_height);

	glm::mat4 projection_matrix = camera->projectionMatrix();
	shader->setUniform("myprojection_matrix", projection_matrix);

	glm::mat4 view_matrix = camera->viewMatrix();
	shader->setUniform("myview_matrix", view_matrix);

	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(view_matrix)));
	shader->setUniform("mynormal_matrix", normal_matrix);

	
	//drawing the sphere.
	if (sphere != nullptr)
	{
		shader->setUniform("kd", sphere->kd);
		shader->setUniform("mymodel_matrix", glm::translate(glm::mat4(1.0f), sphere->center));
		glutSolidSphere(sphere->radius - 0.2, 50, 50);
	}

	shader->setUniform("mymodel_matrix", glm::mat4(1.0f));

	//drawing the bounding cube.
	glutWireCube(2*CUBE_RADIUS);

	//drawing the mass spring.
	shader->setUniform("kd", my_mass_spring->kd);

	my_mass_spring->drawSpring();

	glm::vec3 rotAxis(0.0f, -1.0f, 0.0f);
	shader->setUniform("mymodel_matrix", glm::rotate(glm::mat4(1.0f), wind->angle-1.7f,rotAxis));
	glutSolidCone(1, 4, 10, 10);
	


	glFlush();
}

void init()
{
	srand( (unsigned int)time(NULL));
	sphere = nullptr;
	my_mass_spring = new myMassSpring();
	wind = new Wind();
	ang = 0;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH );
	glutCreateWindow("Collision Detection.");
	   
	GLenum err = glewInit() ; 
	if (GLEW_OK != err) std::cerr << "Error: " << glewGetString(err) << std::endl; 

	shader = new myShader("shaders/phong-vertex.glsl", "shaders/phong-fragment.glsl");
	camera = new myCamera();

	glClearColor(1.0, 1, 1, 0);
 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS) ;
	glDisable(GL_CULL_FACE);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard2);
	glutReshapeFunc(reshape);
	glutReshapeWindow(camera->window_width, camera->window_height);

	glutMotionFunc(mousedrag) ;
	glutMouseFunc(mouse) ;
	glutMouseWheelFunc(mouseWheel);

	init();

	glutMainLoop();

	return 0;
}