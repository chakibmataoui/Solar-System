#include "planet.hpp"

planet::planet(float x, float y, float z,float r,
	       float c_x, float c_y ,float c_z, float c_r) : _x(x), _y(y) , _z(z),_r(r) ,_c_x(c_x), _c_y(c_y) , _c_z(c_z),_c_r(c_r) {
  //Sphere creation
  _ball = new Sphere(_r,32,16);

  //VBO creation
  glGenBuffers(1,&_vbo);
  glBindBuffer(GL_ARRAY_BUFFER,_vbo);
  glBufferData(GL_ARRAY_BUFFER,_ball->getVertexCount()*sizeof(ShapeVertex),_ball->getDataPointer(),GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,0);

  //VAO setting
  glGenVertexArrays(1,&_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,_vbo);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);

  //Others
}

void planet::draw(){
  _shader->use();
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLES,0,_ball->getVertexCount());
  glBindVertexArray(0);
}

void planet::assignShader(Program &shader){
  _shader = &shader;
}
