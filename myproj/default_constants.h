#pragma once
#include "glm/glm.hpp"

const glm::vec3 DEFAULT_CAMERA_EYE = glm::vec3(0.0f, 0.0f, 40.0f);
const glm::vec3 DEFAULT_CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_CAMERA_FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);

const float DEFAULT_FOVY = 45.0f;
const float DEFAULT_zNEAR = 0.2f;
const float DEFAULT_zFAR = 2000.0f;

const int DEFAULT_WINDOW_HEIGHT = 400;
const int DEFAULT_WINDOW_WIDTH = 600;

const float DEFAULT_KEY_MOVEMENT_STEPSIZE = 1.0f;
const float DEFAULT_MOUSEWHEEL_MOVEMENT_STEPSIZE = 0.5f;
const float DEFAULT_LEFTRIGHTTURN_MOVEMENT_STEPSIZE = 0.01f;

const float CUBE_RADIUS = 30.0f;