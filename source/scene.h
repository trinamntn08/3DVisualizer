#pragma once
#include"model.h"
#include"boundingBox.h"

class Scene 
{
public:
    Scene(const std::string& filePath);

    // Initialize the scene with 1000 cubes
    void InitializeCubes(const std::string& filePath);

    // Render all cubes in the scene
    void Render(Shader& shader);

    std::vector<Model>& getCubes()
    {
        return m_cubes;
    }

    const BoundingBox& getSceneBounds() const
    {
        return m_sceneBounds;
    }

private:
    std::vector<Model> m_cubes;
    BoundingBox m_sceneBounds;
};