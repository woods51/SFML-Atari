#pragma once

#include "Tile.h"

class Ball
{
public:
	Ball(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(WIDTH/2, 610.0f), float radius = 12.0f,
		sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f), std::string textureID = "ball_01")
	{
		m_startPos = pos;
		m_startVel = sf::Vector2f(m_speed, -m_speed);
		//m_velocity = m_startVel;
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

	const sf::Texture* getTexture() const { return m_shape.getTexture(); }
	const sf::CircleShape& getShape() const { return m_shape; }

	sf::Vector2f getDiagonalPosition() const;
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	enum class Direction getDirection() const { return m_currentDir; }
	float getRadius() const { return m_shape.getRadius(); }

	sf::Vector2f getVelocity() const { return m_velocity; }
	sf::Vector2f getStartPosition() const { return m_startPos; }
	sf::Vector2f getStartVelocity() const { return m_startVel; }
	inline bool getActive() const { return m_active; }

	void setPosition(sf::Vector2f pos) { m_shape.setPosition(pos); }
	void setVelocity(sf::Vector2f vel) { m_velocity = vel; }
	void setStartPosition(sf::Vector2f pos) { m_startPos = pos; }
	void setStartVelocity(sf::Vector2f vel) { m_startVel = vel; }

	void setActive() { if (!m_active) m_velocity = m_startVel; m_active = true; }

private:
	void handleBorder();
	
	bool m_active = false;
	sf::CircleShape m_shape;
	sf::Vector2f m_startPos;
	sf::Vector2f m_startVel;
	sf::Vector2f m_velocity;
	enum class Direction m_currentDir = Direction::Idle;

	float m_speed = 8.0f;
	std::string m_colors[6] = { "ball_01", "ball_02", "ball_03", "ball_04", "ball_05", "ball_06" };
	int m_colorIndex = 0;
	
};