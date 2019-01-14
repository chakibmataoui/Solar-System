#include "app.hpp"

bool app::init(){
  glEnable(GL_DEPTH_TEST);
  //Loading shaders
  cout << applicationPath->dirPath() << endl;
  gen = loadProgram(applicationPath->dirPath() + "shaders/planete.vs.glsl",
	applicationPath->dirPath() + "shaders/planete.fs.glsl");
  a.assignShader(gen);
  return true;
}

void app::handleEvent(SDL_Event e){
  
}

void app::update(){
}


void app::draw(){
  a.draw();
}
