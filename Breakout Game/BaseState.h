#pragma once

class BaseState
{
public:
	BaseState();

	virtual void inputHandler() = 0;
	virtual void eventHandler() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

protected:
	Game m_game;
};