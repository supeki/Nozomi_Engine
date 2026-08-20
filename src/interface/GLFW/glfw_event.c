// Nozomi Engine
// GLFW backend
// glfw_event.c

#include <GLFW/glfw3.h>
#include "glfw_video.h"

#include "../../i_event.h"
#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_input.h"

double old_mousex, old_mousey;

void I_PollEvents(void)
{
	double mouse_xpos, mouse_ypos;
	int mouse_state;
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
		game_quit = true;

	for (uint8_t c = 0; c < NUMCONTROLS; c++)
		if (glfwGetKey(window, gamecontrolbinds[0][c][0]) == GLFW_PRESS)
			gamecontrols[0][c]++;
		else if (glfwGetKey(window, gamecontrolbinds[0][c][0]) == GLFW_RELEASE)
			gamecontrols[0][c] = 0;

	glfwGetCursorPos(window, &mouse_xpos, &mouse_ypos);
	mouseaxis[0][MOUSE_POSX] = mouse_xpos;
	mouseaxis[0][MOUSE_POSY] = mouse_ypos;
	mouseaxis[0][MOUSE_INPUTX] = mouse_xpos - old_mousex;
	mouseaxis[0][MOUSE_INPUTY] = mouse_ypos - old_mousey;
	old_mousex = mouse_xpos;
	old_mousey = mouse_ypos;

	mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	switch (mouse_state) {
		case GLFW_PRESS:
			mousecontrols[0][MOUSE_LBUTTON]++;
		case GLFW_RELEASE:
			mousecontrols[0][MOUSE_LBUTTON] = 0;
	}
}
