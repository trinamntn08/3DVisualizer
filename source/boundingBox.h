#pragma once

#include"shader.h"
#include <glm/glm.hpp>

class Mesh;
class Model;

class BoundingBox 
{
public:
    BoundingBox();

    void Reset();

    // Expand the bounding box to include a model
 //   void ExpandToFit(const Model& model);

    glm::vec3 GetCenter() const;

    glm::vec3 GetDimensions() const;
    float GetBoundingBoxRadius() const;
    void Render(Shader& shader);
    void ExpandToInclude(const BoundingBox& bbox);

    void UpdateScale(float scale);

    void Move(const glm::vec3& movePos);


    void setMinBound(glm::vec3& minBound) { m_minBounds = minBound; };
    void setMaxBound(glm::vec3& maxBound) { m_maxBounds = maxBound; };

    const glm::vec3 GetMinBounds() const { return m_minBounds; };
    const glm::vec3 GetMaxBounds() const { return m_maxBounds; };

    Mesh toMesh();

private:
    glm::vec3 m_minBounds; //The vector is closest to the origin (0,0,0)
    glm::vec3 m_maxBounds; //The vector is farthest  to the origin (0,0,0)
};
