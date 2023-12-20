#pragma once

#include"PhysicsEngine/PhysicsObject.h"
#include"model.h"
#include "boundingBox.h"

class Shader;

static std::string path_terrain_texture = "source/resources/terrain/";
static std::string heightMapFile = "source/resources/terrain/heightmap_paris.png"; 

extern unsigned int nbrPatchesTess ;


class BaseTerrain:public PhysicsObject
{
public:
	BaseTerrain(glm::vec3 scale= glm::vec3(1.0f,1.0f,1.0f));

	virtual ~BaseTerrain();

	virtual void UpdatePhysics(glm::vec3 gravity, float timeStep);

	inline void SetScale(const glm::vec3& newScale) { m_scale = newScale; }
	inline glm::vec3 GetScale() const { return m_scale; }

	void Render(Shader& shader);
	void RenderTesselation(Shader& shader);

	void InitTerrain();
	std::vector<Vertex> InitVerticesWithHeightMapFromFile(const char* imagePath, unsigned int& width, unsigned int& height);

	//Tesselation
	void InitTerrainTesselation();
	std::vector<Vertex> InitVerticesTessWithHeightMapTexture(const char* heightMapFilePath, unsigned int& width, unsigned int& height);

	Texture LoadTerrainTextures(std::string name_texture, std::string pathFile_texture);

	float GetHeightForPos(float x,float z);
	float GetHeightInterpolated(float x, float z);
	
	glm::vec3 ConstrainCameraPosToTerrain(glm::vec3 camPos);


	void CalculateNormals(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices);

	void SetPosition(const glm::vec3& newPosition);


	void ComputeBoundingBox();
	void UpdateBoundingBox(glm::vec3 deltaPos);

	const BoundingBox& GetBoundingBox() const { return m_bbox; };

	inline unsigned int getWidth() { return m_width; }
	inline unsigned int getDepth() { return m_depth; }

	void storeTerrainHeightData(std::vector<float>& heightData)
	{
		m_heightMap = std::move(heightData);
	};

private:
	glm::vec3 m_scale = glm::vec3(1.0f);

	unsigned int m_width = 0; //x-axis
	unsigned int m_depth = 0; //z-axis
	std::unique_ptr<Mesh> m_terrain = nullptr;

	BoundingBox m_bbox;

	std::vector<float> m_heightMap;
};