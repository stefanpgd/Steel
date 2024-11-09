#pragma once
#include "Framework/Project.h"

class Scene;
class SimpleRenderStage;

class TinyHybridProject : public Project
{
public:
	TinyHybridProject();

	void Update(float deltaTime) override;
	void Render(ComPtr<ID3D12GraphicsCommandList4> commandList) override;

private:
	Scene* scene;

	// Render Stages //
	SimpleRenderStage* simpleRenderStage;
};