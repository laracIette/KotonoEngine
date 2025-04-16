#pragma once
#include "SceneObject.h"
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
#include "Task.h"
class TMesh : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	void Init() override;
	void Update() override;
	void Cleanup() override;

	KtShader* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader* shader);
	void SetModel(KtModel* model);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;


private:
	KtShader* _shader;
	KtModel* _model;
	OTask* _spinTask;

	void Draw();
	void AddModelToRenderQueue() const;
	void AddWireframeToRenderQueue() const;

	// temp
	void Spin();
};

