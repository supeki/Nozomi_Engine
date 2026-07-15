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
void window_size_callback(GLFWwindow* window, int width, int height);

void I_StartupGraphics(void)
{
	// glfw stuff
    if (!glfwInit())
        I_Error("Failed to initialize GLFW");

	window = glfwCreateWindow(VID_WIDTH, VID_HEIGHT, GAME_NAME, NULL, NULL);
	if (!window) 
		I_Error("Failed to create GLFW window!\n");

    glfwMakeContextCurrent(window);

	glGenTextures(1, &screenTex);
	glBindTexture(GL_TEXTURE_2D, screenTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// opengl stuff
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, VID_WIDTH, VID_HEIGHT);
	glfwSetWindowSizeCallback(window, window_size_callback);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	float scale = (float)height / (float)VID_HEIGHT;
	float xscale = (float)width / (float)VID_WIDTH;
	
	if (xscale < scale)
		scale = xscale;
	
	int s_width = (int)(scale*(float)VID_WIDTH);
	int s_height = (int)(scale*(float)VID_HEIGHT);
	glViewport(width/2 - s_width/2, height/2 - s_height/2, s_width, s_height);
}

void I_PushGraphics(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VID_WIDTH, VID_HEIGHT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, vid.buffer);

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
