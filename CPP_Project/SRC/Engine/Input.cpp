#include "Input.h"
#include "Logger.h"

Input::Input()
{
	Logger::Log("Input constructor called");

	if (SDL_NumJoysticks() > 0 && SDL_IsGameController(0))
	{
		gameController = SDL_GameControllerOpen(0);
		Logger::Log("Game controller found");
	}
}

Input::~Input()
{
	if (gameController)
	{
		SDL_GameControllerClose(gameController);
	}

	Logger::Log("Input destructor called");

}

void Input::BindKey(Action action, SDL_Scancode key)
{
	keyBindings[action] = key;
}

void Input::BindButton(Action action, SDL_GameControllerButton button)
{
	buttonBindings[action] = button;
}

void Input::ProccesInput()
{
	heldButtons.clear();
	pressedButtons.clear();
	heldKeys.clear();
	pressedKeys.clear();

	const uint8_t* state = SDL_GetKeyboardState(NULL);

	for (const auto& [action, key] : keyBindings)
	{
		if (state[key])
		{
			heldKeys.insert(key); //ajout la key presser
		}
		if (state[key] && !heldKeys.count(key))
		{
			pressedKeys.insert(key);
		}
	}

	if (gameController)
	{
		for (const auto& [action, button] : buttonBindings)
		{
			if (SDL_GameControllerGetButton(gameController, button))
			{
				heldButtons.insert(button);
				if (!heldButtons.count(button))
				{
					pressedButtons.insert(button);
				}
			}
		}

		//read left stick axis
		leftStickX = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f;//normalize
		leftStickY = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f;//normalize

		//apply dead zone
		if (fabs(leftStickX) < DEAD_ZONE)
		{
			leftStickX = 0.0f;
		}
		if (fabs(leftStickY) < DEAD_ZONE)
		{
			leftStickY = 0.0f;
		}
	}

}

float Input::GetLeftStickX()
{
	return leftStickX;
}

float Input::GetLeftStickY()
{
	return leftStickY;
}

bool Input::IsActionHeld(Action action)
{
	return heldKeys.count(keyBindings[action]) || heldButtons.count(buttonBindings[action]);
}

bool Input::IsActionPress(Action action)
{
	return pressedKeys.count(keyBindings[action]) || pressedButtons.count(buttonBindings[action]);
}



