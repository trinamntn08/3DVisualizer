#pragma once
#include"model.h"
#include"boundingBox.h"

class Scene 
{
public:
    Scene();

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

    void UpdateSpiderPosition();


    // for now, scene bounds only relates to cubes bounds
    void CalculateSceneBounds();

private:
    std::vector<Model> m_cubes;
    Model m_spider;
    BoundingBox m_sceneBounds;
};