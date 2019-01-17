#include "planet.hpp"

planet::planet(vec4 ballCoords,vec4 pathCoords ,vec3 elipseCoefs, vec2 rotationSpeed) :_ballCoords(ballCoords),_pathCoords(pathCoords),speed(rotationSpeed) , _elipseCoefs(elipseCoefs) {
  //Sphere creation
  _ball = new Sphere(_ballCoords.w,32,16);

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
  ModelMatrix = glm::mat4(1.0f);
  rotations = vec2(0,0);

  //Textures
  glGenTextures(1,&_texture);
  
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
  glBindTexture(GL_TEXTURE_2D,_texture);
  glActiveTexture(_texture);
  glUniform1i(uSampler,0);
  
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLES,0,_ball->getVertexCount());
  glBindVertexArray(0);
  
  glBindTexture(GL_TEXTURE_2D,0);
}

void planet::assignShader(Program &shader){
  _shader = &shader;
  uSampler = glGetUniformLocation(_shader->getGLId(),"fSampler");
}

void planet::loadTexture(string filepath){
  cout << "Loading texture" << endl;
  std::unique_ptr<Image> texImg = loadImage(FilePath(filepath));
  glBindTexture(GL_TEXTURE_2D,_texture);
    
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texImg->getWidth(),texImg->getHeight(),0,GL_RGBA,GL_FLOAT,texImg->getPixels());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D,0);
}


void planet::update(float delta){
  	       
  rotations.y +=speed.y*delta;
  translation = vec3(cos(rotations.y)*_pathCoords.w*_elipseCoefs.x + _pathCoords.x , 0,sin(rotations.y)*_pathCoords.w *_elipseCoefs.z + _pathCoords.z);
  ModelMatrix = translate(mat4(1),translation);

  rotations.x += speed.x*delta;
  ModelMatrix = rotate(ModelMatrix,radians(rotations.x),vec3(0,1,0));
	
}
