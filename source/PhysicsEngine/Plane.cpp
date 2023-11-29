#include "Plane.h"

Plane::Plane()
{
	m_shapeID = PLANE;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
	m_distanceToOrigin = 50.0f;
	m_position = m_normal;
	m_2D = false;
	m_elasticity = 0.7f;
}

Plane::Plane(glm::vec3 normal, float distance, bool twoD)
{
	m_shapeID = PLANE;
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_2D = twoD;
	m_elasticity = 0.7f;
}


Plane::~Plane()
{
}

void Plane::updatePhysics(glm::vec3 gravity, float timeStep)
{
}

