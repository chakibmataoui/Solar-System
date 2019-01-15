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
  planet(float x = 0, float y = 0, float z = 0, float r = 1,
	 float c_x = 0, float c_y = 0, float c_z = 0, float c_r = 1);
  ~planet(){
    glDeleteBuffers(1,&_vbo);
    glDeleteVertexArrays(1,&_vao);
  };
  void draw();
  void assignShader(Program &shader);
  //void loadTexture();
  //Props
  Sphere *_ball;
  //Ball related positions
  float _x;
  float _y;
  float _z;
  float _r;
  //Galaxy related positions
  float _c_x;
  float _c_y;
  float _c_z;
  float _c_r;
  //OpenGLrelated
  GLuint _vao;
  GLuint _vbo;
  GLuint _texture;
  glm::mat4 ProjMatrix;
  glm::mat4 MVMatrix;
  glm::mat4 NormalMatrix;
  //Shader related
  Program *_shader;
  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uTime;
};
