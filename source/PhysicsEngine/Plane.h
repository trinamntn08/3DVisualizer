#pragma once

#include"PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec3 normal, float distance, bool twoD = false);
	virtual ~Plane();

	virtual void updatePhysics(glm::vec3 gravity, float timeStep);

	virtual glm::vec3 GetPosition() override { return m_position; };
	//virtual  void setPosition(glm::vec3 a_position) override;
	glm::vec3 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }
	float getElasticity() { return m_elasticity; }
	void setElasticity(float a_elasticity) { m_elasticity = a_elasticity; }


protected:
	bool m_2D=false;
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::vec3 m_normal = glm::vec3(0.0f, 1.0f, 0.0f);;
	float m_distanceToOrigin = 50.0f;
	float m_elasticity = 0.7f;
};

