#include "Framework/Scene.h"
#include "Graphics/Model.h"

Scene::Scene()
{
	//AddModel("Assets/Models/Sponza/sponza.gltf");
}

void Scene::AddModel(const std::string& path)
{
	models.push_back(new Model(path, true));
}

const std::vector<Model*>& Scene::GetModels()
{
	return models;
}