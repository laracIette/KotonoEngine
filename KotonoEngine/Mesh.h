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

	void AddToRenderQueue() const;

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

	void Spin();

private:
	KtShader* _shader;
	KtModel* _model;
	OTask* _spinTask;
};

