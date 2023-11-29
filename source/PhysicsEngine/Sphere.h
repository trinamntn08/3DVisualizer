#pragma once
#include "PhysicsObject.h"

class Sphere :public PhysicsObject
{
public:
	Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 color, bool twoD = false);
	Sphere(glm::vec3 position, float angle, float speed, float mass, float radius, glm::vec4 color, bool twoD = false);
	virtual ~Sphere();

	virtual void updatePhysics(glm::vec3 gravity, float tiemStep);
	
	float getRadius() { return m_radius; }
	glm::vec4 getColor() { return m_color; }
private:
	float m_radius;
	glm::vec4 m_color;
};

