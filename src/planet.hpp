#pragma once
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glm/glm.hpp>
#include "shared.hpp"

using namespace std;
using namespace glimac;
using namespace glm;

class planet{
public:
  //Methods
  planet(vec4 ballCoords = vec4(0,0,0,1),vec4 pathCoords = vec4(0,0,0,0),vec3 elipseCoefs = vec3(1,1,1), vec2 rotationSpeed = vec2(0,0));
  ~planet(){
    glDeleteBuffers(1,&_vbo);
    glDeleteVertexArrays(1,&_vao);
    glDeleteTextures(1,&_texture);
  };
  void draw();
  void update(float delta);
  void updateSat(float delta,vec3 source); //Not used 
  void assignShader(Program &shader);
  void loadTexture(string filepath);
  //void loadTexture();
  //Props
  Sphere *_ball;
  vec2 speed; //Around it self and arround rotation point
  vec2 rotations;
  vec3 _elipseCoefs;
  vec3 translation;
  //Ball related positions
  vec4 _ballCoords;
  //Galaxy related positions
  vec4 _pathCoords;
  //OpenGLrelated
  GLuint _vao;
  GLuint _vbo;
  GLuint _texture;
  glm::mat4 ModelMatrix;
  //Shader related
 
  Program *_shader;
  GLint uTime; // Not used
  GLint uSampler; //Not used part of the app.cpp 

  //Path related path not working at end
  GLuint pvbo; 
  GLuint pvao;
  vector<vec3> path;
  vector<vec3> generatePath();
  void initPaths();
  void drawPaths();
};
