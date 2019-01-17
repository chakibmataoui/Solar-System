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
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include "camera.hpp"
#include "planet.hpp"
using namespace glimac;
using namespace std;
using namespace glm;
extern FilePath *applicationPath;


const float ratio = 100000;
const float daySpeed = 1;
class app{
public:
  app(SDLWindowManager &w) : _w(&w){};
  ~app() {
    

  };
  bool init(int w, int h);
  void handleEvent(SDL_Event e);
  void update(float delta);
  void draw();
private:
  SDLWindowManager *_w;
  //List of planets
  planet sun = planet(vec4(0,0,0,(float)1391016/ratio),vec4(0,0,0,0),vec3(1,1,1),vec2(daySpeed*25,0));
  vector<planet*> planetList;
  //Skybox
  planet skybox = planet(vec4(0,0,0,1000));
  //Shaders generic
  Program gen;
  GLint uMVP,uMV,uNormal,uTextureSampler;
  GLint uLightDir;
  //Camera related
  camera cam;
  //Projection view
  mat4 Projection;
  mat4 MV;
  mat4 MVP;
  mat4 Normal;
};
