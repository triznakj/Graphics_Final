#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "GLHelper.h"
#include "WorldState.h"

class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
		
		//camera
        this->P = glm::perspective(1.0f, 1.0f, 0.1f, 100.0f);
        this->C = glm::mat4(1);
		this->M = glm::mat4(1);
	}

	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			//glDeleteBuffers(1, &positionBuffer);
			//glDeleteBuffers(1, &colorBuffer);
		}
	}

	void init(WorldState & state)
	{
		initialized = true;

		float ver = initLoader();
		if( ver < 1.0f ) {
			printf("OpenGL is not supported.\n");
			exit(1);
		}
		printf("OpenGL version %.1f is supported.\n", ver);
        
		glm::vec3 dim = state.getModel().getDimension();
		float maxDim = std::max(dim[0], std::max(dim[1], dim[2]));
		this->P = glm::perspective(90.0f, 1.0f, 0.1f, 5000.0f);
        C = state.getCameraMatrix();

		setupShader();
		setupBuffers(state.getModel());
	}

	void display(WorldState & state)
	{
		//TODO1: Your goal is to allow the materials to blend and make a grey region
		// You must also find a way to hide the black quad
		// Rules:
		// You must draw all four quads! (see the draw calls below)
		// You may add commands to only this function
		// You may reorder commands in this function
		// You may not remove or skip any calls in this or any other function
		

		//TODO2: Depth might be helpful
		glEnable(GL_DEPTH_TEST);

		//TODO3: Need to cull black square...
		//Hint: it's facing a different direction than all the other ones

		//TODO4: No blending yet, so enable blending and set the blend function
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//clear the old frame
		glViewport(0,0,state.currentRes[0],state.currentRes[1]);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Model & model = state.getModel();

		C = state.getCameraMatrix();

		//use shader
		glUseProgram(shaderProg[0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "C"), 1, GL_FALSE, &C[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "P"), 1, GL_FALSE, &P[0][0]);
		glBindVertexArray(vertexArray[0]);

		//TODO5: Arrange the shapes so they blend correctly
		drawBlueQuad();
		drawGreenQuad();
		drawRedQuad();
		
		drawBlackQuad();
		
		
		

		//cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("display");
	}


private:
	bool initialized;
	GLuint shaderProg[2];
	GLuint vertexArray[2];
	GLuint textures[4];

	glm::mat4 P;
	glm::mat4 C;
	glm::mat4 M;

	
	float initLoader()
	{
		float ver = 0.0f;
#ifdef GLEW
		glewExperimental = GL_TRUE;
		
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		
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
#endif
		
#ifdef GL3W
		if (gl3wInit()) {
			fprintf(stderr, "failed to initialize OpenGL\n");
		}
		
		if (gl3wIsSupported(1, 1)) { ver = 1.1f; }
		if (gl3wIsSupported(1, 2)) { ver = 1.2f; }
		if (gl3wIsSupported(1, 3)) { ver = 1.3f; }
		if (gl3wIsSupported(1, 4)) { ver = 1.4f; }
		if (gl3wIsSupported(1, 5)) { ver = 1.5f; }
		
		if (gl3wIsSupported(2, 0)) { ver = 2.0f; }
		if (gl3wIsSupported(2, 1)) { ver = 2.1f; }
		
		if (gl3wIsSupported(3, 0)) { ver = 3.0f; }
		if (gl3wIsSupported(3, 1)) { ver = 3.1f; }
		if (gl3wIsSupported(3, 2)) { ver = 3.2f; }
		if (gl3wIsSupported(3, 3)) { ver = 3.3f; }
		
		if (gl3wIsSupported(4, 0)) { ver = 4.0f; }
		if (gl3wIsSupported(4, 1)) { ver = 4.1f; }
		if (gl3wIsSupported(4, 2)) { ver = 4.2f; }
		if (gl3wIsSupported(4, 3)) { ver = 4.3f; }
		if (gl3wIsSupported(4, 4)) { ver = 4.4f; }
		if (gl3wIsSupported(4, 5)) { ver = 4.5f; }
#endif
		
		return ver;
	}

	void setupShader()
	{
		char const * vertPath = "resources/simple.vert";
		char const * fragPath = "resources/simple.frag";
		shaderProg[0] = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);

		checkGLError("shader");
	}

	void setupBuffers(Model & model)
	{
		GLuint positionBuffer;
		GLuint colorBuffer;

		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes(), &model.getPosition()[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// now the texture coords
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getColorBytes(), &model.getColor()[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//setup the vertex array
		glGenVertexArrays(2, vertexArray);

		{
			GLint positionSlot = glGetAttribLocation(shaderProg[0], "pos");
			GLint colorSlot = glGetAttribLocation(shaderProg[0], "vertColor");
			glBindVertexArray(vertexArray[0]);
			glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
			glEnableVertexAttribArray(positionSlot);
			glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glEnableVertexAttribArray(colorSlot);
			glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glBindVertexArray(0);
		}

		checkGLError("setup");
	}

	void drawQuad(size_t quadId)
	{
		glDrawArrays(GL_TRIANGLES, 6*quadId, 6);
	}

	void drawBlackQuad()
	{ drawQuad(0); }

	void drawRedQuad()
	{ drawQuad(1); }

	void drawGreenQuad()
	{ drawQuad(2); }

	void drawBlueQuad()
	{ drawQuad(3); }
};

#endif