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
    std::vector<int*> vec1 = { new int(0), new int(1), new int(2), new int(3), new int(4), new int(5), new int(6), new int(7), new int(8), new int(9) };
    auto collection1 = KtCollection<int*>(vec1);
    collection1.AddFilter([](int* item) { return *item % 2 == 0; });
    collection1.AddFilter([](int* item) { return *item > 5; });

    for (const auto* item : collection1)
    {
        KT_DEBUG_LOG("%u", *item);
    }

    std::vector<int> vec2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    auto collection2 = KtCollection<int>(vec2);
    collection2.AddFilter([](int item) { return item % 2 == 0; });
    collection2.AddFilter([](int item) { return item > 5; });
    
    for (const auto item : collection2)
    {
        KT_DEBUG_LOG("%u", item);
    }

    auto collection3 = KtCollection<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    collection3.AddFilter([](int item) { return item % 2 == 0; });
    collection3.AddFilter([](int item) { return item > 5; });
    
    for (const auto item : collection3)
    {
        KT_DEBUG_LOG("%u", item);
    }

    return 0;

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