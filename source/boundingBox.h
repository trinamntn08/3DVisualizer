#pragma once

#include"shader.h"
#include <glm/glm.hpp>

class Mesh;
class Model;

class BoundingBox 
{
public:
    BoundingBox();

    // Expand the bounding box to include a point
    void ExpandToFit(const Model& model);

    // Calculate the center of the bounding box
    glm::vec3 GetCenter() const;

    // Calculate the dimensions of the bounding box
    glm::vec3 GetDimensions() const;
    float GetBoundingBoxRadius() const;
    void Render(Shader& shader);

    Mesh toMesh();

private:
    glm::vec3 m_minBounds; //The vector is closest to the origin (0,0,0)
    glm::vec3 m_maxBounds; //The vector is farthest  to the origin (0,0,0)
};
