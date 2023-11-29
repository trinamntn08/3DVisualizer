#include "Box.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"

#include <stdlib.h>
#include <iostream>
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\transform.hpp"

Box::Box(glm::vec3 position, glm::vec3 velocity, float mass, float size, glm::vec4 color, bool twoD)
{
	m_shapeID = BOX;
	m_size = size;
	m_color = color;
	m_rigidbody = new RigidBody(position, velocity, glm::vec3(0.0f, 0.0f, 0.0000001f), mass);
	m_2D = twoD;
}

Box::Box(glm::vec3 position, float angle, float speed, float mass, float size, glm::vec4 color, bool twoD)
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


/// testing AABB intersection with another AABB
/// - uses the center point/radius method
bool Box::checkCollision(PhysicsObject * other)
{	
	if (other->getShapeID() == BOX)
	{
		Box* box = dynamic_cast<Box*>(other);
		// cache data
		glm::vec3 thisPos = getPosition();
		glm::vec3 otherPos = box->getPosition();
		float otherSize = box->getSize();
		// perform checks
		if (abs(thisPos.x - otherPos.x) > (m_size + otherSize)) return false;
		if (abs(thisPos.y - otherPos.y) > (m_size + otherSize)) return false;
		if (abs(thisPos.z - otherPos.z) > (m_size + otherSize)) return false;
		// return true is all checks pass
		return true;
	}

	if (other->getShapeID() == SPHERE)
	{
		Sphere* sphere = dynamic_cast<Sphere*>(other);
		float radius = sphere->getRadius();
		// get the squared distance bewtween the center point and the AABB
		float distance = distToPointAABB(sphere->getPosition());
		// perform checks
		if (distance > radius * radius) return false;
		// return true is all checks pass
		return true;
	}

	return false;
}
/// Returns the distance between AABB and a given point/vector
/// - distance is returned as a squred value, square root required for true distance
float Box::distToPointAABB(glm::vec3 point)
{
	glm::vec3 center = getPosition();
	float distance = 0.0f;
	// setup min max values as we use radius system for AABB
	glm::vec3 min(center.x - m_size, center.y - m_size, center.z - m_size);
	glm::vec3 max(center.x + m_size, center.y + m_size, center.z + m_size);
	// iterate through the point vector and compare it to min/max values
	// - accumulate squared distance as we go
	for (int i = 0; i < 3; i++)
	{
		float p = point[i];
		if (p < min[i]) 
		{
			distance += (min[i] - p) * (min[i] - p);
		}
		if (p > max[i]) 
		{
			distance += (p - max[i]) * (p - max[i]);
		}
	}
	return distance;
}

