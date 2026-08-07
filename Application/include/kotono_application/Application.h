#pragma once
#include <glm/ext/vector_uint2.hpp>
#ifdef EDITOR
	#include <kotono_object/Ptr.h>
	class WMainWindow;
#endif
class UApplication final
{
public:
	void Run();

private:
	void Init();
	void Update() const;
	void Cleanup() const;

	void LogUPS() const;
	void OnWindowResized(const glm::uvec2 extent) const;

private:
#	ifdef EDITOR
		UPtr<WMainWindow> mainWindow_;
#	endif
};