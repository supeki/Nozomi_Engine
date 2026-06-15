// Nozomi Engine
// GLFW backend
// glfw_video.c

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "../../i_system.h"
#include "../../i_video.h"
#include "../../game_video.h"
#include "../../game_defs.h"

static GLuint screenTex;
GLFWwindow *window;

void I_StartupGraphics(void)
{
	// glfw stuff
    if (!glfwInit())
        I_Error("Failed to initialize GLFW");

	window = glfwCreateWindow(vid.width, vid.height, GAME_NAME, NULL, NULL);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	if (!window) 
		I_Error("Failed to create GLFW window!\n");

    glfwMakeContextCurrent(window);

	glGenTextures(1, &screenTex);
	glBindTexture(GL_TEXTURE_2D, screenTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// opengl stuff
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, vid.width, vid.height);
}

void I_PushGraphics(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vid.width, vid.height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, vid.buffer);

	// draw a quad covering the entire window
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(-1, -1);
		glTexCoord2f(1, 1); glVertex2f( 1, -1);
		glTexCoord2f(1, 0); glVertex2f( 1,  1);
		glTexCoord2f(0, 0); glVertex2f(-1,  1);
	glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
}
