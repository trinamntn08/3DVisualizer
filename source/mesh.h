#ifndef MESH_H
#define MESH_H

#define NOMINMAX
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex 
{
    glm::vec3 Position;
    glm::vec3 Normal=glm::vec3(0.0f,1.0f,0.0f);
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture 
{
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;
    //Empty
    Mesh();
    //
    Mesh(std::vector<Vertex>& vertices,
        std::vector<unsigned int>& indices,
        std::vector<Texture>& textures);

    Mesh(std::vector<Vertex>& vertices, 
        std::vector<Texture>& textures);

    // Terrain tesselation
    Mesh(std::vector<Vertex>& vertices, 
        std::vector<Texture>& textures, 
        int val);

    ~Mesh();

    void Render(Shader& shader);
    void RenderSkyDome(Shader& shader);
    void RenderTesselation(Shader& shader);
private:
    // render data 
    unsigned int VBO, EBO;

    void setupMesh();
    void setupMeshWithoutIndices();
    void setupTessMesh();
};
#endif