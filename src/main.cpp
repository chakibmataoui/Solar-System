#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include "app.hpp"

using namespace glimac;

FilePath *applicationPath;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
  int w = 800, h = 600;
    SDLWindowManager windowManager(w, h, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;
    //Loading filepath
    applicationPath = new FilePath(argv[0]);
    app solar_sys(windowManager);
    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    solar_sys.init(w,h);
    // Application loop:
    float lastTime = 0;
    float delta = 0;
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
	    solar_sys.handleEvent(e);
        }
	//Send lag time to the solar system app update
	float currentTime = windowManager.getTime();
	delta = currentTime - lastTime;
	lastTime = currentTime;
	solar_sys.update(delta);

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
	//After updating the solar system physics draw the world
	solar_sys.draw();
        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
