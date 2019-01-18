#include "app.hpp"

bool app::init(int w,int h){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  //For skybox
  glEnable(GL_CULL_FACE);
  //Init projections
  Projection  = perspective(radians(45.f),(float)w/h,0.1f,100000.f);


  //Loading shaders
  gen = loadProgram(applicationPath->dirPath() + "shaders/planete.vs.glsl", applicationPath->dirPath() + "shaders/planete.fs.glsl");
  uMVP = glGetUniformLocation(gen.getGLId(),"uMVPMatrix");
  uMV = glGetUniformLocation(gen.getGLId(),"uMVMatrix");
  uNormal = glGetUniformLocation(gen.getGLId(),"uNormalMatrix");
  uLightDir = glGetUniformLocation(gen.getGLId(),"uLightSource");
  uSpecular = glGetUniformLocation(gen.getGLId(),"specularS");
  uAmbient = glGetUniformLocation(gen.getGLId(),"ambientStrength");
  uRotationUV = glGetUniformLocation(gen.getGLId(),"rotationUV");
  //Sun init
  sun.assignShader(gen);
  sun.loadTexture(applicationPath->dirPath() + "textures/sunmap.jpg");

  //Moon init
  moon.assignShader(gen);
  moon.loadTexture(applicationPath->dirPath() + "textures/moonmap1k.jpg");
  /*vector<planet*> planetList = {mercury,
				venus,
				earth,
				mars,
				jupiter,
				saturn,
				uranus,
				neptune,
				pluton};*/
  /*Here initialize all the related data to the world space from the Nasa website*/
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
  //Compute the radius based on the aphelion and periphelion and than compute the new center in the init function
  vector<float> radius;
  for(int i = 0; i < aphelion.size();++i){
    radius.push_back((aphelion[i]+periphelion[i])/2);
  }
  
  vector<string> texturePath = {"mercurymap.jpg",
				"venusmap.jpg",
				"EarthMap.jpg",
				"mars_1k_color.jpg",
				"jupitermap.jpg",
				"saturnmap.jpg",
				"uranusmap.jpg",
				"neptunemap.jpg",
				"plutomap1k.jpg"};
  vector<float> speedDay = {4222.6,
			    2802.0,
			    24,
			    27.7,
			    9.9,
			    10.7,
			    17.2,
			    16.1,
			    153.3};
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
  //Turn speed set
  //TODO : Find a way to modify the turn speed in runtime
  float turnSpeed = 0.01;
  /* démarrage en position aléatoire */
  srand (time(NULL));
  for(int i = 0; i < radius.size(); ++i){
    planetList.push_back(new planet(vec4(0,0,0,diametre[i]),vec4(aphelion[i]-radius[i]*100000/ratio,incline[i]*0,0,radius[i]*100000/ratio),vec3(1,1,1),vec2(speedDay[i]*365.5*turnSpeed,orbitalPeriod[i]/365.5*turnSpeed)));
    planetList[i]->assignShader(gen);
    planetList[i]->loadTexture(applicationPath->dirPath() + "textures/" + texturePath[i]);
    
    //Random start position
    //planetList[i]->rotations = vec2(0,rand());
  }
  
  //Satellite init data
  //Mars
  vector<float> Marsdiametre = {(float)15/ratio,
			    (float)7.8/ratio};
  vector<float> Marsaphelion = {9378,
			    23459};
  vector<float> Marsperiphelion = {9378 ,
			       23459};
  vector<float> Marsradius;
  for(int i = 0; i < Marsaphelion.size();++i){
    Marsradius.push_back((Marsaphelion[i]+Marsperiphelion[i])/2);
  }
  
  vector<string> MarstexturePath = {"moonmap1k.jpg",
				"moonmap1k.jpg"};
  vector<float> MarsspeedDay = {0.31891,
			    1.26244};
  vector<float> MarsorbitalPeriod = {0.31891,
				 1.26244};
  vector<float> Marsincline = {1.08 ,
			       1.79 };
  for(int i = 0; i < Marsradius.size(); ++i){
    marsSat.push_back(new planet(vec4(0,0,0,Marsdiametre[i]),vec4(Marsaphelion[i]-Marsradius[i]*100000/ratio,Marsincline[i],0,Marsradius[i]/ratio),vec3(1,1,1),vec2(MarsspeedDay[i]*365.5*turnSpeed,MarsorbitalPeriod[i]/365.5*turnSpeed)));
    marsSat[i]->assignShader(gen);
    marsSat[i]->loadTexture(applicationPath->dirPath() + "textures/" + MarstexturePath[i]); 
    //Random start position
    //planetList[i]->rotations = vec2(0,rand());
  }
  
  //////////////////////////jupiter
  vector<float> Jupiterdiametre = {(float)4821/ratio,
				   (float)5262/ratio,
				   (float)3122/ratio,
				   (float)3643/ratio};
  vector<float> Jupiteraphelion = {1870,
				   1068,
				   664,
				   420};
  vector<float> Jupiterperiphelion = {1896 ,
				      1072,
				      678,
				      424};
  vector<float> Jupiterradius;
  for(int i = 0; i < Jupiteraphelion.size();++i){
    Jupiterradius.push_back((Jupiteraphelion[i]+Jupiterperiphelion[i])/2);
  }
  
  vector<string> JupitertexturePath = {"moonmap1k.jpg",
				       "moonmap1k.jpg",
				       "moonmap1k.jpg",
				       "moonmap1k.jpg"};
  vector<float> JupiterspeedDay = {400.5/24,
				   171.7/24,
				   85.2/24,
				   42.5/24};
  vector<float> JupiterorbitalPeriod = {16.7,
					7.2,
					3.6,
					1.8};
  vector<float> Jupiterincline = {0.19 ,
				  0.18 ,	0.47 ,	0.04 };
  for(int i = 0; i < Jupiterradius.size(); ++i){
    jupiterSat.push_back(new planet(vec4(0,0,0,Jupiterdiametre[i]),vec4(Jupiteraphelion[i]-Jupiterradius[i]*1000/ratio,Jupiterincline[i],0,Jupiterradius[i]*1000/ratio),vec3(1,1,1),vec2(JupiterspeedDay[i]*365.5*turnSpeed,JupiterorbitalPeriod[i]/365.5*turnSpeed)));
    jupiterSat[i]->assignShader(gen);
    jupiterSat[i]->loadTexture(applicationPath->dirPath() + "textures/" + JupitertexturePath[i]); 
    //Random start position
    //planetList[i]->rotations = vec2(0,rand());
  }
  ////////////////////saturn
  vector<float> Saturndiametre = {(float)208/ratio,
				   (float)257/ratio,
				   (float)538/ratio,
				  (float)563/ratio,
				  (float)765/ratio};
  vector<float> Saturnaphelion = {185.52,238.02,294.66,377.40,527.04};
  vector<float> Saturnperiphelion = Saturnaphelion;
  vector<float> Saturnradius;
  for(int i = 0; i < Saturnaphelion.size();++i){
    Saturnradius.push_back((Saturnaphelion[i]+Saturnperiphelion[i])/2);
  }
  
  vector<string> SaturntexturePath = {"moonmap1k.jpg",
				       "moonmap1k.jpg",
				       "moonmap1k.jpg",
				      "moonmap1k.jpg",
				      "moonmap1k.jpg"};
  vector<float> SaturnspeedDay = {1,1,1,1,1};
  vector<float> SaturnorbitalPeriod = {0.9424218,1.370218,1.887802,2.736915,4.517500};
  vector<float> Saturnincline = {1.53,0,1.86,0.02,0.35 };
  for(int i = 0; i < Saturnradius.size(); ++i){
    saturnSat.push_back(new planet(vec4(0,0,0,Saturndiametre[i]),vec4(Saturnaphelion[i]-Saturnradius[i]*1000/ratio,Saturnincline[i],0,Saturnradius[i]*1000/ratio),vec3(1,1,1),vec2(SaturnspeedDay[i]*365.5*turnSpeed,SaturnorbitalPeriod[i]/365.5*turnSpeed)));
    saturnSat[i]->assignShader(gen);
    saturnSat[i]->loadTexture(applicationPath->dirPath() + "textures/" + SaturntexturePath[i]); 
    //Random start position
    //planetList[i]->rotations = vec2(0,rand());
  }
  ///////////////////////////////uranus
  vector<float> Urnausdiametre = {(float)234.2 /ratio,
				  (float)577.9 /ratio,
				  (float)584.7 /ratio
				  ,(float)788.9 /ratio
				  ,(float)761.4 /ratio};
  vector<float> Urnausaphelion = {129.90,199.90,266.00,436.30,583.50 };
  vector<float> Urnausperiphelion = Urnausaphelion;
  vector<float> Urnausradius;
  for(int i = 0; i < Urnausaphelion.size();++i){
    Urnausradius.push_back((Urnausaphelion[i]+Urnausperiphelion[i])/2);
  }
  
  vector<string> UrnaustexturePath = {"moonmap1k.jpg",
				       "moonmap1k.jpg",
				       "moonmap1k.jpg",
				      "moonmap1k.jpg",
				      "moonmap1k.jpg"};
  vector<float> UrnausspeedDay = {1,1,1,1,1};
  vector<float> UrnausorbitalPeriod = {1.413479,2.520379,4.144176,8.705867,13.463234};
  vector<float> Urnausincline = {4.34,0.04,0.13,0.08,0.07 };
  for(int i = 0; i < Urnausradius.size(); ++i){
    uranusSat.push_back(new planet(vec4(0,0,0,Urnausdiametre[i]),vec4(Urnausaphelion[i]-Urnausradius[i]/ratio,Urnausincline[i],0,Urnausradius[i]/ratio),vec3(1,1,1),vec2(UrnausspeedDay[i]*365.5*turnSpeed,UrnausorbitalPeriod[i]/365.5*turnSpeed)));
    uranusSat[i]->assignShader(gen);
    uranusSat[i]->loadTexture(applicationPath->dirPath() + "textures/" + UrnaustexturePath[i]); 
    //Random start position
    //planetList[i]->rotations = vec2(0,rand());
  }
  ///////////////////////////////netptune
  vector<float> Neptunediametre = {(float)1353.4 /ratio,(float)170/ratio};
  vector<float> Neptuneaphelion = {354.76 ,5513.4};
  vector<float> Neptuneperiphelion = {354.76,5513.4  };
  vector<float> Neptuneradius;
  for(int i = 0; i < Neptuneaphelion.size();++i){
    Neptuneradius.push_back((Neptuneaphelion[i]+Neptuneperiphelion[i])/2);
  }
  
  vector<string> NeptunetexturePath = {"moonmap1k.jpg","moonmap1k.jpg"};
  vector<float> NeptunespeedDay = {1,1};
  vector<float> NeptuneorbitalPeriod = {5.876854,360.13619};
  vector<float> Neptuneincline = {157.345,7.23 };
  for(int i = 0; i < Neptuneradius.size(); ++i){
    neptuneSat.push_back(new planet(vec4(0,0,0,Neptunediametre[i]),vec4(Neptuneaphelion[i]-Neptuneradius[i]/ratio,Neptuneincline[i],0,Neptuneradius[i]/ratio),vec3(1,1,1),vec2(NeptunespeedDay[i]*365.5*turnSpeed,NeptuneorbitalPeriod[i]/365.5*turnSpeed)));
    neptuneSat[i]->assignShader(gen);
    neptuneSat[i]->loadTexture(applicationPath->dirPath() + "textures/" + NeptunetexturePath[i]);
  }
  //////////////////////////////pluton
  vector<float> Plutondiametre = {(float)1000 /ratio};
  vector<float> Plutonaphelion = {19596  };
  vector<float> Plutonperiphelion = {19596  };
  vector<float> Plutonradius;
  for(int i = 0; i < Plutonaphelion.size();++i){
    Plutonradius.push_back((Plutonaphelion[i]+Plutonperiphelion[i])/2);
  }
  
  vector<string> PlutontexturePath = {"moonmap1k.jpg"};
  vector<float> PlutonspeedDay = {6.3872};
  vector<float> PlutonorbitalPeriod = {6.3872};
  vector<float> Plutonincline = {0.00005 };
  for(int i = 0; i < Plutonradius.size(); ++i){
    plutonSat.push_back(new planet(vec4(0,0,0,Plutondiametre[i]),vec4(Plutonaphelion[i]-Plutonradius[i]/ratio,Plutonincline[i],0,Plutonradius[i]/ratio),vec3(1,1,1),vec2(PlutonspeedDay[i]*365.5*turnSpeed,PlutonorbitalPeriod[i]/365.5*turnSpeed)));
    plutonSat[i]->assignShader(gen);
    plutonSat[i]->loadTexture(applicationPath->dirPath() + "textures/" + PlutontexturePath[i]); 
    //Random start position
    //planetList[i]->rotations = vec2(0,rand());
  }



  ////////////RINGS
  //Both rings are just planets that we scale at Y and rotate the UV in the shader 
  saturnRings = new planet(vec4(0,0,0,diametre[5]*2),vec4(0,0,0,0),vec3(0,0,0),vec2(0,0));
  saturnRings->assignShader(gen);
  saturnRings->loadTexture(applicationPath->dirPath() + "textures/saturnringcolor.jpg");
  ////////////RINGS
  uranusRings = new planet(vec4(0,0,0,diametre[6]*1.5),vec4(0,0,0,0),vec3(0,0,0),vec2(0,0));
  uranusRings->assignShader(gen);
  uranusRings->loadTexture(applicationPath->dirPath() + "textures/uranusringcolour.jpg");
  
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
    //Acceleration of fps
    if(e.key.keysym.sym == SDLK_l){
      //TODO acceleration less
    }
    if(e.key.keysym.sym == SDLK_m){
      //TODO acceleration more
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
    //For the planet view we update the coordinates of each camera by giving a pointer to the current planet we want to focus on
    //I could have used that for the satellite updates too
    if(e.key.keysym.sym == SDLK_KP0){
      cam.p = &sun;
      cam.zoomIn = sun._ballCoords.w*2;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP1){
      cam.p = planetList[0];
      cam.zoomIn =  planetList[0]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP2){
      cam.p = planetList[1];
      cam.zoomIn =  planetList[1]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP3){
      cam.p = planetList[2];
      cam.zoomIn =  planetList[2]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP4){
      cam.p = planetList[3];
      cam.zoomIn =  planetList[3]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP5){
      cam.p = planetList[4];
      cam.zoomIn =  planetList[4]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP6){
      cam.p = planetList[5];
      cam.zoomIn =  planetList[5]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP7){
      cam.p = planetList[6];
      cam.zoomIn =  planetList[6]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP8){
      cam.p = planetList[7];
      cam.zoomIn =  planetList[7]->_ballCoords.w*2.5;
      cam.cam_state = PLANETVIEW;
    }
    if(e.key.keysym.sym == SDLK_KP9){
      cam.p = planetList[8];
      cam.zoomIn =  planetList[8]->_ballCoords.w*2.5;
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
  //Updating planets
  for(int i = 0; i < planetList.size(); ++i){
    planetList[i]->update(delta);
  }
  //Update moon coordinates
  moon._pathCoords.x = planetList[2]->translation.x;
  moon._pathCoords.y = planetList[2]->translation.y;
  moon._pathCoords.z = planetList[2]->translation.z;
  moon.update(delta);
  //Update satelite coordinates
  for(int i = 0; i < marsSat.size();++i){
    marsSat[i]->_pathCoords.x = planetList[3]->translation.x;
    marsSat[i]->_pathCoords.y = planetList[3]->translation.y;
    marsSat[i]->_pathCoords.z = planetList[3]->translation.z;
    marsSat[i]->update(delta);
  }
  for(int i = 0; i < jupiterSat.size();++i){
    jupiterSat[i]->_pathCoords.x = planetList[4]->translation.x;
    jupiterSat[i]->_pathCoords.y = planetList[4]->translation.y;
    jupiterSat[i]->_pathCoords.z = planetList[4]->translation.z;
    jupiterSat[i]->update(delta);
  }
  for(int i = 0; i < saturnSat.size();++i){
    saturnSat[i]->_pathCoords.x = planetList[5]->translation.x;
    saturnSat[i]->_pathCoords.y = planetList[5]->translation.y;
    saturnSat[i]->_pathCoords.z = planetList[5]->translation.z;
    saturnSat[i]->update(delta);
  }
  for(int i = 0; i < uranusSat.size();++i){
    uranusSat[i]->_pathCoords.x = planetList[6]->translation.x;
    uranusSat[i]->_pathCoords.y = planetList[6]->translation.y;
    uranusSat[i]->_pathCoords.z = planetList[6]->translation.z;
    uranusSat[i]->update(delta);
  }
  for(int i = 0; i < neptuneSat.size();++i){
    neptuneSat[i]->_pathCoords.x = planetList[7]->translation.x;
    neptuneSat[i]->_pathCoords.y = planetList[7]->translation.y;
    neptuneSat[i]->_pathCoords.z = planetList[7]->translation.z;
    neptuneSat[i]->update(delta);
  }
  for(int i = 0; i < plutonSat.size();++i){
    plutonSat[i]->_pathCoords.x = planetList[8]->translation.x;
    plutonSat[i]->_pathCoords.y = planetList[8]->translation.y;
    plutonSat[i]->_pathCoords.z = planetList[8]->translation.z;
    plutonSat[i]->update(delta);
  }
  //RINGS
  //Update rings coordinates
  saturnRings->_pathCoords.x = planetList[5]->translation.x;
  saturnRings->_pathCoords.y = planetList[5]->translation.y;
  saturnRings->_pathCoords.z = planetList[5]->translation.z;
  //RINGs
  uranusRings->_pathCoords.x = planetList[6]->translation.x;
  uranusRings->_pathCoords.y = planetList[6]->translation.y;
  uranusRings->_pathCoords.z = planetList[6]->translation.z;
  //saturnRings->update(delta);
  //Update camera coordinates
  cam.update(delta);
  //Update sun coordinates
  sun.update(delta);
}


void app::draw(){
  //Clearing buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //setting lighting 
  glUniform1f(uSpecular,1);
  glUniform1f(uAmbient,0.5);
  glUniform1f(uRotationUV,0);
  //Drawing main planets
  for(int i = 0; i < planetList.size(); ++i){
    /*Drawing a planet*/
    MV =  planetList[i]->ModelMatrix;
    MVP = Projection * cam.viewMatrix * MV;
    Normal =  glm::transpose(glm::inverse(MV));
    vec4 camD = MV * vec4(cam._eyePos,1);
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(vec3(camD)));
    planetList[i]->draw();
  }
  //Satelite drawing
  /*Drawing a planet*/
  MV = cam.viewMatrix * moon.ModelMatrix;
  MVP = Projection * MV;
  Normal =  glm::transpose(glm::inverse(MV));
  
  glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
  glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
  glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
  glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
  moon.draw();
  //Mars
  for(int i = 0; i < marsSat.size();++i){
    //moon.ModelMatrix = translate(moon.ModelMatrix,vec3(1+i,0,0));
    MV = cam.viewMatrix * marsSat[i]->ModelMatrix;
    MVP = Projection * MV;
    Normal =  glm::transpose(glm::inverse(MV));
  
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
    marsSat[i]->draw();
  }
  //Jupiter
  for(int i = 0; i < jupiterSat.size();++i){
    //moon.ModelMatrix = translate(moon.ModelMatrix,vec3(1+i,0,0));
    MV = cam.viewMatrix * jupiterSat[i]->ModelMatrix;
    MVP = Projection * MV;
    Normal =  glm::transpose(glm::inverse(MV));
  
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
    jupiterSat[i]->draw();
  }
  //saturn
  for(int i = 0; i < saturnSat.size();++i){
    //moon.ModelMatrix = translate(moon.ModelMatrix,vec3(1+i,0,0));
    MV = cam.viewMatrix * saturnSat[i]->ModelMatrix;
    MVP = Projection * MV;
    Normal =  glm::transpose(glm::inverse(MV));
  
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
    saturnSat[i]->draw();
  }
  //uranus
  for(int i = 0; i < uranusSat.size();++i){
    //moon.ModelMatrix = translate(moon.ModelMatrix,vec3(1+i,0,0));
    MV = cam.viewMatrix * uranusSat[i]->ModelMatrix;
    MVP = Projection * MV;
    Normal =  glm::transpose(glm::inverse(MV));
  
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
    uranusSat[i]->draw();
  }
  //netptune
  for(int i = 0; i < neptuneSat.size();++i){
    //moon.ModelMatrix = translate(moon.ModelMatrix,vec3(1+i,0,0));
    MV = cam.viewMatrix * neptuneSat[i]->ModelMatrix;
    MVP = Projection * MV;
    Normal =  glm::transpose(glm::inverse(MV));
  
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
    neptuneSat[i]->draw();
  }
  //pluton
  for(int i = 0; i < plutonSat.size();++i){
    //moon.ModelMatrix = translate(moon.ModelMatrix,vec3(1+i,0,0));
    MV = cam.viewMatrix * plutonSat[i]->ModelMatrix;
    MVP = Projection * MV;
    Normal =  glm::transpose(glm::inverse(MV));
  
    glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
    glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
    glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
    plutonSat[i]->draw();
  }
  /*Drawing a planet*/
  glUniform1f(uSpecular,1);
  glUniform1f(uAmbient,1);
  glUniform1f(uRotationUV,0);
  MV = cam.viewMatrix * sun.ModelMatrix;
  MVP = Projection * MV;
  Normal =  glm::transpose(glm::inverse(MV));
  
  glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
  glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
  glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
  glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
  sun.draw();



  //RINGS
  glUniform1f(uSpecular,1);
  glUniform1f(uAmbient,0.5);
  //I forgot to mention this in the report : It's the rotation value for the texture
  /*
    ref of the function : https://gist.github.com/ayamflow/c06bc0c8a64f985dd431bd0ac5b557cd
   */
  glUniform1f(uRotationUV,M_PI/2);
  //moon.ModelMatrix = translate(moon.ModelMatrix,vec3(1+i,0,0));
  mat4 tmp = planetList[5]->ModelMatrix;
  tmp = scale(tmp,vec3(1,0.01,1));
  MV = cam.viewMatrix * tmp;
  MVP = Projection * MV;
  Normal =  glm::transpose(glm::inverse(MV));
  
  glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
  glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
  glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
  glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
  saturnRings->draw();

  //Uranus RINGS
  glUniform1f(uSpecular,0.5);
  glUniform1f(uAmbient,0.5);
  glUniform1f(uRotationUV,M_PI/2);
  //Rotation of the uranus rings two times one for the Y to face the sun and the second to be vertical
tmp = planetList[6]->ModelMatrix;
tmp = glm::rotate(tmp,-radians(planetList[6]->rotations.x),vec3(0,1,0));
tmp = glm::rotate(tmp,radians(90.f),vec3(1,0,1));
  tmp = scale(tmp,vec3(1,0.01,1));
  MV = cam.viewMatrix * tmp;
  MVP = Projection * MV;
  Normal =  glm::transpose(glm::inverse(MV));
  
  glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
  glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
  glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
  glUniformMatrix4fv(uLightDir,1,GL_FALSE,glm::value_ptr(cam._eyePos));
  uranusRings->draw();
  /*Drawing a skybox*/p
  glCullFace(GL_FRONT);  
  MV = cam.viewMatrix * skybox.ModelMatrix;
  MVP = Projection * MV;
  Normal =  glm::transpose(glm::inverse(MV));
  glUniformMatrix4fv(uMVP,1,GL_FALSE,glm::value_ptr(MVP));
  glUniformMatrix4fv(uMV,1,GL_FALSE,glm::value_ptr(MV));
  glUniformMatrix4fv(uNormal,1,GL_FALSE,glm::value_ptr(Normal));
  skybox.draw();
  //Setting cull face to its origin to not affect other plantes
  glCullFace(GL_BACK);
}


