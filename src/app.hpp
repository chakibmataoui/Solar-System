#pragma once
#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

using namespace glimac;
using namespace std;

class app{
public:
  app(SDLWindowManager &w) : _w(&w){};
  ~app() {};
  bool init();
  void handleEvent(SDL_Event e);
  void update();
  void draw();
private:
  SDLWindowManager *_w;
  //First init test part for drawing simple triangle
};
