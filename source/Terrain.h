#pragma once
#include"mesh.h"
class Shader;

static std::string path_terrain_texture = "source/resources/terrain/";
static std::string heightMapFile = "source/resources/terrain/iceland_heightmap.png"; 
class BaseTerrain
{
public:
	BaseTerrain() { Init(); }

	void Init(float WorldScale=10.0f);

	void Render(Shader& shader);

//	void LoadFromFile(const char* filePath);
//	float GetHeight(int x, int z) const { return m_heightMap.Get(x, z); }

	float GetWorldScale() const { return m_worldScale; }

	void initTerrain();
	Texture LoadTerrainTextures(std::string name_texture, std::string pathFile_texture);
	float GetHeightForPos(unsigned int x,unsigned int z, std::vector<std::vector<float>> heightMap);
	std::vector<Vertex> InitVerticesWithHeightMapFromFile(const char* imagePath, unsigned int& width, unsigned int& height);
protected:
	void LoadHeightMapFile(const char* pFilename);

	int m_terrainSize = 0;
	float m_worldScale = 1.0f;
//	TriangleList m_triangleList;

//	Array2D<float> m_heightMap;

//	TerrainTechnique m_terrainTech;

	Mesh* m_terrain = nullptr;
	unsigned int m_width=0;
	unsigned int m_depth=0;
};