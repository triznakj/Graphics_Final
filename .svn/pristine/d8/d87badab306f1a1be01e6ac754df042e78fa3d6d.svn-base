#define USER1 "triznakj"
#define USER2 "nobody"

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include "glew/glew.h"
#include <SFML/Window.hpp>
#include "simplePNG.h"
#define RESOLUTION 512

GLuint shaderProg;

GLuint positionBuffer;
GLuint colorBuffer;

GLuint positionSlot;
GLuint colorSlot;

GLfloat positions[32] = {-0.5,-.75, -0.5,-.5, -0.5,-.25, //points (0-2)
			 0,0.5, 0.5,1, //line (3-4)
			 0,-1, .5,0, 1,-1, //triangle (5-7)
			-.75,.5, -.6,.6, -.4,.6, -.25,.5,//trapezoid (8-11)
			-.9,.5, -.6, .7, -.4, .7, -.1, .5};//half trap (12-15)
GLfloat colors[48] = {1,1,.1, 0.5,0.5,0.5, 1,0,0, 0,1,0, 0,0,1,
		     1,1,1, 0,.5,1, 1,.5,0, .5,.5,.75,
		     .6,.6,.6, .8,.4,.4, .9,.8,.7,
		     .5,.5,.5, .6,.9,.3, .2,.6,.75, 1,0,0};

void printProgramLinkLog(GLuint obj);
void printProgramCompileLog(GLuint obj);

/*
 Draw a single frame
 */
void display()
{
	// Clear the color bits in the display buffer
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Use a simple shader to render the line
	glUseProgram(shaderProg);
	
	// Render using vertex attributes (data already on GPU) (~2008, 3.0)
	// https://web.archive.org/web/20150225192608/http://www.arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
	
	// Tell OpenGL we want to use a buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	
	// Tell OpenGL what shader data slot we want to use
	glEnableVertexAttribArray(positionSlot);
	
	// Tell OpenGL how to interpret the data
	glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Do the same thing for colors
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glEnableVertexAttribArray(colorSlot);
	glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Draw some primitives as: glDrawArrays(type, first, count)
	glDrawArrays(GL_LINES, 3, 2);
	glDrawArrays(GL_POINTS, 0, 1);
	glDrawArrays(GL_POINTS, 1, 1);
	glDrawArrays(GL_POINTS, 2, 1);
	glDrawArrays(GL_TRIANGLES, 5, 3);
	glDrawArrays(GL_LINE_LOOP, 8, 4);
	glDrawArrays(GL_LINE_STRIP, 12, 4);
	
	// Tell OpenGL we are done with the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Tell OpenGL we are done with the shader slot
	glDisableVertexAttribArray(positionSlot);
	glDisableVertexAttribArray(colorSlot);
	
	// Tell OpenGL we are done with the shader
	glUseProgram(0);
}

/*
 Initialize the graphics state
 */
void graphicsInit()
{
	// glew will help us use GL functions, so set it up here
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	
	// Shaders are programs that do the actual rendering on the GPU
	// We will discuss these in detail later, for now, just set them up
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	char const * vertSource = "attribute vec2 pos; attribute vec3 color; varying vec4 smoothColor; void main() { gl_Position=vec4(pos.xy, 0, 1); smoothColor=vec4(color.xyz, 1); }";
	char const * fragSource = "varying vec4 smoothColor; void main() { gl_FragColor = smoothColor; }";
	
	glShaderSource(vertShader, 1, (char const **)&vertSource, NULL);
	glShaderSource(fragShader, 1, (char const **)&fragSource, NULL);
	
	glCompileShader(vertShader);
	printProgramCompileLog(vertShader);
	glCompileShader(fragShader);
	printProgramCompileLog(fragShader);
	
	shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertShader);
	glAttachShader(shaderProg, fragShader);
	
	glLinkProgram(shaderProg);
	printProgramLinkLog(shaderProg);
	
	// The data we will render needs to be on the GPU
	// These commands upload the data
	
	// Find out where the shader expects the data
	positionSlot = glGetAttribLocation(shaderProg, "pos");
	colorSlot = glGetAttribLocation(shaderProg, "color");

	// Generate a GPU side buffer
	glGenBuffers(1, &positionBuffer);
	
	// Tell OpenGL we want to work with the buffer we just made
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	
	// Allocate and upload data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	
	// Do the same thing for the color data
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void saveBuffer(sf::Window const & window)
{
    unsigned char *dest;
    unsigned int w = window.getSize().x;
    unsigned int h = window.getSize().y;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    dest = (unsigned char *) malloc( sizeof(unsigned char)*w*h*3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
    
    simplePNG_write("_program1.png", w, h, dest);
    free(dest);
}

class GLBox
{
public:
	GLBox()
	{
		// Create the main window
		sf::Window window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program1");
		
		graphicsInit();
        unsigned int frameCount = 0;
		
		// Start render loop
		while (window.isOpen())
		{			
			// Set the active window before using OpenGL commands
			// It's not needed here because the active window is always the same,
			// but don't forget it if you use multiple windows or controls
            window.setActive();
			
			// Handle any events that are in the queue
			sf::Event event;
			while (window.pollEvent(event))
			{
				// Close window : exit
				//if (event.type == sf::Event::Closed)
				//	window.close();
				
				// Escape key : exit
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window.close();
				
				// This is for grading your code. DO NOT REMOVE
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Equal)
				{
                    saveBuffer(window);
				}
			}
			
			// Render the scene
			display();
            frameCount++;
            if(frameCount == 100)
                saveBuffer(window);
			
			// Finally, display rendered frame on screen
			window.display();
		}
	}
	
	~GLBox()
	{
		// Clean up the buffer
		glDeleteBuffers(1, &positionBuffer);
	}
private:
	sf::Window *App;
};

void printProgramCompileLog(GLuint obj)
{
	GLint infologLength;
	GLint status;
	int charsWritten = 0;
	char *infoLog;
	
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
		return;
	
	printf("Error compiling shader: ");
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s",infoLog);
		free(infoLog);
	}
	printf("\n");
}

void printProgramLinkLog(GLuint obj)
{
	GLint infologLength;
	GLint status;
	char *infoLog;
	int charsWritten  = 0;
	
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
		return;
	
	printf("Error linking shader: ");
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s",infoLog);
		free(infoLog);
	}
	printf("\n");
}

int main()
{
	printf("Program by %s+%s\n", USER1, USER2);
	GLBox prog;
	
    return EXIT_SUCCESS;
}
