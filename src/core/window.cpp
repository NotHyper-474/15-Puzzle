#include "window.h"

void Window::HandleEvents(const SDL_Event* e)
{
	if (e->type != SDL_WINDOWEVENT) return;
	switch (e->window.event)
	{
	case SDL_WINDOWEVENT_RESIZED:

		width = e->window.data1;
		height = e->window.data2;
		break;
	}
}