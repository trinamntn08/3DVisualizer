#pragma once

#include "mesh.h"
static std::string path_skyBox = "source/resources/skybox/skybox1/";
static std::vector<Texture> LoadSkyBoxTextures(std::vector<std::string> textures_faces);

class Shader;
class Skybox
{
public:
    Skybox() { init(); }
    Skybox(const Texture& texure) :m_texture(texure) { init(); }

    void Render(Shader& shader);

    Texture& getTexture() { return m_texture; }

private:
    Mesh* m_skyBox = nullptr;
    Texture m_texture;
    void init();
};


