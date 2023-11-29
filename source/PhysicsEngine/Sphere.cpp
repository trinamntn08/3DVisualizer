#include "Sphere.h"
#include "RigidBody.h"
#include "glm\gtx\transform.hpp"

Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 color, bool twoD)
{
	m_shapeID = SPHERE;
	m_radius = radius;
	m_color = color;
	m_rigidbody = new RigidBody(position, velocity, glm::vec3(0.0f, 0.0f, 0.01f), mass);
	m_2D = twoD;
}

Sphere::Sphere(glm::vec3 position, float angle, float speed, float mass, float radius, glm::vec4 color, bool twoD)
{
	m_shapeID = SPHERE;
	m_radius = radius;
	m_color = color;
	m_rigidbody = new RigidBody(position, angle, speed, glm::vec3(0.0f, 0.0f, 0.01f), mass);
	m_2D = twoD;
}

Sphere::~Sphere()
{
	delete m_rigidbody;
}

void Sphere::updatePhysics(glm::vec3 gravity, float timeStep)
{
	m_rigidbody->updatePhysics(gravity, timeStep);
}


