#pragma once

#include "Tile.h"

class Ball
{
public:
	Ball(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), float radius = 12.0f,
		sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f), std::string textureID = "ball_01")
	{
		m_startPos = pos;
		m_startVel = sf::Vector2f(m_speed, m_speed);
		m_velocity = m_startVel;
		m_shape.setPosition(pos);
		m_shape.setRadius(radius);
		m_shape.setScale(scale);
		m_shape.setTexture(rm.getTexture(textureID));
	};
	
	void update(sf::Time);
	void move(sf::Time);
	void handlePaddle(enum class Surface, enum class Direction);
	void handleTile(enum class Surface surface);
	void reset();
	
	enum class Surface collision(sf::Vector2f, sf::Vector2f) const;
	inline double distance(sf::Vector2i, sf::Vector2i) const;
	void toggleColor(ResourceManager&);

	const sf::Texture* getTexture() const;
	const sf::CircleShape& getShape() const;

	enum class Direction getDirection() const;
	float getRadius() const;
	sf::Vector2f getPosition() const;
	sf::Vector2f getDiagonalPosition() const;
	sf::Vector2f getVelocity() const;
	sf::Vector2f getStartPosition() const;
	sf::Vector2f getStartVelocity() const;

	void setPosition(sf::Vector2f);
	void setVelocity(sf::Vector2f);
	void setStartPosition(sf::Vector2f);
	void setStartVelocity(sf::Vector2f);

private:
	void handleBorder();

	sf::CircleShape m_shape;
	sf::Vector2f m_startPos;
	sf::Vector2f m_startVel;
	sf::Vector2f m_velocity;
	enum class Direction m_currentDir = Direction::Idle;

	float m_speed = 8.0f;
	std::string m_colors[6] = { "ball_01", "ball_02", "ball_03", "ball_04", "ball_05", "ball_06" };
	int m_colorIndex = 0;
	
};