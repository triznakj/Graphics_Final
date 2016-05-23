#ifndef __GL_LOADER
#define __GL_LOADER

static float initLoader()
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

#endif
