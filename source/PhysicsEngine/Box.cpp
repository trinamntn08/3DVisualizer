#include "Box.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"

#include <stdlib.h>
#include <iostream>
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\transform.hpp"

Box::Box(glm::vec3 position, glm::vec3 velocity, float mass, glm::vec3 size, glm::vec4 color, bool twoD)
{
	m_shapeID = BOX;
	m_size = size;
	m_color = color;
	m_rigidbody = new RigidBody(position, velocity, glm::vec3(0.0f, 0.0f, 0.0000001f), mass);
	m_2D = twoD;
}

Box::Box(glm::vec3 position, float angle, float speed, float mass, glm::vec3 size, glm::vec4 color, bool twoD)
{
	m_shapeID = BOX;
	m_size = size;
	m_color = color;
	m_rigidbody = new RigidBody(position, angle, speed, glm::vec3(0.0f, 0.0f, 0.00000001f), mass);
	m_2D = twoD;
}

Box::~Box()
{
	delete m_rigidbody;
}

void Box::updatePhysics(glm::vec3 gravity, float timeStep)
{
	m_rigidbody->updatePhysics(gravity, timeStep);
}

bool Box::checkCollision(PhysicsObject * other)
{	
	if (other->getShapeID() == BOX)
	{
		Box* box = dynamic_cast<Box*>(other);
		glm::vec3 thisPos = this->GetPosition();
		glm::vec3 otherPos = box->GetPosition();
		glm::vec3 otherSize = box->GetSize();

		// perform checks
		if (abs(thisPos.x - otherPos.x) > (m_size.x + otherSize.x)) return false;
		if (abs(thisPos.y - otherPos.y) > (m_size.y + otherSize.y)) return false;
		if (abs(thisPos.z - otherPos.z) > (m_size.z + otherSize.z)) return false;
		return true;
	}

	if (other->getShapeID() == SPHERE)
	{
		Sphere* sphere = dynamic_cast<Sphere*>(other);
		float radius = sphere->GetRadius();
		// get the squared distance bewtween the center point and the AABB
		float distance = distPointToBox(sphere->GetPosition());

		return distance > radius ? false : true;
	}

	return false;
}

float Box::distPointToBox(glm::vec3 point)
{
	glm::vec3 center = this->GetPosition();
	float distance = 0.0f;

	glm::vec3 min(center.x - m_size.x, center.y - m_size.y, center.z - m_size.z);
	glm::vec3 max(center.x + m_size.x, center.y + m_size.y, center.z + m_size.z);

	for (int i = 0; i < 3; i++)
	{
		float p = point[i];
		if (p < min[i]) 
		{
			distance += std::pow((min[i] - p),2);
		}
		if (p > max[i]) 
		{
			distance += std::pow((p - max[i]),2);
		}
	}
	return std::sqrt(distance);
}

