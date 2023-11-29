#pragma once
#include "PhysicsObject.h"

class Plane :
	public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec3 normal, float distance, bool twoD = false);
	virtual ~Plane();

	virtual void updatePhysics(glm::vec3 gravity, float timeStep);
	// get/set
	virtual glm::vec3 getPosition() override { return m_position; };
	//virtual  void setPosition(glm::vec3 a_position) override;
	glm::vec3 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }
	float getElasticity() { return m_elasticity; }
	void setElasticity(float a_elasticity) { m_elasticity = a_elasticity; }


private:
	bool m_2D;
	glm::vec3 m_position;
	glm::vec3 m_normal;
	float m_distanceToOrigin;
	float m_elasticity;
};

