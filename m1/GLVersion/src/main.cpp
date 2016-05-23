#include <stdio.h>
#include "glew/glew.h"
#include <SFML/Window.hpp>

#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#endif

float getVer()
{
    float ver = 0.0f;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    //fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    if (GLEW_VERSION_1_1) { ver = 1.1f; }
    if (GLEW_VERSION_1_2) { ver = 1.2f; }
    if (GLEW_VERSION_1_3) { ver = 1.3f; }
    if (GLEW_VERSION_1_4) { ver = 1.4f; }
    if (GLEW_VERSION_1_5) { ver = 1.5f; }
    
    if (GLEW_VERSION_2_0) { ver = 2.0f; }
    if (GLEW_VERSION_2_1) { ver = 2.1f; }
    
    if (GLEW_VERSION_3_0) { ver = 3.0f; }
    if (GLEW_VERSION_3_1) { ver = 3.1f; }
    if (GLEW_VERSION_3_2) { ver = 3.2f; }
    if (GLEW_VERSION_3_3) { ver = 3.3f; }
    
    if (GLEW_VERSION_4_0) { ver = 4.0f; }
    if (GLEW_VERSION_4_1) { ver = 4.1f; }
    if (GLEW_VERSION_4_2) { ver = 4.2f; }
    if (GLEW_VERSION_4_3) { ver = 4.3f; }
    if (GLEW_VERSION_4_4) { ver = 4.4f; }
    if (GLEW_VERSION_4_5) { ver = 4.5f; }
    
    return ver;
}

int main(int argc, char const** argv)
{
    sf::err().rdbuf(NULL); //hide errors
    
#ifdef __APPLE__
    //stop nslog messages
    // http://stackoverflow.com/questions/12608988/hiding-nslog-output-coming-from-precompiled-library
    int nullFD = open("/dev/null", O_WRONLY);
    int oldFD = dup(2); // Duplicate the old file descriptor, so it can be restored
    dup2(nullFD, 2); // Redirect
#endif

    sf::VideoMode mode(100, 100, 32);
#ifdef __linux__
    sf::ContextSettings settings(32, 0, 0, 4, 5); //try for core
#else
    sf::ContextSettings settings(0, 0, 0, 4, 5, sf::ContextSettings::Core); //try for core
#endif
    sf::Window window(mode, "glver", sf::Style::Default, settings);

    float ver = getVer();
    
#ifdef __APPLE__
    dup2(oldFD, 2); // Redirect back
    close(oldFD); // Not needed anymore
#endif
    
    
    if( ver < 1.0f ) {
        printf("OpenGL is not supported.\n");
        return 0;
    }
    
    printf("OpenGL version %.1f is supported.\n", ver);
    return 0;


    return EXIT_SUCCESS;
}
