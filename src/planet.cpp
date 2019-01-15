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
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
  glBindBuffer(GL_ARRAY_BUFFER,_vbo);
  glVertexAttribPointer(VERTEX_ATTR_POSITION,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)0);
  glVertexAttribPointer(VERTEX_ATTR_NORMAL,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)offsetof(ShapeVertex,normal));
  glVertexAttribPointer(VERTEX_ATTR_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)offsetof(ShapeVertex,texCoords));
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);

  //Matrix initialization
  ProjMatrix = glm::perspective(glm::radians(45.f),1.f,0.1f,1000.f);
  MVMatrix = translate(ProjMatrix,vec3(1,1,-5));
  NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
  //Others
}

void planet::draw(){
  //Check that the current shader is not this one to avoid swaps
  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm);
  //If the shader is not active load it
  if(pgm != _shader->getGLId()){
    _shader->use();
  }
  //Drawing
  glBindVertexArray(_vao);
  glm::mat4 mvp = ProjMatrix * MVMatrix;
  glUniformMatrix4fv(uMVPMatrix,1,GL_FALSE,glm::value_ptr(mvp));
  glUniformMatrix4fv(uMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
  glUniformMatrix4fv(uNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
  glDrawArrays(GL_TRIANGLES,0,_ball->getVertexCount());
  glBindVertexArray(0);
}

void planet::assignShader(Program &shader){
  _shader = &shader;
  uMVPMatrix = glGetUniformLocation(_shader->getGLId(),"uMVPMatrix");
  uMVMatrix = glGetUniformLocation(_shader->getGLId(),"uMVMatrix");
  uNormalMatrix = glGetUniformLocation(_shader->getGLId(),"uNormalMatrix");
  // uMVPMatrix = glGetUniformLocation(_shader->getGLId(),"uMVPMatrix");
}
