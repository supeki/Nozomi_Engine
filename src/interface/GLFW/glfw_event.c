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
	{
		uint32_t key = gamecontrolbinds[c][0];

		if (key >= 0)
			if (glfwGetKey(window, key) == GLFW_PRESS)
				gamecontrols[c]++;
			else
				gamecontrols[c] = 0;
	}
}
