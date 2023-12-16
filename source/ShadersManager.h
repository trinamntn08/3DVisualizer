#pragma once

#include"shader.h"


class ShadersManager
{

public:
    Shader objects;
    Shader skyBox;
    Shader skyDome;
    Shader terrain;
    Shader plane;

    void setObjectsShader(const Shader& shader) { objects = shader; }
    void setSkyBoxShader(const Shader& shader) { skyBox = shader; }
    void setSkyDomeShader(const Shader& shader) { skyDome = shader; }
    void setTerrainShader(const Shader& shader) { terrain = shader; }
    void setPlaneShader(const Shader& shader) { plane = shader; }

};

