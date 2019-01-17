#include "app.hpp"

bool app::init(int w,int h){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  //For skybox
  glEnable(GL_CULL_FACE);
  //Init projections
  Projection  = perspective(radians(45.f),(float)w/h,0.1f,10000.f);
  //TODO : INIT CAMERA TO BE CHANGED HERE

  //Loading shaders
  gen = loadProgram(applicationPath->dirPath() + "shaders/planete.vs.glsl", applicationPath->dirPath() + "shaders/planete.fs.glsl");
  uMVP = glGetUniformLocation(gen.getGLId(),"uMVPMatrix");
  uMV = glGetUniformLocation(gen.getGLId(),"uMVMatrix");
  uNormal = glGetUniformLocation(gen.getGLId(),"uNormalMatrix");
  uLightDir = glGetUniformLocation(gen.getGLId(),"uLightSource");

  //Sun
  sun.assignShader(gen);
  sun.loadTexture(applicationPath->dirPath() + "textures/sunmap.jpg");
  
  /*vector<planet*> planetList = {mercury,
				venus,
				earth,
				mars,
				jupiter,
				saturn,
				uranus,
				neptune,
				pluton};*/

  vector<float> diametre = {(float)4879/ratio,
			    (float)12104/ratio,
			    (float)12765/ratio,
			    (float)3933/ratio,
			    (float)142984/ratio,
			    (float)120536/ratio,
			    (float)51118/ratio,
			    (float)49528/ratio,
			    (float)2370/ratio};
  vector<float> aphelion = {69.8,
			    108.9,
			    152.1,
			    249.2,
			    816.6,
			    1514.5,
			    3003.6,
			    4545.7,
			    7375.9};
  vector<float> periphelion = {46.0 ,
			       107.5,
			       147.1,
			       206.6,
			       740.5,
			       1352.6,
			       2741.3,
			       4444.5,
			       4436.8};
  vector<float> radius;
  for(int i = 0; i < aphelion.size();++i){
    radius.push_back((aphelion[i]+periphelion[i])/2);
  }
  
  vector<string> texturePath = {"mercurymap.jpg",
				"venusmap.jpg",
				"earthmap.jpg",
				"mars_1k_color.jpg",
				"jupitermap.jpg",
				"saturnmap.jpg",
				"uranusmap.jpg",
				"neptunemap.jpg",
				"plutomap1k.jpg",};
  vector<float> speedDay = {4222.6/24,
			    2802.0/24,
			    1,
			    27.7/24,
			    9.9/24,
			    10.7/24,
			    17.2/24,
			    16.1/24,
			    153.3/24};
  vector<float> orbitalPeriod = {88,
				 224.7,
				 365.5,
				 687.0,
				 4331,
				 10747,
				 30589,
				 59800,
				 90560};
  vector<float> incline = {7.0 ,
			   3.4 ,
			   0.0 ,
			   1.9,
			   1.3 ,
			   2.5 ,
			   0.8 ,
			   1.8 ,
			   17.2};
  float turnSpeed = 0.01;
  /* démarrage en position aléatoire */
  srand (time(NULL));
  for(int i = 0; i < radius.size(); ++i){
    planetList.push_back(new planet(vec4(0,0,0,diametre[i]),vec4(aphelion[i]-radius[i]*100000/ratio,0,0,radius[i]*100000/ratio),vec3(1,1,1),vec2(speedDay[i]/365.5*turnSpeed,orbitalPeriod[i]/365.5*turnSpeed)));
    planetList[i]->assignShader(gen);
    planetList[i]->loadTexture(applicationPath->dirPath() + "textures/" + texturePath[i]);
    //Random rotation
    planetList[i]->rotations = vec2(rand(),rand());
  }
  
  //Skybox
  skybox.assignShader(gen);
  skybox.loadTexture(applicationPath->dirPath() + "textures/Galaxy.jpg");
  return true;
}

void app::handleEvent(SDL_Event e){
  switch(e.type){
  case SDL_KEYDOWN :
    if(e.key.keysym.sym == SDLK_z){
      cam.movState.moveForward = true;
    }
    if(e.key.keysym.sym == SDLK_s){
      cam.movState.moveBackward = true;
    }
    if(e.key.keysym.sym == SDLK_q){
      cam.movState.moveLeft = true;
    }
    if(e.key.keysym.sym == SDLK_d){
      cam.movState.moveRight = true;
    }
    if(e.key.keysym.sym == SDLK_UP){
      cam.movState.turnUp = true;
    }
    if(e.key.keysym.sym == SDLK_DOWN){
      cam.movState.turnDown = true;
    }
    if(e.key.keysym.sym == SDLK_LEFT){
      cam.movState.turnLeft = true;
    }
    if(e.key.keysym.sym == SDLK_RIGHT){
      cam.movState.turnRight = true;
    }
    if(e.key.keysym.sym == SDLK_SPACE){
      cam.movState.moveUp = true;
    }
    if(e.key.keysym.sym == SDLK_a){
      cam.movState.moveDown = true;
    }
    //CHange view
    if(e.key.keysym.sym == SDLK_F1){
      cam.cam_state = FREEVIEW;
    }
    if(e.key.keysym.sym == SDLK_F2){
      cam.cam_state = SIDEVIEW;
    }
    if(e.key.keysym.sym == SDLK_F3){
      cam.cam_state = TOPVIEW;
    }
    //View Planet N
    if(e.key.keysym.sym == SDLK_KP0){
      cout << "0" << endl;
      cam.p = &sun;
      cam.cam_state = PLANETVIEW;
    }
    break;
    case SDL_KEYUP :
    if(e.key.keysym.sym == SDLK_z){
      cam.movState.moveForward = false;
    }
    if(e.key.keysym.sym == SDLK_s){
      cam.movState.moveBackward = false;
    }
    if(e.key.keysym.sym == SDLK_q){
      cam.movState.moveLeft = false;
    }
    if(e.key.keysym.sym == SDLK_d){
      cam.movState.moveRight = false;
    }
    if(e.key.keysym.sym == SDLK_UP){
      cam.movState.turnUp = false;
    }
    if(e.key.keysym.sym == SDLK_DOWN){
      cam.movState.turnDown = false;
    }
    if(e.key.keysym.sym == SDLK_LEFT){
      cam.movState.turnLeft = false;
    }
    if(e.key.keysym.sym == SDLK_RIGHT){
      cam.movState.turnRight = false;
    }
    if(e.key.keysym.sym == SDLK_SPACE){
      cam.movState.moveUp = false;
    }
    if(e.key.keysym.sym == SDLK_a){
      cam.movState.moveDown = false;
    }
    break;
  }
}

void app::update(float delta){
  cam.update(delta);
  for(int i = 0; i < planetList.size(); ++i){
    planetList[i]->update(delta);
  }
  sun.update(delta);
}


void app::draw(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(int i = 0; i < planetList.size(); ++i){
  /*Drawing a planet*/
    MV = cam.viewMatrix * planetList[i]->ModelMatrix;
    MVP = Projection * MV;
    Normal =  glm::transpose(glm::inverse(MV));
  
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
    planetList[i]->draw();
  }

  /*Drawing a planet*/
  MV = cam.viewMatrix * sun.ModelMatrix;
  MVP = Projection * MV;
  Normal =  glm::transpose(glm::inverse(MV));
  
  glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
  glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
  glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
  glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
  sun.draw();
  
  /*Drawing a skybox*/
  glCullFace(GL_FRONT);  
  MV = cam.viewMatrix * skybox.ModelMatrix;
  MVP = Projection * MV;
  Normal =  glm::transpose(glm::inverse(MV));
  glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
  glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
  glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
  skybox.draw();
  glCullFace(GL_BACK);
}
