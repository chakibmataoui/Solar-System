#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include "app.hpp"

using namespace glimac;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    app solar_sys(windowManager);
    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    solar_sys.init();
    // Application loop:
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
	solar_sys.update();

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
	solar_sys.draw();
        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
