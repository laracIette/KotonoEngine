#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include "Engine.h"
#include "Time.h"
#include "ObjectManager.h"


#include <kotono_framework/Collection.h>


int main()
{
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    auto collection = KtCollection<int>(vec);
    collection.AddFilter([](int item) { return item % 2 == 0; });
    collection.AddFilter([](int item) { return item > 6; });

    for (const int item : collection.GetResult())
    {
        KT_DEBUG_LOG("%u", item);
    }

    Framework.Init();
    Engine.Init();

    while (!Framework.GetWindow().GetShouldClose())
    {
        Framework.GetInputManager().Update();
        Engine.GetTime().Update();
        Engine.GetObjectManager().Update();
        Framework.GetRenderer().DrawFrame();
    }

    Engine.Cleanup();
    Framework.Cleanup();

	return 0;
}