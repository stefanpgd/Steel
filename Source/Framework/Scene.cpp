#include "Framework/Scene.h"
#include "Graphics/Model.h"
#include "Graphics/Camera.h"

Scene::Scene(bool spawnDefaultObjects)
{
	if(spawnDefaultObjects)
	{
		AddModel("Assets/Models/Default/GroundPlane/plane.gltf");
		//AddModel("Assets/Models/Default/Dragon/dragon.gltf");
		AddModel("Assets/Models/Default/FlightHelmet/FlightHelmet.gltf");
	}

	Camera = new ::Camera();
}

void Scene::Update(float deltaTime)
{
	Camera->Update();
}

void Scene::AddModel(const std::string& path)
{
	models.push_back(new Model(path, true));
}

const std::vector<Model*>& Scene::GetModels()
{
	return models;
}