#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "GLHelper.h"
#include "WorldState.h"

//#define TEX_TYPE GL_TEXTURE_2D
#define TEX_TYPE GL_TEXTURE_RECTANGLE

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
		buildRenderBuffers(state.getModel());
	}
	
	void simulatePaticles(WorldState & state, size_t & sourceSet)
	{
		size_t shaderId = 0;
		///// particle update /////
		glViewport(0, 0, mapSizeX, mapSizeY);
		shaderId = 0;
		glUseProgram(shaderProg[shaderId]);
		uploadUniforms(shaderProg[shaderId], state);
		activateIOBuffers(sourceSet, shaderId);
		
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(vertexArray);
		glPointSize(1.0f); //this has to be 1.0, since we draw points into the textures to update
		glDrawArrays(GL_POINTS, 0, state.getModel().getVertexCount());
		checkGLError("particle update");
		
		sourceSet = !sourceSet;
	}

	void display(WorldState & state)
	{
		
		size_t shaderId = 0;
		static size_t sourceSet = 0;
		
		//TODO check out the textures
		drawTexture(renderTextures[2]);
//		return;

		///// particle update /////
		simulatePaticles(state, sourceSet);
		
		//TODO check out the textures after update
		drawTexture(renderTextures[2]);
//		return;

		///// normal model draw /////
		glViewport(0, 0, state.currentRes[0], state.currentRes[1]);
		shaderId = 1;
		glUseProgram(shaderProg[shaderId]);
		uploadUniforms(shaderProg[shaderId], state);
		activateIOBuffers(sourceSet, shaderId);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPointSize(2.0f);
		
		glBindVertexArray(vertexArray);
		glDrawArrays(GL_POINTS, 0, state.getModel().getVertexCount());
		
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("particle draw");
	}
	
	void buildRenderBuffers(Model const & model)
	{
		mapSizeX = PARTICLE_SIZE_X;
		mapSizeY = PARTICLE_SIZE_Y;
		printf("Particle texture %ux%u\n", mapSizeX, mapSizeY);

		//setup buffers
		glGenFramebuffers(2, frameBuffers);
		glGenTextures(4, renderTextures);
		
		for(size_t b=0; b<2; b++) {
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[b]);
			
			printf("Buffer %u (%u) textures: ", b, frameBuffers[b]);
			//make renderbuffer for depth, attach to FB
			GLuint renderBuffer;
			glGenRenderbuffers(1, &renderBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mapSizeX, mapSizeY);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
			
			{
				GLuint attachPoint;
				GLuint texId;
				
				attachPoint = 0;
				texId = b*2+attachPoint;
				printf("%u ", texId);
				glBindTexture(TEX_TYPE, renderTextures[texId]);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(TEX_TYPE, 0, GL_RGBA32F, mapSizeX, mapSizeY, 0, GL_RGBA, GL_FLOAT, &model.getPosition()[0]);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+attachPoint, renderTextures[texId], 0);
				
				attachPoint = 1;
				texId = b*2+attachPoint;
				printf("%u ", texId);
				//setup texture
				glBindTexture(TEX_TYPE, renderTextures[texId]);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				
				//make texture, attach to FB
				glTexImage2D(TEX_TYPE, 0, GL_RGBA32F, mapSizeX, mapSizeY, 0, GL_RGBA, GL_FLOAT, &model.getVelocities()[0]);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+attachPoint, renderTextures[texId], 0);
				
				GLenum colorBuffers[2] = {GL_COLOR_ATTACHMENT0+0, GL_COLOR_ATTACHMENT0+1};
				glDrawBuffers(2, colorBuffers);
			}
			
			printf("\n");
			
			checkGLError("particle buffer");
			checkGLFrameBuffer(GL_FRAMEBUFFER, "particle framebuffer");
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


private:
	bool initialized;
	GLuint shaderProg[2];
	GLuint textureShader;
	GLuint vertexArray;
	GLuint quadVertexArray;
	GLuint frameBuffers[2];
	GLuint renderTextures[4];
	size_t mapSizeX;
	size_t mapSizeY;

	glm::vec2 size;
	
	float _near;
	float _far;
	float fov;

	void drawTexture(GLuint texId)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(textureShader);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_RECTANGLE, texId);
		glUniform1i( glGetUniformLocation(textureShader, "texId"), 0);
		
		//draw a quad that fills the entire view
		glBindVertexArray(quadVertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("render texture");
	}
	
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
		char const * vPath;
		char const * fPath;
		vPath = "resources/particleUpdate.vert";
		fPath = "resources/particleUpdate.frag";
		shaderProg[0] = ShaderManager::shaderFromFile(&vPath, &fPath, 1, 1);
		
		vPath = "resources/particleDraw.vert";
		fPath = "resources/particleDraw.frag";
		shaderProg[1] = ShaderManager::shaderFromFile(&vPath, &fPath, 1, 1);
		
		vPath = "resources/showTex.vert";
		fPath = "resources/showTex.frag";
		textureShader = ShaderManager::shaderFromFile(&vPath, &fPath, 1, 1);

		checkGLError("shader");
	}

	void setupBuffers(Model & model)
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		
		GLuint idBuffer;
		GLint idSlot = 0;
		
		//upload element ids only
		glGenBuffers(1, &idBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, idBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getElementBytes(), &model.getElements()[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(idSlot);
		glVertexAttribPointer(idSlot, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		//quad for render to texture
		static const GLfloat quadVertexData[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
		};
		checkGLError("id buffer");
		
		glGenVertexArrays(1, &quadVertexArray);
		glBindVertexArray(quadVertexArray);
		GLuint quadVertexBuffer;
		glGenBuffers(1, &quadVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(glGetAttribLocation(textureShader, "pos"));
		glVertexAttribPointer(glGetAttribLocation(textureShader, "pos"), 3, GL_FLOAT, GL_FALSE, 0, 0);

		checkGLError("quad buffer");
	}

	void uploadUniforms(GLuint shaderId, WorldState const & state)
	{
		glm::vec3 dim = state.getModel().getDimension();
		float maxDim = std::max(dim[0], std::max(dim[1], dim[2]));
		_near = maxDim*0.1f;
		_far  = maxDim*10000.0f;
		fov = 1.0f;
		
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
		
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "M"), 1, GL_FALSE, &M[0][0]);
		//glUniform2f(glGetUniformLocation(shaderId, "resolution"), state.currentRes[0], state.currentRes[1]);
		glUniform2ui(glGetUniformLocation(shaderId, "mapSize"), mapSizeX, mapSizeY);
		glUniform1ui(glGetUniformLocation(shaderId, "increaseVelocity"), state.getIncreaseVelocity());
	}
	
	void activateIOBuffers(size_t sourceSet, size_t sid)
	{
		GLuint destSet = (!sourceSet);
		GLuint texUnitId;
		GLuint texId;

//		printf("sets src %i dest%i\n", sourceSet, destSet);
//		printf("input unit/tex: ");
		texUnitId = 0;
		texId = sourceSet*2+texUnitId;
//		printf("%i/%i ", texUnitId, texId);
		glActiveTexture(GL_TEXTURE0+texUnitId);
		glBindTexture(TEX_TYPE, renderTextures[texId]);
		glUniform1i( glGetUniformLocation(shaderProg[sid], "posTexture"), texUnitId);
		
		texUnitId = 1;
		texId = sourceSet*2+texUnitId;
//		printf("%i/%i \n", texUnitId, texId);
		glActiveTexture(GL_TEXTURE0+texUnitId);
		glBindTexture(TEX_TYPE, renderTextures[texId]);
		glUniform1i( glGetUniformLocation(shaderProg[sid], "velTexture"), texUnitId);
		
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[destSet]);
		checkGLError("activateIOBuffer");
	}

};

#endif