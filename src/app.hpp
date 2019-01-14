#pragma once
#include <glimac/SDLWindowManager.hpp>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

#include "planet.hpp"
using namespace glimac;
using namespace std;

extern FilePath *applicationPath;

class app{
public:
  app(SDLWindowManager &w) : _w(&w){};
  ~app() {
    

  };
  bool init();
  void handleEvent(SDL_Event e);
  void update();
  void draw();
private:
  SDLWindowManager *_w;
  //List of planets
  planet a;
  //List of shaders
  Program gen;
  //Camera
};
