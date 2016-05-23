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
		
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
        
		glm::vec3 dim = state.getModel().getDimension();
		float maxDim = std::max(dim[0], std::max(dim[1], dim[2]));
		this->P = glm::perspective(90.0f, 1.0f, 0.1f, 5000.0f);
        C = state.getCameraMatrix();

		setupTextures();
		setupShader();
		setupBuffers(state.getModel());
	}

	void display(WorldState & state)
	{
		//glEnable(GL_DEPTH_TEST);

		//clear the old frame
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Model & model = state.getModel();

		C = state.getCameraMatrix();

		//use shader
		glUseProgram(shaderProg[0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "C"), 1, GL_FALSE, &C[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "P"), 1, GL_FALSE, &P[0][0]);
		glBindVertexArray(vertexArray[0]);

		size_t numQuads = model.getVertexCount()/6;
		for(int i=0; i<numQuads; i++)
		{
			//TODO bind the texture object
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			//TODO activate a texture unit
			GLint texUnitId = 0;
			glActiveTexture(GL_TEXTURE0 + texUnitId);
			//TODO bind the sampler to a texture unit
			glUniform1i(glGetUniformLocation(shaderProg[0], "texUnit"), texUnitId);
			glDrawArrays(GL_TRIANGLES, 6*i, 6);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		bool drawWireframe = true;
		if(drawWireframe) {
			glUseProgram(shaderProg[1]);
			glBindVertexArray(vertexArray[1]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "C"), 1, GL_FALSE, &C[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "P"), 1, GL_FALSE, &P[0][0]);

			for(int i=0; i<numQuads*2; i++)
			{
				glDrawArrays(GL_LINE_LOOP, 3*i, 3);
			}
		}

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
		char const * wireFragPath = "resources/wireframe.frag";
		shaderProg[0] = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);
		shaderProg[1] = ShaderManager::shaderFromFile(&vertPath, &wireFragPath, 1, 1);

		checkGLError("shader");
	}

	void setupTextures()
	{
		//generate texture names
		glGenTextures(4, textures);
		sf::Image image;

		char const * imagePaths[4] = {"resources/rhit1.png", "resources/rhit2.png", "resources/rhit3.png", "resources/rhit4.png"};

		for(int i=0; i<4; i++)
		{
			if (!image.loadFromFile(imagePaths[i])) {
				fprintf(stderr, "Could not load: %s\n", imagePaths[i]);
				exit(2);
			}
			int texSizeX = image.getSize().x;
			int texSizeY = image.getSize().y;
			unsigned char * texData = (unsigned char*) image.getPixelsPtr();

			//TODO bind a texture object
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			//TODO set min/mag sampling parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//TODO set edge wrap parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//TODO upload texture and generate mipmap
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSizeX, texSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
			bool  mipmapEnabled = true;
			if(mipmapEnabled)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
				//mip mapping, upload 0 level, then build maps
			}
			else
			{
				//no mip mapping, upload 0 level only
			}

			//TODO unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		checkGLError("texture");
	}

	void setupBuffers(Model & model)
	{
		GLuint positionBuffer;
		GLuint texCoordBuffer;

		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes(), &model.getPosition()[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// now the texture coords
		glGenBuffers(1, &texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getTexCoordBytes(), &model.getTexCoord()[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//setup the vertex array
		glGenVertexArrays(2, vertexArray);

		//first one for texture rendering
		{
			GLint positionSlot = glGetAttribLocation(shaderProg[0], "pos");
			GLint texCoordSlot = glGetAttribLocation(shaderProg[0], "texCoord");
			glBindVertexArray(vertexArray[0]);
			glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
			glEnableVertexAttribArray(positionSlot);
			glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
			glEnableVertexAttribArray(texCoordSlot);
			glVertexAttribPointer(texCoordSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindVertexArray(0);
		}

		//second one for wireframe rendering
		{
			GLint positionSlot = glGetAttribLocation(shaderProg[1], "pos");
			GLint texCoordSlot = glGetAttribLocation(shaderProg[1], "texCoord");
			glBindVertexArray(vertexArray[1]);
			glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
			glEnableVertexAttribArray(positionSlot);
			glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
			glEnableVertexAttribArray(texCoordSlot);
			glVertexAttribPointer(texCoordSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindVertexArray(0);
		}

		checkGLError("setup");
	}
};

#endif