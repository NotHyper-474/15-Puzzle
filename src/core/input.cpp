#include "input.h"

#include <SDL.h>
#include <cstring>

Input::Input() :
m_inputs(),
m_downKeys(),
m_upKeys(),
m_mouseInput(),
m_downMouse(),
m_upMouse(),
m_mouseX(0),
m_mouseY(0)
{
}

void Input::SetCursor(bool visible) const
{
	SDL_ShowCursor(static_cast<int>(visible));
}

void Input::SetMousePosition(const Vector2f& pos) const
{
	SDL_WarpMouseInWindow(nullptr, (int)pos.x, (int)pos.x);
}

void Input::ClearMouseAndKeys()
{
	m_upKeys.reset();
	m_downKeys.reset();
	m_downMouse.reset();
	m_upMouse.reset();
}
