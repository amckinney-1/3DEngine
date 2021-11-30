#include "InputSystem.h"
#include <algorithm>

namespace nEngine
{
	void InputSystem::Startup()
	{
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);
		keyboardState.resize(numKeys);
		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());
		prevKeyboardState = keyboardState;

		// set initial mouse position
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		mousePosition = glm::vec2{ x , y };
		prevMousePosition = mousePosition;
	}

	void InputSystem::Shutdown()
	{
		//
	}

	void InputSystem::Update(float dt)
	{
		prevKeyboardState = keyboardState;
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr);
		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());

		prevMouseButtonState = mouseButtonState;
		prevMousePosition = mousePosition;
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		mousePosition = glm::vec3{ x, y, 0 };
		mouseButtonState[0] = buttons & SDL_BUTTON_LMASK; // button [0001] & [0RML]
		mouseButtonState[1] = buttons & SDL_BUTTON_MMASK; // button [0010] & [0RML]
		mouseButtonState[2] = buttons & SDL_BUTTON_RMASK; // button [0100] & [0RML]
		mouseRelative = mousePosition - prevMousePosition;

	}

	bool InputSystem::IsKeyDown(int id)
	{
		return keyboardState[id];
	}

	bool InputSystem::IsPreviousKeyDown(int id)
	{
		return prevKeyboardState[id];
	}

	InputSystem::eKeyState InputSystem::GetButtonState(int id)
	{
		eKeyState state = eKeyState::Idle;

		bool keyDown = IsButtonDown(id);
		bool prevKeyDown = IsPreviousButtonDown(id);

		if (keyDown)
		{
			state = (prevKeyDown) ? eKeyState::Held : eKeyState::Pressed;
		}
		else
		{
			state = (prevKeyDown) ? eKeyState::Released : eKeyState::Idle;
		}

		return state;
	}

	InputSystem::eKeyState InputSystem::GetKeyState(int id)
	{
		eKeyState state = eKeyState::Idle;

		bool keyDown = IsKeyDown(id);
		bool prevKeyDown = IsPreviousKeyDown(id);

		if (keyDown)
		{
			state = (prevKeyDown) ? eKeyState::Held : eKeyState::Pressed;
		}
		else
		{
			state = (prevKeyDown) ? eKeyState::Released : eKeyState::Idle;
		}

		return state;
	}
}
