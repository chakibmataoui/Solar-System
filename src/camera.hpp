#pragma once
#include <glimac/SDLWindowManager.hpp>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include "planet.hpp"

using namespace glimac;
using namespace std;
using namespace glm;

//State of the user inputs
typedef struct movements{
  bool moveRight = false;
  bool moveLeft  = false;
  bool moveForward  = false;
  bool moveBackward  = false;
  bool turnLeft  = false;
  bool turnRight  = false;
  bool turnUp  = false;
  bool turnDown  = false;
  bool moveUp = false;
  bool moveDown = false;
} movements;

//Camera state enum
typedef enum camera_state{
			  TOPVIEW,
			  SIDEVIEW,
			  PLANETVIEW,
			  FREEVIEW
			  
} camera_state;

class camera{
public:
  camera(vec3 eyePos = vec3(1,0,0), vec3 eyeLook = vec3(0,0,0), vec3 upVec = vec3(0,1,0),float speed = 50);
  void update(float delta);
  vec3 _eyePos;
  vec3 _eyeLook;
  vec3 _upVec;
  float _speed;
  //camera front used to compute the camera freefly vector for the eye looking position
  vec3 cameraFront;
  mat4 viewMatrix;
  movements movState;
  camera_state cam_state;
  //Planet view
  planet *p; // planet pointer for the planet view coordinates update  (not mentioned in the report)
  float angleAround = 0;
  float zoomIn = 100;
};
