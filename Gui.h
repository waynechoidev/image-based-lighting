#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "Common.h"

class Gui
{
public:
	Gui() {};
	
	void initialise(GLFWwindow* window);
	void update(bool& useTexture, bool& wireFrame, bool& backFaceCull, 
		float& translation, float& scaling, float& rotation,
		bool& usePerspective, Material& material, Light& light);
	void render();

	~Gui() {};
};

