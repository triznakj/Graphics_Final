#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "GLHelper.h"
#include "WorldState.h"

#define TEX_TYPE GL_TEXTURE_2D
//#define TEX_TYPE GL_TEXTURE_RECTANGLE

class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
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
		
		//glCullFace(GL_BACK);
		//glEnable(GL_CULL_FACE);
		
		setupShader();
		setupBuffers(state.getModel());
	}

	void display(WorldState & state)
	{
		size_t sid;

		///// light view /////
		//activate our new framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		sid = 2;
		glUseProgram(shaderProg[sid]);
		uploadUniforms(shaderProg[sid], state);

		//draw
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, state.getModel().getElements().size(), GL_UNSIGNED_INT, 0);
		checkGLError("light view");
		
		
		
		///// light point /////
		//default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sid = 1;
		glUseProgram(shaderProg[sid]);
		uploadUniforms(shaderProg[sid], state);
		
		glBindVertexArray(lightArray);
		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
		checkGLError("light point");
		
		

		///// model /////
		sid = 0;
		glUseProgram(shaderProg[sid]);
		uploadUniforms(shaderProg[sid], state);
		
		GLuint texId = 0;
		glActiveTexture(GL_TEXTURE0+texId);
		glBindTexture(TEX_TYPE, renderTexture);
		glUniform1i( glGetUniformLocation(shaderProg[sid], "texId"), texId);
		
		//draw
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, state.getModel().getElements().size(), GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("model");
	}
	
	void buildRenderBuffers(size_t xSize, size_t ySize)
	{
		mapSizeX = xSize;
		mapSizeY = ySize;
		
		
		if(renderTexture != 0) {
			glDeleteTextures(1, &renderTexture);
			glDeleteRenderbuffers(1, &renderBuffer);
		}
		
		//setup texture
		glGenTextures(1, &renderTexture);
		glBindTexture(TEX_TYPE, renderTexture);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		//framebuffer
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		
		//make renderbuffer for depth, attach to FB
		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mapSizeX, mapSizeY);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		
		//make texture, attach to FB
		glTexImage2D(TEX_TYPE, 0, GL_RGBA, mapSizeX, mapSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
		GLenum colorBuffer = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &colorBuffer);
		
		
		checkGLError("shadow buffer");
		checkGLFrameBuffer(GL_FRAMEBUFFER, "shadow buffer");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


private:
	bool initialized;
	GLuint shaderProg[3];
	GLuint vertexArray;
	GLuint quadVertexArray;
	GLuint lightArray;
	GLuint frameBuffer;
	GLuint renderTexture;
	GLuint renderBuffer;
	size_t mapSizeX;
	size_t mapSizeY;

	glm::vec2 size;
	
	float _near;
	float _far;
	float fov;

	
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
		char const * vertPath = "resources/reflectance.vert";
		char const * fragPath = "resources/reflectance.frag";
		shaderProg[0] = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);
		
		char const * lightVPath = "resources/lightPos.vert";
		char const * lightFPath = "resources/lightPos.frag";
		shaderProg[1] = ShaderManager::shaderFromFile(&lightVPath, &lightFPath, 1, 1);
		
		char const * shdVPath = "resources/shadowMap.vert";
		char const * shdFPath = "resources/shadowMap.frag";
		shaderProg[2] = ShaderManager::shaderFromFile(&shdVPath, &shdFPath, 1, 1);

		checkGLError("shader");
	}

	void setupBuffers(Model & model)
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		
		GLuint positionBuffer;
		GLuint colorBuffer;
		GLuint elementBuffer;
		GLint colorSlot = 1;
		GLint positionSlot = 0;
		
		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes(), &model.getPosition()[0], GL_STATIC_DRAW);
		//positionSlot = glGetAttribLocation(shaderProg[0], "pos");
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Do the same thing for the color data
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getColorBytes(), &model.getColor()[0], GL_STATIC_DRAW);
		//colorSlot =    glGetAttribLocation(shaderProg[0], "colorIn");
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getElementBytes(), &model.getElements()[0], GL_STATIC_DRAW);
		//leave the element buffer active
		
		//quad for render to texture
		static const GLfloat quadVertexData[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
		};
		
		glGenVertexArrays(1, &quadVertexArray);
		glBindVertexArray(quadVertexArray);
		GLuint quadVertexBuffer;
		glGenBuffers(1, &quadVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		//hacky way to draw the light
		glGenVertexArrays(1, &lightArray);
		glBindVertexArray(lightArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBindVertexArray(0);

		checkGLError("setup");
	}

	void uploadUniforms(GLuint shaderId, WorldState const & state)
	{
		glm::vec3 dim = state.getModel().getDimension();
		float maxDim = std::max(dim[0], std::max(dim[1], dim[2]));
		_near = maxDim*0.1f;
		_far  = maxDim*3.0f;
		fov = 0.9f;
		
		glm::mat4 P = glm::perspective(1.0f, fov, _near, _far);
		glm::mat4 mT = state.getModelTranslate();
		glm::mat4 mR = state.getModelRotate();
		glm::mat4 C = state.getCameraMatrix();
		glm::mat4 M = C*mR*mT;
		glm::mat3 N = glm::inverseTranspose(glm::mat3(M));
		glm::vec4 lightPos = state.getLightPos();
		glm::vec4 camPos = state.getCameraPos();
		glm::mat4 Lr = state.getLightRotate();
		glm::mat4 Lv = state.getLightViewMatrix();
		glm::mat4 Lp;
		Lp = glm::perspective(1.0f, fov, _near, _far);
		Lp = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 50.0f);

		//hacky light source size change
		GLfloat maxDis = state.getModel().getDimension()[2] * 3;
		GLfloat distScale = 1.0f / (glm::length(Lr*lightPos - camPos) / maxDis);
		glPointSize(glm::mix(1.0f, 10.0f, distScale));

		//printf("cam %f %f %f\n", camPos[0], camPos[1], camPos[2]);
		//printf("light %f %f %f\n", lightPos[0], lightPos[1], lightPos[2]);
		
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lp"), 1, GL_FALSE, &Lp[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "C"), 1, GL_FALSE, &C[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "mR"), 1, GL_FALSE, &mR[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "mT"), 1, GL_FALSE, &mT[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "M"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix3fv(glGetUniformLocation(shaderId, "N"), 1, GL_FALSE, &N[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lr"), 1, GL_FALSE, &Lr[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lv"), 1, GL_FALSE, &Lv[0][0]);
		glUniform4fv(glGetUniformLocation(shaderId, "lightPos"), 1, &lightPos[0]);
		glUniform4fv(glGetUniformLocation(shaderId, "camPos"), 1, &camPos[0]);

//		glUniform1f(glGetUniformLocation(shaderId, "elapsedTime"), state.currentTime);
//		glUniform1f(glGetUniformLocation(shaderId, "near"), _near);
//		glUniform1f(glGetUniformLocation(shaderId, "far"), _far);
//		glUniform1f(glGetUniformLocation(shaderId, "fov"), fov);
//		glUniform1f(glGetUniformLocation(shaderId, "cursorScrollAmount"), state.cursorScrollAmount);
		glUniform2f(glGetUniformLocation(shaderId, "resolution"), state.currentRes[0], state.currentRes[1]);
//		glUniform3f(glGetUniformLocation(shaderId, "modelCenter"),  state.center[0], state.center[1], state.center[2]);
//		glUniform3f(glGetUniformLocation(shaderId, "lookAtPos"),  state.cameraLook[0], state.cameraLook[1], state.cameraLook[2]);
//		glUniform3f(glGetUniformLocation(shaderId, "cameraUp"),  state.cameraUp[0], state.cameraUp[1], state.cameraUp[2]);
//		glUniform2f(glGetUniformLocation(shaderId, "cursorAbsolutePos"), state.cursorAbsolutePos[0], state.cursorAbsolutePos[1]);
//		glUniform2f(glGetUniformLocation(shaderId, "cursorDragAmount"), state.cursorDragAmount[0], state.cursorDragAmount[1]);
//		glUniform2f(glGetUniformLocation(shaderId, "lastClickPos"), state.lastClickPos[0], state.lastClickPos[1]);
//		glUniform2f(glGetUniformLocation(shaderId, "lastFrameDragPos"), state.lastFrameDragPos[0], state.lastFrameDragPos[1]);
//		glUniform1i(glGetUniformLocation(shaderId, "mouseButtonDown"), state.mouseButtonDown);
	}

};

#endif