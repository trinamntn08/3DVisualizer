#pragma once

#include"shader.h"


class ShadersManager
{

public:
    Shader objects;
    Shader skyBox;
    Shader skyDome;
    Shader rawTerrain;
    Shader tessTerrain;
    Shader simulTerrain;
    Shader plane;

    void setObjectsShader(const Shader& shader) { objects = shader; }
    void setSkyBoxShader(const Shader& shader) { skyBox = shader; }
    void setSkyDomeShader(const Shader& shader) { skyDome = shader; }
    void setRawTerrainShader(const Shader& shader) { rawTerrain = shader; }
    void setTessTerrainShader(const Shader& shader) { tessTerrain = shader; }
    void setSimulTerrainShader(const Shader& shader) { simulTerrain = shader; }
    void setPlaneShader(const Shader& shader) { plane = shader; }

};

