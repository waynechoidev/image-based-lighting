#include "Gui.h"

void Gui::initialise(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.FontGlobalScale = 1.5f;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Gui::update(bool& useTexture, float& translation, float& scaling, float& rotation,	float* viewRotation, Material& material)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Scene Control");

	// Print framerate
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Checkbox("Use Texture", &useTexture);

	ImGui::Text("Model");
	ImGui::SliderFloat3("Translation", &translation, -3.0f, 3.0f);
	ImGui::SliderFloat3("Rotation", &rotation, -3.14f, 3.14f);
	//ImGui::SliderFloat3("Scaling", &scaling, 0.1f, 2.0f);

	ImGui::Text("View");
	ImGui::SliderFloat3("View rotation", viewRotation, -3.14f, 3.14f);

	ImGui::Text("Material");
	ImGui::SliderFloat("Diffuse", &material.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &material.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("Shininess", &material.shininess, 0.0f, 1.0f);

	ImGui::End();
}

void Gui::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
