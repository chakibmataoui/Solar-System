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


const float ratio = 30000; //Normalization ratio
const float daySpeed = 1;//Speed of 1 day
class app{
public:
  app(SDLWindowManager &w) : _w(&w){};
  ~app() {
    for(int i = 0; i < planetList.size();++i)
      delete planetList[i];
    for(int i = 0; i < marsSat.size();++i)
      delete marsSat[i];
    for(int i = 0; i < saturnSat.size();++i)
      delete saturnSat[i];
    for(int i = 0; i < uranusSat.size();++i)
      delete uranusSat[i];
    for(int i = 0; i < neptuneSat.size();++i)
      delete neptuneSat[i];
    for(int i = 0; i < plutonSat.size();++i)
      delete plutonSat[i];
    delete saturnRings;
    delete uranusRings;
  };
  bool init(int w, int h); //Init the world and opengl related functionalities
  void handleEvent(SDL_Event e); //Handle key enter events
  void update(float delta); //Update the physics
  void draw(); //Draw the world
private:
  SDLWindowManager *_w;
  //List of planets
  planet sun = planet(vec4(0,0,0,(float)1391016/ratio),vec4(0,0,0,0),vec3(1,1,1),vec2(daySpeed*25,0)); 
  planet moon = planet(vec4(0,0,0,(float)3475/ratio),vec4(0,5.4,0,(float)40600/ratio),vec3(1,1,1),vec2(-1,1/24));
  vector<planet*> planetList; //The list of the main planets
  //Satelites arrays
  vector<planet*> marsSat;
  vector<planet*> jupiterSat;
  vector<planet*> saturnSat;
  vector<planet*> uranusSat;
  vector<planet*> neptuneSat;
  vector<planet*> plutonSat;

  //Rings
  planet *saturnRings;
  planet *uranusRings;
  //Skybox
  planet skybox = planet(vec4(0,0,0,50000));
  //Shaders generic
  Program gen;
  GLint uMVP,uMV,uNormal,uTextureSampler;
  GLint uLightDir;
  GLint uSpecular,uAmbient,uRotationUV;
  //Camera related
  camera cam;
  //Projection view
  mat4 Projection;
  mat4 MV;
  mat4 MVP;
  mat4 Normal;
};
