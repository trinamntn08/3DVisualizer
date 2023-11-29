#pragma once

#include"PhysicsObject.h"


class Box : public PhysicsObject
{
public:
	Box(glm::vec3 position, glm::vec3 velocity, float mass, glm::vec3 size, glm::vec4 color, bool twoD = false);
	Box(glm::vec3 position, float angle, float speed, float mass, glm::vec3 size, glm::vec4 color, bool twoD = false);
	virtual ~Box();

	virtual void updatePhysics(glm::vec3 gravity, float timeStep);

	glm::vec3 GetSize() { return m_size; }
	glm::vec4 getColor() { return m_color; }

	bool checkCollision(PhysicsObject* other);
	float distPointToBox(glm::vec3 point);


private:
	glm::vec3 m_size;
	glm::vec4 m_color;
};

