#pragma once
#include "..\mesh.h"

class AbstractSky
{
public:
    AbstractSky() { };
    virtual ~AbstractSky() {};

    virtual void Render(Shader& shader) = 0;

    virtual std::vector<Texture> LoadTextures(std::vector<std::string> textures) =0;

protected:
    std::unique_ptr<Mesh> m_sky = std::make_unique<Mesh>();
};