// Nozomi Engine
// GLFW backend
// glfw_event.c

#include <GLFW/glfw3.h>
#include "glfw_video.h"

#include "../../i_event.h"
#include "../../game_main.h"
#include "../../game_input.h"

void I_PollEvents(void)
{
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
		game_quit = true;

    // not sure if this part works
	int c;

	for (c = CON_UP; c < NUMCONTROLS; c++)
	{
		int key = gamecontrolbinds[c][0];

		if (key >= 0)
			gamecontrols[c] = (glfwGetKey(window, key) == GLFW_PRESS);
	}
}
