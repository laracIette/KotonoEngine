#pragma once
#include "Window.h"
#include "AudioManager.h"
#include "ObjectManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "EngineInterfaceComponentManager.h"
class KotonoEngine final
{
public:
	KotonoEngine();
	~KotonoEngine();

	void Run();

	Window* GetWindow() const;
	//Framebuffer* GetFramebuffer() const;
	AudioManager* GetAudioManager() const;
	InputManager* GetInputManager() const;
	TextureManager* GetTextureManager() const;
	ShaderManager* GetShaderManager() const;
	EngineInterfaceComponentManager* GetEngineInterfaceComponentManager() const;

	const std::string& GetProjectDirectory() const;

private:
	Window* _window;
	//Framebuffer* _framebuffer;
	AudioManager* _audioManager;
	InputManager* _inputManager;
	ShaderManager* _shaderManager;
	TextureManager* _textureManager;
	EngineInterfaceComponentManager* _engineInterfaceComponentManager;

	float _updateRate;
	float _drawRate;

	std::string _projectDirectory;

	const std::chrono::duration<float> GetUpdateTime() const;
	const std::chrono::duration<float> GetDrawTime() const;
};

extern KotonoEngine* Engine;

