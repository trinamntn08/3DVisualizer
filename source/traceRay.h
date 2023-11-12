#pragma once

#include <glm/glm.hpp>
#include"boundingBox.h"
struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
};

struct HitPayload
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;

	int ObjectIndex;
};

