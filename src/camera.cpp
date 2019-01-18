#include "camera.hpp"

camera::camera(vec3 eyePos,vec3 eyeLook, vec3 upVec,float speed) : _eyePos(eyePos) , _eyeLook(eyeLook), _upVec(upVec),_speed(speed){
  
  viewMatrix = lookAt(_eyePos,_eyeLook,_upVec);
  cam_state = FREEVIEW;
  cameraFront = vec3(0,0,-1);
}

void camera::update(float delta){
  float speed = _speed*delta;
  if(cam_state == FREEVIEW){
    //Movements here for the free fly just compute the new vector to point to according to the speed of rotation
    if(movState.moveLeft){
      _eyePos -= normalize(cross(cameraFront,_upVec))*speed;
    }
    if(movState.moveRight){
      _eyePos += normalize(cross(cameraFront,_upVec))*speed;
    }
    if(movState.moveForward){
       _eyePos += speed * cameraFront;
    }
    if(movState.moveBackward){
       _eyePos -= speed * cameraFront;
    }
    //Up and Down moements
    if(movState.moveUp){
      _eyePos += speed * normalize(cross(cameraFront,vec3(0,0,-1)))*speed;
    }
    if(movState.moveDown){
       _eyePos -= speed * normalize(cross(cameraFront,vec3(0,0,-1)))*speed;
    }
    //Rotation turn
    if(movState.turnLeft){
      _eyeLook.x-=radians(speed);
    }
    if(movState.turnRight){
      _eyeLook.x+=radians(speed);
    }
    if(movState.turnUp){
      _eyeLook.y+=radians(speed);
    }
    if(movState.turnDown){
      _eyeLook.y-=radians(speed);
    }
    //Compute the new eye look coords vector
    vec3 front = vec3(cos(_eyeLook.x) * cos(_eyeLook.y),
		    sin(_eyeLook.y),
		    sin(_eyeLook.x) * cos(_eyeLook.y));
    cameraFront = normalize(front);
    viewMatrix = lookAt(_eyePos,_eyePos + cameraFront,_upVec);
  }
  if(cam_state == PLANETVIEW){
    if(movState.turnLeft){
      angleAround-=radians(speed);
    }
    if(movState.turnRight){
      angleAround+=radians(speed);
    }
    if(movState.turnUp){
      zoomIn -= speed;
    }
    if(movState.turnDown){
      zoomIn += speed;
    }
    //Simple rotation based on the position of the planet 
    vec3 front = vec3(cos(angleAround) * (p->_ballCoords.w + zoomIn) + p->translation.x,
		    p->translation.y,
		    sin(angleAround)* (p->_ballCoords.w + zoomIn) + p->translation.z);
     viewMatrix = lookAt(front,p->translation,_upVec);
  }
  if(cam_state == TOPVIEW){
    if(movState.turnLeft){
      angleAround-=radians(speed);
    }
    if(movState.turnRight){
      angleAround+=radians(speed);
    }
    if(movState.turnUp){
      zoomIn -= speed;
    }
    if(movState.turnDown){
      zoomIn += speed;
    }
    vec3 front = vec3(cos(angleAround) * ( zoomIn) ,
		      200,
		      sin(angleAround)* ( zoomIn) );
    viewMatrix = lookAt(front,vec3(0,0,0),_upVec);
  }
  if(cam_state == SIDEVIEW){
    if(movState.turnLeft){
      angleAround-=radians(speed);
    }
    if(movState.turnRight){
      angleAround+=radians(speed);
    }
    if(movState.turnUp){
      zoomIn -= speed;
    }
    if(movState.turnDown){
      zoomIn += speed;
    }
    vec3 front = vec3(cos(angleAround) * ( zoomIn) ,
		      0,
		      sin(angleAround)* ( zoomIn) );
    viewMatrix = lookAt(front,vec3(0,0,0),_upVec);
    
  }
}
