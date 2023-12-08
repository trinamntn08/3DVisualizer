#pragma once

#include"mesh.h"

static std::string path_SkyDome = "source/resources/skydome/";
static std::vector<Texture> LoadSkyDomeTextures(std::string textures_skydome);

class SkyDome
{
public:

    SkyDome(int nbr_rows =50, int nbr_cols =100, float radius=1.0f)
    {
        InitSkyDome(nbr_rows, nbr_cols, radius);
    }

    Vertex CreateVertex(const glm::vec3& position);
    void InitSkyDome(unsigned int nbr_rows=8, unsigned int nbr_cols=32,float radius=1.0f);
    void Render(Shader& shader);
    std::vector<Vertex> InitVertices(int NumRows=0, int NumCols=0, float Radius=1.0f);
    std::vector<unsigned int> InitIndices(int NumRows, int NumCols);
    glm::vec3 InitPtsBySphericalCoords(float Radius, float Pitch, float Heading);

private:
    unsigned int m_numVertices=0;
    Mesh* m_skyDome;
};