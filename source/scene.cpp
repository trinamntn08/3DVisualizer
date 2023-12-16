#include"scene.h"
#include"traceRay.h"
#include"PhysicsEngine/RigidBody.h"
#include"PhysicsEngine/Sphere.h"
#include"PhysicsEngine/Plane.h"
#include"PhysicsEngine/Box.h"
#include <glm/gtx/projection.hpp>
#include <glm/gtx/perpendicular.hpp>
#include"Timer.h"
#include"Logger.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFuncs[] =
{
	Scene::planeToPlane, Scene::planeToSphere, Scene::planeToBox,
	Scene::sphereToPlane, Scene::sphereToSphere, Scene::sphereToBox,
	Scene::boxToPlane, Scene::boxToSphere, Scene::boxToBox
};

Scene::Scene(Sky typeSkye)
{
	m_typeSky = typeSkye;
	loadScene();
}

Scene::~Scene()
{
    for (auto& item : m_allPhysicsObjects)
    {
        delete item;
    }
}

void Scene::loadScene()
{
	Timer("loadScene");
	
    InitializeCubes(cubePath);

    CalculateSceneBounds();

	InitializeBalls(ballPath);
	
	m_plane = std::make_unique <PlaneModel>();
	
	if (m_typeSky == Sky::SkyBox)
	{
		m_skyBox = std::make_unique<Skybox>();
	}
	else if (m_typeSky == Sky::SkyDome)
	{
		m_skyDome = std::make_unique<SkyDome>();
	}

	m_terrain = std::make_unique<BaseTerrain>();
	
}
void  Scene::ResetScene()
{
    // reset all objects's position and velocity
    for (auto& item : m_allPhysicsObjects) 
    {
        item->ResetPosition();
        item->ResetVelocity();
		SphereModel* ball_item = dynamic_cast<SphereModel*>(item);
		if (ball_item)
		{
			ball_item->ComputeBoundingBox();
		}
    }

	// reset all cubes's position and velocity
	for (auto& item : m_cubes)
	{
		item->ResetPosition();
		item->ResetVelocity();
		BoxModel* box_item = dynamic_cast<BoxModel*>(item);
		if (box_item)
		{
			box_item->ComputeBoundingBox();
		}
	}
}
void  Scene::ClearScene()
{
    m_allPhysicsObjects.clear();
}
void Scene::UpdateAllObjectsToFitScene()
{
	for (auto& item : m_allPhysicsObjects)
	{
		UpdatePhysicsObjectToFitScene(*item);
	}
}
void Scene::UpdatePhysicsObjectToFitScene(PhysicsObject& object)
{
	SphereModel* box = dynamic_cast<SphereModel*>(&object);

    glm::vec3 sceneBoundsCenter = m_sceneBounds.GetCenter();
    const BoundingBox& model_bbox = box->GetBoundingBox();
    glm::vec3 bboxCenter = model_bbox.GetCenter();
    glm::vec3 modelMove = sceneBoundsCenter - bboxCenter;

    // Adjust the y-axis of spider to move it above the scene
    float yOffset = m_sceneBounds.GetMaxBounds().y - model_bbox.GetMinBounds().y;
    modelMove.y = yOffset;
    box->SetPosition(modelMove);
}
void Scene::OnUpdate(float deltaTime)
{
	for (auto& item : m_allPhysicsObjects)
	{
		item->UpdatePhysics(m_properties.gravity ? m_gravity : glm::vec3(0.0f), deltaTime);
	}

	// check for collisions
	if (m_properties.collisions)
	{
		checkCollisions();
	}
}

void Scene::InitializeCubes(const std::string &filePath)
{
	m_cubes.clear();

    int numRows = 10; 
    int numCols = 10; 
    float spacing = 0.0f; // Spacing between cubes
	float cubeSize = 2.0f;


	for (int row = 0; row < numRows; row++) 
	{
		for (int col = 0; col < numCols; col++) 
		{
			BoxModel* cube_i = new BoxModel(filePath);
			glm::vec3 position(col * (spacing + cubeSize), 50.0f, row* (spacing + cubeSize));
			cube_i->SetPosition(position);
			cube_i->SetCurrentPosAsOriginalPos();
			m_cubes.push_back(cube_i);
		}
	}
	/*
	// top walls
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			BoxModel* cube_i = new BoxModel(filePath);
			glm::vec3 position(col * (spacing + cubeSize), numCols * (spacing + cubeSize), row * (spacing + cubeSize));
			cube_i->SetPosition(position);
			m_grounds.push_back(cube_i);
		}
	}

	// left walls
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols+1; col++) {
			BoxModel* cube_i = new BoxModel(filePath);
			glm::vec3 position( 0.0f, col * (spacing + cubeSize), row * (spacing + cubeSize));
			cube_i->SetPosition(position);
			m_grounds.push_back(cube_i);
		}
	}

	//right walls
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols+1; col++) {
			BoxModel* cube_i = new BoxModel(filePath);
			glm::vec3 position(numCols * (spacing + cubeSize), col * (spacing + cubeSize), row * (spacing + cubeSize));
			cube_i->SetPosition(position);
			m_grounds.push_back(cube_i);
		}
	}

	diagonal walls
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			Cube* cube_i = new Cube(filePath);
			glm::vec3 position(col * spacing, col * spacing, row * spacing);
			cube_i->SetPosition(position);
			m_grounds.push_back(cube_i);
		}
	}
	*/

}

void Scene::InitializeBalls(const std::string& ballPath)
{
	SphereModel* ball1 = new SphereModel(ballPath, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	SphereModel* ball2 = new SphereModel(ballPath, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	SphereModel* ball3 = new SphereModel(ballPath, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	m_allPhysicsObjects.push_back(ball1);
	m_allPhysicsObjects.push_back(ball2);
	m_allPhysicsObjects.push_back(ball3);

	UpdateAllObjectsToFitScene();

	ball1->Translation(glm::vec3(3.0f, 5.0f, 3.0f));
	ball2->Translation(glm::vec3(-3.0f, 7.0f, 3.0f));
	ball3->Translation(glm::vec3(0.0f, 4.0f, 3.0f));

	ball1->SetCurrentPosAsOriginalPos();
	ball2->SetCurrentPosAsOriginalPos();
	ball3->SetCurrentPosAsOriginalPos();
}

void Scene::CalculateSceneBounds()
{
	m_sceneBounds.Reset();
	for (const BoxModel* cube : m_cubes)
	{
		const BoundingBox& cubeBounds = cube->GetBoundingBox();
		m_sceneBounds.ExpandToInclude(cubeBounds);
	}
}

void Scene::Render(ShadersManager& shadersManager, const std::unique_ptr<Camera>& camera)
{
	glClearColor(0.08f, 0.16f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	RenderPhysicsObjects(shadersManager.objects,camera);

	if (m_typeSky == Sky::SkyBox)
	{
		RenderSkyBox(shadersManager.skyBox, camera);
	}
	else if(m_typeSky == Sky::SkyDome)
	{
		RenderSkyDome(shadersManager.skyDome, camera);
	}

	RenderPlane(shadersManager.plane, camera);
	RenderTerrainTesselation(shadersManager.terrain, camera);
}
void Scene::RenderPhysicsObjects(Shader& shader,const std::unique_ptr<Camera>& camera, bool isRender_BBoxes)
{
	shader.activate();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = camera->GetProjectionMatrix();
	shader.setMat_MVP(model, view, projection);

	for (BoxModel* cube : m_cubes)
	{
		cube->Render(shader);
	}


	for (auto& item : AllPhysicsObjects())
	{
		SphereModel* ball_item = dynamic_cast<SphereModel*>(item);
		if (ball_item)
		{
			ball_item->Render(shader);
		}
	}

    // Render bounding boxes
    if (isRender_BBoxes)
    {
		m_sceneBounds.Render(shader);

        for (auto& item : AllPhysicsObjects())
        {
			SphereModel* ball = dynamic_cast<SphereModel*>(item);
            BoundingBox bbox_item = ball->GetBoundingBox();
            bbox_item.Render(shader);
        }

		BoundingBox bbox_terrain = m_terrain->GetBoundingBox();
		bbox_terrain.Render(shader);
    }
}
void Scene::RenderPlane(Shader& shader_plane, const std::unique_ptr<Camera>& camera)
{
	shader_plane.activate();
	// lighting info
	static glm::vec3 lightPos(0.0f, 100.0f, 0.0f);
	shader_plane.setVec3("lightPos", lightPos);
	shader_plane.setInt("blinn", true);

	glm::mat4 model_plane = glm::mat4(1.0f);
	glm::mat4 view_plane = camera->GetViewMatrix();
	glm::mat4 projection_plane = camera->GetProjectionMatrix();
	shader_plane.setMat_MVP(model_plane, view_plane, projection_plane);
	m_plane->Render(shader_plane);
}
void Scene::RenderSkyBox(Shader& shader_skybox, const std::unique_ptr<Camera>& camera)
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	shader_skybox.activate();
	glm::mat4 model_skyBox = glm::mat4(1.0f);
	glm::mat4 view_skyBox = camera->GetViewMatrix();
	glm::mat4 projection_skyBox = camera->GetProjectionMatrix();
	shader_skybox.setMat_MVP(model_skyBox, view_skyBox, projection_skyBox);
	m_skyBox->Render(shader_skybox);

	glDepthFunc(GL_LESS);
}
void Scene::RenderSkyDome(Shader& shader_skydome, const std::unique_ptr<Camera>& camera)
{
	glDepthFunc(GL_LEQUAL);
	shader_skydome.activate();

	glm::mat4 model_skyDome = glm::mat4(1.0f);
	glm::mat4 view_skyDome = camera->GetViewMatrix();
	glm::mat4 projection_skyDome = camera->GetProjectionMatrix();
	shader_skydome.setMat_MVP(model_skyDome, view_skyDome, projection_skyDome);

	m_skyDome->Render(shader_skydome);
	glDepthFunc(GL_LESS);
}
void Scene::RenderTerrain(Shader& shader_terrain, const std::unique_ptr<Camera>& camera)
{
	shader_terrain.activate();
	m_terrain->Render(shader_terrain);
}

void Scene::RenderTerrainTesselation(Shader& shader_terrain, const std::unique_ptr<Camera>& camera)
{
	shader_terrain.activate();
	glm::mat4 model_terrain = glm::mat4(1.0f);
	model_terrain = glm::scale(model_terrain, this->getTerrain()->GetScale());
	glm::mat4 view_terrain = camera->GetViewMatrix();
	glm::mat4 projection_terrain = camera->GetProjectionMatrix();
	shader_terrain.setMat_MVP(model_terrain, view_terrain, projection_terrain);
	
	// Change light over time
	//static float angle = 0.0f;
	//angle += 0.003f;
	//if (angle > 2.0f * 3.1415926f)
	//{
	//    angle = 0.0f;
	//}

	//// Simulate sun's path
	//float radius = 10.0f; // Can adjust based on desired orbit size
	//float sunHeightMax = 10.0f; // Maximum height of the sun
	//float sunHeightMin = -10.0f; // Minimum height of the sun (can be negative if you want the sun to go below the horizon)
	//float y = sunHeightMin + (sunHeightMax - sunHeightMin) * 0.5f * (1 + sinf(angle - 3.1415926 / 2.0f));
	//// Calculate sun position
	//glm::vec3 LightDir(sinf(angle) * radius, y, cosf(angle) * radius);

	//glm::vec3 ReversedLightDir = -glm::normalize(LightDir);
	////     glm::vec3 ReversedLightDir = -glm::vec3(0.0f, 1.0f, 0.0f);
	//m_shader_terrain.setVec3("gReversedLightDir", 0.0f, ReversedLightDir.y, ReversedLightDir.z );

	m_terrain->RenderTesselation(shader_terrain);
}
/*********************************************************************************************************
*                     COLLISIONS
**********************************************************************************************************/

void Scene::checkCollisions()
{
	int nbrObjects = numberOfObjects();

	// check for collisions between objects
	for (int idx1 = 0; idx1 < nbrObjects - 1; idx1++)
	{
		for (int idx2 = idx1 + 1; idx2 < nbrObjects; idx2++)
		{
			PhysicsObject* objA = m_allPhysicsObjects[idx1];
			PhysicsObject* objB = m_allPhysicsObjects[idx2];
			int shapeIdA = objA->getShapeID();
			int shapeIdB = objB->getShapeID();
			// skip checking collisions for joints
			if (shapeIdA < 0 || shapeIdB < 0)
			{
				continue;
			}
			// function pointers
			int functionID = (shapeIdA * SHAPE_COUNT) + shapeIdB;
			fn collisionFuncPtr = collisionFuncs[functionID];
			if (collisionFuncPtr != nullptr)
			{
				collisionFuncPtr(objA, objB);
			}
		}
	}

	int nbrGrounds = m_cubes.size();
	for (int objIdx = 0; objIdx < nbrObjects ; objIdx++)
	{
		PhysicsObject* physObj = m_allPhysicsObjects[objIdx];

		for (int groundIndex = 0; groundIndex < nbrGrounds; groundIndex++) {
			BoxModel* ground = m_cubes[groundIndex];

			// Check for collision between physics object and ground
			int shapeIdA = physObj->getShapeID();
			int shapeIdB = ground->getShapeID();
			// skip checking collisions for joints
			if (shapeIdA < 0 || shapeIdB < 0)
			{
				continue;
			}
			// function pointers
			int functionID = (shapeIdA * SHAPE_COUNT) + shapeIdB;
			fn collisionFuncPtr = collisionFuncs[functionID];
			if (collisionFuncPtr != nullptr)
			{
				collisionFuncPtr(physObj, ground);
			}
		}
	}
}
/*********************************************************************************************************
* Plane to Object collsions
**********************************************************************************************************/

bool Scene::planeToPlane(PhysicsObject* a_planeA, PhysicsObject* a_planeB)
{
	Plane* planeA = dynamic_cast<Plane*>(a_planeA);
	Plane* planeB = dynamic_cast<Plane*>(a_planeB);
	// check if objects aren't null before testing
	if (planeA != nullptr && planeB != nullptr) {
		// check plane for collision
		// if the normals are pointing in the same direction
		//	- just check distance between and distance fom origin
		// if the normal are at an angle
		//	- could do some fancy trig or aabb stuff
	}
	return false;
}

bool Scene::planeToSphere(PhysicsObject* plane, PhysicsObject* sphere)
{
	return sphereToPlane(sphere, plane);
}

bool Scene::planeToBox(PhysicsObject* plane, PhysicsObject* box)
{
	return boxToPlane(box, plane);
}
/*********************************************************************************************************
* Sphere to Object collsions
**********************************************************************************************************/
bool Scene::sphereToSphere(PhysicsObject* a_sphereA, PhysicsObject* a_sphereB)
{
	SphereModel* sphereA = dynamic_cast<SphereModel*>(a_sphereA);
	SphereModel* sphereB = dynamic_cast<SphereModel*>(a_sphereB);

	if (sphereA != nullptr && sphereB != nullptr)
	{
		// check sphere for collision
		float distance = glm::distance(sphereA->GetPosition(), sphereB->GetPosition());
		float totalRadius = sphereA->GetRadius() + sphereB->GetRadius();
		// compare distance between centers to combined radius
		if (distance < totalRadius) 
		{
			// cahce bool values
			bool kinematicA = sphereA->Rigidbody()->m_data.isKinematic;
			bool kinematicB = sphereB->Rigidbody()->m_data.isKinematic;
			bool onGroundA = sphereA->Rigidbody()->m_data.onGround;
			bool onGroundB = sphereB->Rigidbody()->m_data.onGround;
			// check either is kinematic
			if (!kinematicA || !kinematicB) 
			{
				// get the normal of the gap between objects
				glm::vec3 collisionNormal = glm::normalize(sphereB->GetPosition() - sphereA->GetPosition());
				// if both spheres are not on the ground
				if (!onGroundA && !onGroundB) {
					// calculate force vector
					glm::vec3 relativeVelocity = sphereA->GetVelocity() - sphereB->GetVelocity();
					glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
					glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / sphereA->Rigidbody()->m_data.mass + 1.0f / sphereB->Rigidbody()->m_data.mass);

					// combine elasticity
					float combinedElasticity = (sphereA->Rigidbody()->m_data.elasticity +
						sphereB->Rigidbody()->m_data.elasticity / 2.0f);
					// use Newton's third law to apply collision forces to colliding bodies 
					sphereA->Rigidbody()->applyForceToAnotherBody(sphereB->Rigidbody(), forceVector + (forceVector * combinedElasticity));

					// apply torque
					glm::vec3 centerPoint = sphereA->GetPosition() - sphereB->GetPosition();
					glm::vec3 torqueLever = glm::normalize(glm::vec3(centerPoint.y, -centerPoint.x, 0.0f));

					float torque = glm::dot(torqueLever, relativeVelocity) * 1.0f / (1.0f / sphereA->Rigidbody()->m_data.mass + 1.0f / sphereB->Rigidbody()->m_data.mass);

					sphereA->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, -torque));
					sphereB->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, torque));

					// move out spheres out of collision 
					glm::vec3 separationVector = collisionNormal * distance * 0.5f;
					sphereA->SetPosition(sphereA->GetPosition() - separationVector);
					sphereB->SetPosition(sphereB->GetPosition() + separationVector);
				}
				// if one sphere is on the ground treat collsion as plane collision
				if (onGroundA || onGroundB)
				{
					// determine moving sphere
					Sphere* sphere = (onGroundA ? sphereB : sphereA);
					Sphere* sphereGround = (onGroundA ? sphereA : sphereB);
					// calculate force vector
					glm::vec3 forceVector = -1 * sphere->Rigidbody()->m_data.mass * collisionNormal * (glm::dot(collisionNormal, sphere->GetVelocity()));
					// apply force
					sphere->Rigidbody()->applyForce(forceVector * 2.0f);
					// apply torque
					glm::vec3 torqueLever = glm::normalize(glm::vec3(collisionNormal.y, -collisionNormal.x, 0.0f));

					float torque = glm::dot(torqueLever, sphere->GetVelocity()) * -1.0f / (1.0f / sphereA->Rigidbody()->m_data.mass);
					sphere->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, torque));

					// move out of collision
					glm::vec3 separationVector = collisionNormal * distance * 0.5f;
					sphere->SetPosition(sphere->GetPosition() - separationVector);
					// stop other sphere from being on ground
					sphereGround->Rigidbody()->m_data.onGround = false;
				}
			}
			else
			{
				// object colliding yes, stop objects
				sphereA->SetVelocity(glm::vec3(0.0f));
				sphereB->SetVelocity(glm::vec3(0.0f));
				if (onGroundA || onGroundB)
				{
					sphereA->Rigidbody()->m_data.onGround = true;
					sphereB->Rigidbody()->m_data.onGround = true;
				}
			}
			return true;
		}
	}
	return false;
}

bool Scene::sphereToPlane(PhysicsObject* a_sphere, PhysicsObject* a_plane)
{
	Sphere* sphere = dynamic_cast<Sphere*>(a_sphere);
	Plane* plane = dynamic_cast<Plane*>(a_plane);

	if (sphere != nullptr && plane != nullptr) 
	{
		bool kinematic = sphere->Rigidbody()->m_data.isKinematic;
		glm::vec3 planeNorm = plane->getNormal();
		float planeDO = plane->getDistance();
		// magnitude of sphere vector, plane normal
		float mag = (dot(sphere->GetPosition(), planeNorm));

		// if planeNorm is below 0 magnitude will be negative
		if (mag < 0)
		{
			planeNorm *= -1;
			mag *= -1;
		}

		float collision = mag - sphere->GetRadius();
		// collision check
		if (collision < 0.0f) 
		{
			if (!kinematic) 
			{
				// calculate force vector
				glm::vec3 forceVector = -1 * sphere->Rigidbody()->m_data.mass * planeNorm * (glm::dot(planeNorm, sphere->GetVelocity()));
				// combine elasticity
				float combinedElasticity = (sphere->Rigidbody()->m_data.elasticity +
					plane->getElasticity() / 2.0f);
				// only bounce if not resting on the ground
				if (!sphere->Rigidbody()->m_data.onGround)
				{
					sphere->Rigidbody()->applyForce(forceVector + (forceVector * combinedElasticity));
					// apply torque
					glm::vec3 centerPoint = sphere->GetPosition() - planeNorm;
					glm::vec3 torqueLever = glm::normalize(glm::vec3(centerPoint.y, -centerPoint.x, 0.0f));

					float torque = glm::dot(torqueLever, sphere->GetVelocity()) * 1.0f / (1.0f / sphere->Rigidbody()->m_data.mass);

					sphere->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, -torque));

					// move out of collision
					glm::vec3 separationVector = planeNorm * collision * 0.5f;
					sphere->SetPosition(sphere->GetPosition() - separationVector);
				}
			}
			else
			{
				// object colliding, stop object
				sphere->SetVelocity(glm::vec3(0.0f));
				sphere->Rigidbody()->m_data.onGround = true;
			}
			return true;
		}
	}
	return false;
}

bool Scene::sphereToBox(PhysicsObject* a_sphere, PhysicsObject* a_box)
{
	return boxToSphere(a_box, a_sphere);
}
/*********************************************************************************************************
* Box to Object collsions
**********************************************************************************************************/
bool Scene::boxToSphere(PhysicsObject* a_box, PhysicsObject* a_sphere)
{
	Box* box = dynamic_cast<Box*>(a_box);
	Sphere* sphere = dynamic_cast<Sphere*>(a_sphere);

	if (box != nullptr && sphere != nullptr)
	{
		// collision check
		if (box->checkCollision(sphere))
		{
			// cache some bools for later use
			bool kinematicA = box->Rigidbody()->m_data.isKinematic;
			bool kinematicB = sphere->Rigidbody()->m_data.isKinematic;
			bool onGroundA = box->Rigidbody()->m_data.onGround;
			bool onGroundB = sphere->Rigidbody()->m_data.onGround;
			// check either is kinematic
			if (!kinematicA || !kinematicB)
			{
				glm::vec3 centerDist = sphere->GetPosition() - box->GetPosition();
				glm::vec3 boxesMaxSize = glm::vec3(box->GetSize() + sphere->GetRadius());
				glm::vec3 collisionNormal = glm::normalize(centerDist);
				glm::vec3 overlap = abs(centerDist - boxesMaxSize);
				// if both boxs are not on the ground
				if (!onGroundA && !onGroundB) 
				{
					// calculate force vector
					glm::vec3 relativeVelocity = box->GetVelocity() - sphere->GetVelocity();
					glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
					glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / box->Rigidbody()->m_data.mass + 1.0f / sphere->Rigidbody()->m_data.mass);
					// combine elasticity
					float combinedElasticity = (box->Rigidbody()->m_data.elasticity +
						sphere->Rigidbody()->m_data.elasticity / 2.0f);
					// use Newton's third law to apply collision forces to colliding bodies 
					box->Rigidbody()->applyForceToAnotherBody(sphere->Rigidbody(), forceVector + (forceVector * combinedElasticity));
					// apply torque
					float torque = glm::dot(collisionVector, relativeVelocity) * 1.0f / (1.0f / box->Rigidbody()->m_data.mass + 1.0f / sphere->Rigidbody()->m_data.mass);

					box->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, -torque));
					sphere->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, torque));

					// move out boxs out of collision 
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					box->SetPosition(box->GetPosition() - separationVector);
					sphere->SetPosition(sphere->GetPosition() + separationVector);
				}
				// if one box is on the ground treat collision as plane collision
				if (onGroundA || onGroundB)
				{
					// determine moving box
					PhysicsObject* obj = (onGroundA ? dynamic_cast<PhysicsObject*>(sphere) : dynamic_cast<PhysicsObject*>(box));
					PhysicsObject* objGround = (onGroundA ? dynamic_cast<PhysicsObject*>(box) : dynamic_cast<PhysicsObject*>(sphere));
					// calculate force vector
					glm::vec3 forceVector = -1 * obj->Rigidbody()->m_data.mass * collisionNormal * (glm::dot(collisionNormal, obj->GetVelocity()));
					// apply force
					obj->Rigidbody()->applyForce(forceVector * 2.0f);
					// move out of collision
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					obj->SetPosition(obj->GetPosition() - separationVector);
					// stop other box from being on ground
					objGround->Rigidbody()->m_data.onGround = objGround->Rigidbody()->m_data.isStatic ? true : false;
				}
			}
			else
			{
				// object colliding yes, stop objects
				box->SetVelocity(glm::vec3(0.0f));
				sphere->SetVelocity(glm::vec3(0.0f));
				if (onGroundA || onGroundB) {
					box->Rigidbody()->m_data.onGround = true;
					sphere->Rigidbody()->m_data.onGround = true;
				}
			}
			return true;
		}
	}
	return false;
}

bool Scene::boxToPlane(PhysicsObject* a_box, PhysicsObject* a_plane)
{
	Box* box = dynamic_cast<Box*>(a_box);
	Plane* plane = dynamic_cast<Plane*>(a_plane);

	if (box != nullptr && plane != nullptr)
	{
		glm::vec3 planeNormal = plane->getNormal();
		glm::vec3 center = box->GetPosition();
		glm::vec3 extents(box->GetSize());
		// magnitude of box center and plane vectors
		float mag = dot(planeNormal, center);
		// projection interval radius of box onto the plane
		float radius = extents.x * abs(planeNormal.x) + extents.y * abs(planeNormal.y) + extents.z * abs(planeNormal.z);

		// if planeNorm is below 0 magnitude will be negative
		if (mag < 0)
		{
			planeNormal *= -1;
			mag *= -1;
		}

		float collision = mag - radius;

		// collision check
		if (collision <= 0.0f)
		{
			// cache some data
			bool kinematic = box->Rigidbody()->m_data.isKinematic;
			if (!kinematic) {
				// calculate force vector
				glm::vec3 forceVector = -1 * box->Rigidbody()->m_data.mass * planeNormal * (glm::dot(planeNormal, box->GetVelocity()));
				// combine elasticity
				float combinedElasticity = (box->Rigidbody()->m_data.elasticity +
					plane->getElasticity() / 2.0f);
				// only bounce if not resting on the ground
				if (!box->Rigidbody()->m_data.onGround) {
					// apply force
					box->Rigidbody()->applyForce(forceVector + (forceVector * combinedElasticity));

					// apply torque
					glm::vec3 centerPoint = box->GetPosition() - planeNormal;
					glm::vec3 torqueLever = glm::normalize(glm::vec3(centerPoint.y, -centerPoint.x, 0.0f));
					float torque = glm::dot(torqueLever, box->GetVelocity()) * 1.0f / (1.0f / box->Rigidbody()->m_data.mass);
					box->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, -torque));

					// move out of collision
					glm::vec3 separationVector = planeNormal * collision * 0.5f;
					box->SetPosition(box->GetPosition() - separationVector);
				}
			}
			else {
				// object colliding, stop object
				box->SetVelocity(glm::vec3(0.0f));
				box->Rigidbody()->m_data.onGround = true;
			}
			return true;
		}
	}
	return false;
}

bool Scene::boxToBox(PhysicsObject* a_boxA, PhysicsObject* a_boxB)
{
	Box* boxA = dynamic_cast<Box*>(a_boxA);
	Box* boxB = dynamic_cast<Box*>(a_boxB);

	if (boxA != nullptr && boxB != nullptr)
	{
		// collision check
		if (boxA->checkCollision(boxB))
		{
			// cache some bools for later use
			bool kinematicA = boxA->Rigidbody()->m_data.isKinematic;
			bool kinematicB = boxB->Rigidbody()->m_data.isKinematic;
			bool onGroundA = boxA->Rigidbody()->m_data.onGround;
			bool onGroundB = boxB->Rigidbody()->m_data.onGround;
			// check either is kinematic
			if (!kinematicA || !kinematicB) {
				glm::vec3 centerDist = boxB->GetPosition() - boxA->GetPosition();
				glm::vec3 boxesMaxSize = glm::vec3(boxA->GetSize() + boxB->GetSize());
				glm::vec3 collisionNormal = glm::normalize(centerDist);
				glm::vec3 overlap = abs(centerDist - boxesMaxSize);
				// if both boxs are not on the ground
				if (!onGroundA && !onGroundB)
				{
					// calculate force vector
					glm::vec3 relativeVelocity = boxA->GetVelocity() - boxB->GetVelocity();
					glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
					glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / boxA->Rigidbody()->m_data.mass + 1.0f / boxB->Rigidbody()->m_data.mass);
					// combine elasticity
					float combinedElasticity = (boxA->Rigidbody()->m_data.elasticity +
						boxB->Rigidbody()->m_data.elasticity / 2.0f);
					// use Newton's third law to apply collision forces to colliding bodies 
					boxA->Rigidbody()->applyForceToAnotherBody(boxB->Rigidbody(), forceVector + (forceVector * combinedElasticity));

					// apply torque
					float torque = glm::dot(collisionVector, relativeVelocity) * 1.0f / (1.0f / boxA->Rigidbody()->m_data.mass + 1.0f / boxB->Rigidbody()->m_data.mass);

					boxA->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, -torque));
					boxB->Rigidbody()->applyTorque(glm::vec3(0.0f, 0.0f, torque));

					// move out boxs out of collision 
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					boxA->SetPosition(boxA->GetPosition() - separationVector);
					boxB->SetPosition(boxB->GetPosition() + separationVector);
				}
				// if one box is on the ground treat collsion as plane collision
				if (onGroundA || onGroundB)
				{
					// determine moving box
					Box* box = (onGroundA ? boxB : boxA);
					Box* boxGround = (onGroundA ? boxA : boxB);
					// calculate force vector
					glm::vec3 forceVector = -1 * box->Rigidbody()->m_data.mass * collisionNormal * (glm::dot(collisionNormal, box->GetVelocity()));
					// apply force
					box->Rigidbody()->applyForce(forceVector * 2.0f);
					// move out of collision
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					box->SetPosition(box->GetPosition() - separationVector);
					// stop other box from being on ground
					boxGround->Rigidbody()->m_data.onGround = false;
				}
			}
			else
			{
				// object colliding yes, stop objects
				boxA->SetVelocity(glm::vec3(0.0f));
				boxB->SetVelocity(glm::vec3(0.0f));
				if (onGroundA || onGroundB) {
					boxA->Rigidbody()->m_data.onGround = true;
					boxB->Rigidbody()->m_data.onGround = true;
				}
			}
			return true;
		}
	}
	return false;
}


