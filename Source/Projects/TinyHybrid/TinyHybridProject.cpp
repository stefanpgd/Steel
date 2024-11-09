#include "Projects/TinyHybrid/TinyHybridProject.h"
#include "Projects/TinyHybrid/SimpleRenderStage.h"

#include "Framework/Scene.h"

TinyHybridProject::TinyHybridProject()
{
	scene = new Scene();
	simpleRenderStage = new SimpleRenderStage(scene);
}

void TinyHybridProject::Update(float deltaTime)
{
	scene->Update(deltaTime);
}

void TinyHybridProject::Render(ComPtr<ID3D12GraphicsCommandList4> commandList)
{
	simpleRenderStage->RecordStage(commandList);
}