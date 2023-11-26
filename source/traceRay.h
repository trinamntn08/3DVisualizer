#pragma once

#include <glm/glm.hpp>
#include"boundingBox.h"
struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
};

struct Plane
{
	glm::vec3 normal;
	glm::vec3 point;

	// Constructor to define a plane using a normal vector and a point
	Plane(const glm::vec3& normal, const glm::vec3& point) :
		normal(glm::normalize(normal)), point(point) {}

	// Constructor to define a plane using a point and two vectors in the plane
	Plane(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2)
	{
		normal = glm::normalize(glm::cross(v1, v2));
		this->point = point;
	}
};

struct HitPayload
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;

	int ObjectIndex;
};

