#pragma once
#include <SDL.h>
#include <unordered_map>
#include <set>



enum class Action;


class Input
{
	public:
		Input();
		~Input();

		void BindKey(Action action, SDL_Scancode key);
		void BindButton(Action action, SDL_GameControllerButton button);
		void ProccesInput();
		bool IsActionHeld(Action action);
		bool IsActionPress(Action action);
		float GetLeftStickX();
		float GetLeftStickY();

	private:
		std::unordered_map<Action, SDL_Scancode> keyBindings;
		std::unordered_map<Action, SDL_GameControllerButton> buttonBindings;
		SDL_GameController* gameController = nullptr;

		std::set<SDL_Scancode> heldKeys; //maintenu
		std::set<SDL_Scancode> pressedKeys; //presser
		std::set<uint8_t> heldButtons;
		std::set<uint8_t> pressedButtons;

		float leftStickX = 0.0f;
		float leftStickY = 0.0f;
		const float DEAD_ZONE = 0.2f;
};