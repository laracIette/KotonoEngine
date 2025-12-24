#include "Scene.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SceneMeshComponent.h"
#include "SceneMeshObject.h"
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Model.h>
#include <kotono_common/log.h>

KScene::KScene(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
}

void KScene::Cleanup()
{
	for (const auto& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->Delete();
		}
	}
	sceneObjects_.Clear();

	Base::Cleanup();
}

void KScene::UpdateSceneObjects()
{
	for (auto& sceneObject : sceneObjects_)
	{
		if (sceneObject->GetCanUpdate())
		{
			sceneObject->Update();
		}

		sceneObject->UpdateSceneComponents();
	}
}

void KScene::Add(const UPtr<TSceneObject>& sceneObject)
{
	sceneObjects_.Add(sceneObject);
}

void KScene::Remove(const UPtr<TSceneObject>& sceneObject)
{
	sceneObjects_.Remove(sceneObject);
}

void KScene::SpawnSceneObjects()
{
	for (auto& sceneObject : sceneObjects_)
	{
		sceneObject->Spawn();
	}
}

const KtPool<UPtr<TSceneObject>>& KScene::SceneObjects() const
{
	return sceneObjects_;
}
