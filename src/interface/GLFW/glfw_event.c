// Nozomi Engine
// GLFW backend
// glfw_event.c

#include <GLFW/glfw3.h>
#include "glfw_video.h"

#include "../../i_event.h"
#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_input.h"

void I_PollEvents(void)
{
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
		game_quit = true;

	for (uint8_t c = 0; c < NUMCONTROLS; c++)
		if (glfwGetKey(window, gamecontrolbinds[0][c][0]) == GLFW_PRESS)
			gamecontrols[0][c]++;
		else if (glfwGetKey(window, gamecontrolbinds[0][c][0]) == GLFW_RELEASE)
			gamecontrols[0][c] = 0;
}
