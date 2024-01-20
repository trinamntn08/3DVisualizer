#pragma once

#include"AbstractSky.h"
#include "..\mesh.h"

static std::string path_skyBox = "source/resources/skybox/skybox1/";

class Skybox:public AbstractSky
{
public:
    explicit Skybox():AbstractSky() { InitSkyBox(); }
    ~Skybox() {};

    void Render(Shader& shader) override ;

    std::vector<Texture> LoadTextures(std::vector<std::string> textures_faces);

private:
    void InitSkyBox();
};


