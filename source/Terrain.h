#pragma once

#include"PhysicsEngine/PhysicsObject.h"
#include"model.h"
#include "boundingBox.h"

class Shader;

static std::string path_terrain_texture = "source/resources/terrain/";
static std::string heightMapFile = "source/resources/terrain/heightmap_paris.png"; 

class BaseTerrain:public PhysicsObject
{
public:
	BaseTerrain(glm::vec3 scale= glm::vec3(5.0f,5.0f,5.0f));

	virtual ~BaseTerrain();

	virtual void UpdatePhysics(glm::vec3 gravity, float timeStep);

	void Render(Shader& shader);

//	void LoadFromFile(const char* filePath);
//	float GetHeight(int x, int z) const { return m_heightMap.Get(x, z); }

	inline void SetScale(const glm::vec3& newScale) { m_scale = newScale; }
	inline glm::vec3 GetScale() const { return m_scale; }

	void InitTerrain();
	void UpdateParamsForShaders();
	Texture LoadTerrainTextures(std::string name_texture, std::string pathFile_texture);
	float GetHeightForPos(unsigned int x,unsigned int z, std::vector<std::vector<float>> heightMap);
	std::vector<Vertex> InitVerticesWithHeightMapFromFile(const char* imagePath, unsigned int& width, unsigned int& height);

	void CalculateNormals(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices);

	void SetPosition(const glm::vec3& newPosition);

	void ComputeBoundingBox();
	void UpdateBoundingBox(glm::vec3 deltaPos);

	const BoundingBox& GetBoundingBox() const { return m_bbox; };

private:
	void LoadHeightMapFile(const char* pFilename);

	int m_terrainSize = 0;
	glm::vec3 m_scale = glm::vec3(1.0f);

	Mesh* m_terrain = nullptr;
	unsigned int m_width=0; //x-axis
	unsigned int m_depth=0; //z-axis

	BoundingBox m_bbox;
};