#include "PhysicsObject.h"
#include "RigidBody.h"



PhysicsObject::PhysicsObject()
{
}

glm::vec3 PhysicsObject::getPosition()
{
	return m_rigidbody != nullptr ? m_rigidbody->m_data.position : glm::vec3(-1.0f);
}

glm::vec3 PhysicsObject::getVelocity()
{
	return m_rigidbody != nullptr ? m_rigidbody->m_data.velocity : glm::vec3(-1.0f);
}

void PhysicsObject::setPosition(glm::vec3 position)
{
	if (m_rigidbody != nullptr) 
	{
		m_rigidbody->m_data.position = position;
	}
}

void PhysicsObject::setVelocity(glm::vec3 velocity)
{
	if (m_rigidbody != nullptr) 
	{
		m_rigidbody->m_data.velocity = velocity;
	}
}

void PhysicsObject::resetPosition()
{
	if (m_rigidbody != nullptr) 
	{
		m_rigidbody->m_data.position = m_rigidbody->m_data.startPosition;
	}
}

void PhysicsObject::resetVelocity()
{
	if (m_rigidbody != nullptr) 
	{
		m_rigidbody->m_data.velocity = m_rigidbody->m_data.startVelocity;
	}
}

