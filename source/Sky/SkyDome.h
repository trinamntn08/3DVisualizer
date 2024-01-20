#pragma once

#include"AbstractSky.h"
#include"..\mesh.h"

static std::string path_SkyDome = "source/resources/skydome/";

class SkyDome : public AbstractSky
{
public:

    explicit SkyDome(int nbr_rows =50, int nbr_cols =100, float radius=1.0f) :AbstractSky()
    {
        InitSkyDome(nbr_rows, nbr_cols, radius);
    }

    void Render(Shader& shader) override;
    std::vector<Texture> LoadTextures(std::vector<std::string> textures_skydome) override;


    Vertex CreateVertex(const glm::vec3& position);
    void InitSkyDome(unsigned int nbr_rows = 8, unsigned int nbr_cols = 32, float radius = 1.0f);
    
    std::vector<Vertex> InitVertices(int NumRows=0, int NumCols=0, float Radius=1.0f);
    std::vector<unsigned int> InitIndices(int NumRows, int NumCols);
    glm::vec3 InitPtsBySphericalCoords(float Radius, float Pitch, float Heading);


private:

};