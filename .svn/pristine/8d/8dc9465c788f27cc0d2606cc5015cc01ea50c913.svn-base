#define USER1 "put_username1_here"
#define USER2 "put_username2_here"

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include "glew/glew.h"
#include <SFML/Window.hpp>
#include "WorldState.h"
#include "GraphicsHelpers.h"

class GLBox
{
private:
	void handleEvents(sf::Window & window, WorldState & state)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				state.setRunning(false);
			
			// Escape key : exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				state.setRunning(false);
			
			//TODO1: for text event with code in the range [' ', 'z'], have the state print the unicode character
			//I strongly recommend the SFML event tutorial
			//http://www.sfml-dev.org/tutorials/2.3/window-events.php

			
			//TODO2: if the mouse moves, have the state print the cursor position

		}

		//TODO3: if any arrow keys are down, tell the state to 'move' in that direction
		//This should not use events! It should check the current state of the key each frame
		//Check the tutorial on input state:
		//http://www.sfml-dev.org/tutorials/2.3/window-inputs.php


		//TODO4: do the todos in the run loop
	}
	
public:
	GLBox()
	{
		//you can add variables here if you need



		// Create the main window
		sf::Window window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program1");
		WorldState state;
		graphicsInit();
		
		// Start render loop
		while (state.isRunning())
		{		
			// Handle any events that are in the queue
			this->handleEvents(window, state);
			
			//TODO5: track the time since last render and then tell the state
			//A tutorial on handling SFML time:
			//http://www.sfml-dev.org/tutorials/2.3/system-time.php

            
            
			
			//TODO6: every 2.0 seconds, have the state print the fps
			// you might want to track the total runtime

            
            
			

			display();
			window.display();
		}
		window.close();
	}
	
	~GLBox()
	{
		glDeleteBuffers(1, &positionBuffer);
	}
private:
	sf::Window *App;
};

int main()
{
	//printf("Program by %s+%s\n", USER1, USER2);
	GLBox prog;
	
    return EXIT_SUCCESS;
}
