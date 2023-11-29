#pragma once
#include "glm/glm.hpp"


enum ShapeType {
	JOINT = -1,
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	SHAPE_COUNT = 3
};

class RigidBody;

class PhysicsObject
{
protected:
	PhysicsObject();
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {}
public:
	virtual void updatePhysics(glm::vec3 gravity, float timeStep) = 0;

	int getShapeID() { return m_shapeID; }

	virtual glm::vec3 getPosition();
	virtual glm::vec3 getVelocity();

	virtual void setPosition(glm::vec3 position);
	virtual void setVelocity(glm::vec3 velocity);
	
	RigidBody * rigidbody() { return m_rigidbody; }
	bool toggle2D() { m_2D = !m_2D; return m_2D; }

	virtual void resetPosition();
	virtual void resetVelocity();

protected:
	ShapeType m_shapeID=ShapeType::BOX;
	RigidBody * m_rigidbody = nullptr;
	bool m_2D=false;
};

