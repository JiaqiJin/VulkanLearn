#pragma once

#include "../Common/Singleton.h"
#include "../Math/Math.h"

#include <memory>
#include <set>

enum class KeyState
{
	KEY_DOWN,
	KEY_UP,
	KEY_STATE_COUNT
};

enum class MouseButton
{
	LEFT,
	MIDDLE,
	RIGHT,
	MOUSE_BUTTON_COUNT
};

class IInputListener
{
public:
	virtual void ProcessKey(KeyState keyState, uint8_t keyCode) = 0;
	virtual void ProcessMouse(KeyState keyState, MouseButton mouseButton, const Vector2d& mousePosition) = 0;
	virtual void ProcessMouse(const Vector2d& mousePosition) = 0;
};

class InputHub : public Singleton<IInputListener>
{
public:
	void Register(const std::shared_ptr<IInputListener>& pListener);

	void ProcessKey(KeyState keyState, uint8_t keyCode);
	void ProcessMouse(KeyState keyState, MouseButton mouseButton, const Vector2d& mousePosition);
	void ProcessMouse(const Vector2d& mousePosition);

public:
	double sampleInterval = 60.0f;

private:
	std::set<std::shared_ptr<IInputListener>> m_registeredListener;
};