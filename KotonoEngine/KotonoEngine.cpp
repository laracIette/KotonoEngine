#include "KotonoEngine.h"
#include "EICFileExplorer.h"
#include "RoundedBox.h"
#include "vulkan_helper.h"

KotonoEngine::KotonoEngine() :
    _window(nullptr),
    //_framebuffer(nullptr),
    _audioManager(nullptr),
    _inputManager(nullptr),
    _textureManager(nullptr),
    _shaderManager(nullptr),
    _engineInterfaceComponentManager(nullptr),
    _updateRate(1000.0f),
    _drawRate(60.0f),
    _projectDirectory(R"(C:\Users\nicos\Documents\Kotono Projects\Test)")
{
}

KotonoEngine::~KotonoEngine()
{
    delete _engineInterfaceComponentManager;
    delete _shaderManager;
    delete _textureManager;
    delete _inputManager;
    delete _audioManager;
    //delete _framebuffer;
    delete _window;
}

void KotonoEngine::Run()
{
    // Init
    _window = new Window();
    _window->Init();

    //_framebuffer = new Framebuffer();

    _audioManager = new AudioManager();
    _audioManager->Init();

    _inputManager = new InputManager();

    _textureManager = new TextureManager();

    _shaderManager = new ShaderManager();

    _engineInterfaceComponentManager = new EngineInterfaceComponentManager();

    LogVulkanError("Kotono Engine Initialization");

    auto lastUpdateTime = std::chrono::steady_clock::now();
    auto lastDrawTime = std::chrono::steady_clock::now();

    // MainLoop
    while (_window->IsRunning())
    {
        LogVulkanError("Update");
        const auto now = std::chrono::steady_clock::now();

        // Update
        const auto updateDeltaTime = std::chrono::duration<float>(now - lastUpdateTime);

        if (updateDeltaTime >= GetUpdateTime())
        {
            const auto overtime = std::chrono::duration<float>(updateDeltaTime - GetUpdateTime());
            lastUpdateTime = now - std::chrono::duration_cast<std::chrono::steady_clock::duration>(overtime);

            _engineInterfaceComponentManager->Update(updateDeltaTime.count());
        }

        // Draw
        auto drawDeltaTime = std::chrono::duration<float>(now - lastDrawTime);

        if (drawDeltaTime >= GetDrawTime())
        {
            const auto overtime = std::chrono::duration<float>(drawDeltaTime - GetDrawTime());
            lastDrawTime = now - std::chrono::duration_cast<std::chrono::steady_clock::duration>(overtime);

            //_framebuffer->BeginDraw();
            _engineInterfaceComponentManager->Draw();
            //_framebuffer->DrawTextures();
            _window->SwapBuffers();
        }
    }
}

Window* KotonoEngine::GetWindow() const
{
    return _window;
}

//Framebuffer* KotonoEngine::GetFramebuffer() const
//{
//    return _framebuffer;
//}

AudioManager* KotonoEngine::GetAudioManager() const
{
    return _audioManager;
}

InputManager* KotonoEngine::GetInputManager() const
{
    return _inputManager;
}

ShaderManager* KotonoEngine::GetShaderManager() const
{
    return _shaderManager;
}

TextureManager* KotonoEngine::GetTextureManager() const
{
    return _textureManager;
}

EngineInterfaceComponentManager* KotonoEngine::GetEngineInterfaceComponentManager() const
{
    return _engineInterfaceComponentManager;
}

const std::string& KotonoEngine::GetProjectDirectory() const
{
    return _projectDirectory;
}

const std::chrono::duration<float> KotonoEngine::GetUpdateTime() const
{
    return std::chrono::duration<float>(1.0f / _updateRate);
}

const std::chrono::duration<float> KotonoEngine::GetDrawTime() const
{
    return std::chrono::duration<float>(1.0f / _drawRate);
}
