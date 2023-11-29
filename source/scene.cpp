#include"scene.h"
#include"traceRay.h"
#include"Logger.h"
#include"PhysicsEngine/RigidBody.h"
#include"PhysicsEngine/Sphere.h"
#include"PhysicsEngine/Plane.h"
#include"PhysicsEngine/Box.h"
#include <glm/gtx/projection.hpp>
#include <glm/gtx/perpendicular.hpp>

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFuncs[] =
{
	Scene::planeToPlane, Scene::planeToSphere, Scene::planeToBox,
	Scene::sphereToPlane, Scene::sphereToSphere, Scene::sphereToBox,
	Scene::boxToPlane, Scene::boxToSphere, Scene::boxToBox
};

Scene::~Scene()
{
    for (auto& item : m_cubes)
    {
        delete item;
    }

    for (auto& item : m_allObjects)
    {
        delete item;
    }

    for (auto& item : m_allPhysicsObjects)
    {
        delete item;
    }

    delete m_environment;
}

void Scene::loadScene()
{
    InitializeCubes(environmentPath);
    CalculateSceneBounds();
    //m_spider = new Entity(spiderPath, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.02f));
    //m_ball1 = new Entity(ballPath, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.f));
    //m_ball2 = new Entity(ballPath, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.f));
   
    //m_allObjects.push_back(m_spider);
    //m_allObjects.push_back(m_ball1);
    //m_allObjects.push_back(m_ball2);

    //UpdateEntityToFitScene(*m_spider);
    //UpdateEntityToFitScene(*m_ball1);
    //UpdateEntityToFitScene(*m_ball2);

    //m_ball1->Translation(glm::vec3(-5.0f, 0.0f, 0.0f));
    //m_ball2->Translation(glm::vec3(5.0f, 0.0f, 0.0f)); 

    a_ball1 = new Ball(ballPath, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    a_ball2 = new Ball(ballPath, glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    
    UpdatePhysicsObjectToFitScene(*a_ball1);
    UpdatePhysicsObjectToFitScene(*a_ball2);
    
    a_ball1->Translation(glm::vec3(4.0f, 0.0f, 2.0f));
    a_ball2->Translation(glm::vec3(-4.0f, 0.0f, 2.0f));

    m_allPhysicsObjects.push_back(a_ball1);
    m_allPhysicsObjects.push_back(a_ball2);

    a_ball1->SetCurrentPosAsOriginalPos();
    a_ball2->SetCurrentPosAsOriginalPos();
    m_environment = InitializeEnvironment();
}
void  Scene::ResetScene()
{
    // reset all objects's position and velocity
    for (auto& item : m_allPhysicsObjects) 
    {
        item->ResetPosition();
        item->ResetVelocity();
		Ball* ball_item = dynamic_cast<Ball*>(item);
		if (ball_item)
		{
			ball_item->ComputeBoundingBox();
		}
    }
}
void  Scene::ClearScene()
{
    m_allPhysicsObjects.clear();
}

void Scene::UpdatePhysicsObjectToFitScene(PhysicsObject& object)
{
    Ball* box = dynamic_cast<Ball*>(&object);
    // Move spider to center of scene
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
  //  m_ball1->OnMove(deltaTime);
 //   m_ball2->OnMove(deltaTime/3);
    /*if (BoundingBox::CheckCollision(m_ball1->GetBoundingBox(), m_ball2->GetBoundingBox()))
    {
        Log::info("Objects Collision!");
        Log::info(m_ball1->getInfo());
        Log::info(m_ball2->getInfo());
        Entity::performCollision(*m_ball1, *m_ball2);
    }*/


    a_ball1->UpdatePhysics(glm::vec3(0.0f), deltaTime);
    a_ball2->UpdatePhysics(glm::vec3(0.0f), deltaTime);
	// check for collisions
	if (m_properties.collisions)
	{
		checkCollisions();
	}
}

void Scene::InitializeCubes(const std::string &filePath)
{
    m_cubes.clear();
    // Load the cube model from Model class
    int numRows = 20; 
    int numCols = 20; 
    float spacing = 2.5f; // Spacing between cubes

    // Create cubes arranged in a square grid
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            Entity* cube_i = new Entity(filePath);
            glm::vec3 position(col * spacing, 0.0f, row * spacing);
            cube_i->SetPosition(position);
            m_cubes.push_back(cube_i);
        }
    }
}

void Scene::CalculateSceneBounds()
{
    m_sceneBounds.Reset();
    for (const Entity* cube : m_cubes) 
    {
        const BoundingBox& cubeBounds = cube->GetBoundingBox();
        m_sceneBounds.ExpandToInclude(cubeBounds);
    }
}
void Scene::UpdateEntityToFitScene(Entity& entity)
{
    // Move spider to center of scene
    glm::vec3 sceneBoundsCenter = m_sceneBounds.GetCenter();
    const BoundingBox& model_bbox = entity.GetBoundingBox();
    glm::vec3 bboxCenter = model_bbox.GetCenter();
    glm::vec3 modelMove = sceneBoundsCenter - bboxCenter;

    // Adjust the y-axis of spider to move it above the scene
    float yOffset = m_sceneBounds.GetMaxBounds().y - model_bbox.GetMinBounds().y;
    modelMove.y = yOffset;

    entity.SetPosition(modelMove);
}
void Scene::RenderObjects(Shader& shader, bool isRender_BBoxes)
{
    shader.activate();
    for (Entity* cube : m_cubes) 
    {
        cube->Render(shader);
    }
    m_spider->Render(shader);

    // Render bounding boxes
    if (isRender_BBoxes)
    {
        for (auto& item : AllObjects())
        {
            BoundingBox bbox_item = item->GetBoundingBox();
            bbox_item.Render(shader);
        }
    }
    
}
void Scene::RenderPhysicsObjects(Shader& shader, bool isRender_BBoxes)
{
    shader.activate();

    for (Entity* cube : m_cubes)
    {
        cube->Render(shader);
    }
	for (auto& item : AllPhysicsObjects())
	{
		Ball* ball_item = dynamic_cast<Ball*>(item);
		if (ball_item)
		{
			ball_item->Render(shader);
		}
	}

    // Render bounding boxes
    if (isRender_BBoxes)
    {
        for (auto& item : AllPhysicsObjects())
        {
            Ball* ball = dynamic_cast<Ball*>(item);
            BoundingBox bbox_item = ball->GetBoundingBox();
            bbox_item.Render(shader);
        }
    }
}
void Scene::RenderEnvironment(Shader& shader_environment)
{
    shader_environment.activate();
    m_environment->Render(shader_environment);
}
Mesh* Scene::InitializeEnvironment()
{
    // cube vertices for vertex buffer object
    std::vector<Vertex> vertices;
    /*7------ 6
     /|      /|
    / |     / |
    3------ 2 |
    | |     | |
    | 4---- |--5
    | /     | /
    |/      |/
    0------ 1   */
    glm::vec3 vertexPositions[8] =
    {
      glm::vec3(-1.0,  1.0,  1.0),
      glm::vec3(-1.0, -1.0,  1.0),
      glm::vec3(1.0, -1.0,  1.0),
      glm::vec3(1.0,  1.0,  1.0),
      glm::vec3(-1.0,  1.0, -1.0),
      glm::vec3(-1.0, -1.0, -1.0),
      glm::vec3(1.0, -1.0, -1.0),
      glm::vec3(1.0,  1.0, -1.0),
    };

    for (auto& v : vertexPositions)
    {
        Vertex vertex;
        vertex.Position = v;
        // Set other vertex attributes if needed
        vertices.push_back(vertex);
    }
    // Flip the vertex data
    for (auto& vertex : vertices) {
        vertex.Position = glm::vec3(vertex.Position.x, -vertex.Position.y, vertex.Position.z);
    }
    // cube indices for index buffer object
    std::vector<unsigned int> indices_orig =
    {
      0, 1, 2, 3, // front 
      3, 2, 6, 7, //top
      7, 6, 5, 4, //back
      4, 5, 1, 0,//bottom
      0, 3, 7, 4,//left
      1, 2, 6, 5,//right
    };
    std::vector<unsigned int> indices =
    {
        // Front
        0, 1, 2,
        0, 2, 3,
        // Back
        4, 5, 6,
        4, 6, 7,
        // Left 
        0, 4, 3,
        3, 4, 7,
        // Right
        1, 5, 2,
        2, 5, 6,
        // Top 
        3, 2, 7,
        2, 6, 7,
        // Bottom
        0, 1, 4,
        1, 5, 4
    };
    // set up the cube map texture
    string directory("source/resources/environment/skybox1/");
    const std::string right("right.png");
    const std::string left("left.png");
    const std::string top("top.png");
    const std::string bottom("bottom.png");
    const std::string front("front.png");
    const std::string back("back.png");

    std::vector<std::string> textures_faces;
    textures_faces.push_back(directory + right);
    textures_faces.push_back(directory + left);
    textures_faces.push_back(directory + top);
    textures_faces.push_back(directory + bottom);
    textures_faces.push_back(directory + back);
    textures_faces.push_back(directory + front);
    std::vector<Texture> textures_loaded = LoadCubeMapTextures(textures_faces);

    return new Mesh(vertices, indices, textures_loaded);
}


std::vector<Texture> LoadCubeMapTextures(std::vector<std::string> textures_faces)
{
    std::vector<Texture> textures_loaded;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    // Load the cubemap texture
    for (int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            if (glGetError())
            {
                std::cout << "Texture images loaded failed" << std::endl;
            }

            stbi_image_free(data);
        }
        else
        {
            std::cout << "STBI failed to load cubemap texture: " << textures_faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    textures_loaded.push_back(Texture(textureID, "cubemap", textures_faces[0]));
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textures_loaded;
}
/*********************************************************************************************************
*                     COLLISIONS
**********************************************************************************************************/

void Scene::checkCollisions()
{
	int nbrObjects = numberOfObjects();

	// check for collisions with this object
	for (int outer = 0; outer < nbrObjects - 1; outer++)
	{
		for (int inner = outer + 1; inner < nbrObjects; inner++)
		{
			PhysicsObject* objA = m_allPhysicsObjects[outer];
			PhysicsObject* objB = m_allPhysicsObjects[inner];
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
}
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
	Ball* sphereA = dynamic_cast<Ball*>(a_sphereA);
	Ball* sphereB = dynamic_cast<Ball*>(a_sphereB);

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
				/*sphereA->SetVelocity(glm::vec3(0.0f));
				sphereB->SetVelocity(glm::vec3(0.0f));
				if (onGroundA || onGroundB)
				{
					sphereA->Rigidbody()->m_data.onGround = true;
					sphereB->Rigidbody()->m_data.onGround = true;
				}*/

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
				if (!onGroundA && !onGroundB) {
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


