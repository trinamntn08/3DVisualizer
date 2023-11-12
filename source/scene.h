#pragma once
#include"model.h"
#include"boundingBox.h"

std::vector<Texture> LoadCubeMapTextures(std::vector<std::string> textures_faces);


class Scene 
{
public:
    Scene();

    void InitializeCubes(const std::string& filePath);

    // Render all cubes in the scene
    void RenderObjects(Shader& shader);
    void RenderCubeMap(Shader& shader_cubemap);
    std::vector<Model>& getCubes()
    {
        return m_cubes;
    }

    Model& getSpider()
    {
        return m_spider;
    }

    const BoundingBox& getSceneBounds() const
    {
        return m_sceneBounds;
    }

    void UpdateSpiderPosition();

    void CalculateSceneBounds();

    Mesh InitializeCubemap();

    void loadScene();

private:
    std::vector<Model> m_cubes;
    Model m_spider;
    BoundingBox m_sceneBounds;
    Mesh m_cubemap;
};