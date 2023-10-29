#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Window.h"
#include "Gui.h"
#include "Box.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Program.h"
#include "Texture.h"
#include "Cubemap.h"
#include "Camera.h"
#include "Common.h"

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

int main()
{
	Window mainWindow = Window(1366, 768);
	mainWindow.initialise();

	Gui gui = Gui();
	gui.initialise(mainWindow.getWindow());

	Sphere sphere = Sphere(1.0f);
	sphere.initialise();

	Skybox skybox = Skybox(20.0f);
	skybox.initialise();

	Program mainProgram = Program();
	mainProgram.createFromFiles("main.vert", "main.frag");
	mainProgram.genVertexBuffers();
	mainProgram.genFragmentBuffers();

	Program cubemapProgram = Program();
	cubemapProgram.createFromFiles("skybox.vert", "skybox.frag");

	Cubemap cubemap = Cubemap();
	std::vector<std::string> faces;
	faces.push_back("LancellottiChapel/right.jpg");
	faces.push_back("LancellottiChapel/left.jpg");
	faces.push_back("LancellottiChapel/top.jpg");
	faces.push_back("LancellottiChapel/bottom.jpg");
	faces.push_back("LancellottiChapel/front.jpg");
	faces.push_back("LancellottiChapel/back.jpg");
	cubemap.initialise(faces);

	Texture earthTexture = Texture();
	earthTexture.initialise("textures/map.jpg");

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3(0.0f, 0.0f, -1.0f));

	// Control
	bool useTexture = true;
	bool wireFrame = false;
	bool backFaceCull = true;

	// Model
	glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaling = { 0.4f, 0.4f, 0.4f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };

	// Projection
	float aspectRatio = (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight();
	glm::mat4 perspectiveProjection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	glm::mat4 orthographicProjection = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);
	bool usePerspective = true;

	// Material
	Material material;
	material.ambient = 0.1f;
	material.shininess = 1.0f;
	material.diffuse = 1.0f;
	material.specular = 1.0f;

	// Light
	Light light;
	light.position = { 0.0f, 0.0f, 2.0f };
	light.direction = { 0.0f, 0.0f, -1.0f };
	light.strength = 1.0;
	light.fallOffStart = 0.0f;
	light.fallOffEnd = 10.0f;
	light.spotPower = 1.0f;
	light.isDirectional = 0;
	light.isPoint = 0;
	light.isSpot = 1;
	light.useBlinnPhong = true;

	//glm::mat4 model(1.0f);
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();
	
		gui.update(useTexture, wireFrame, backFaceCull,
			translation.x, scaling.x, rotation.x,
			camera.getRotation(),
			usePerspective, material, light);

		glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);
		backFaceCull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

		mainWindow.clear(0.0f, 0.0f, 0.0f, 1.0f);

		// Projection
		glm::mat4 projection = usePerspective ? perspectiveProjection : orthographicProjection;

		// Model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(translation.x , translation.y, translation.z));
		model = glm::scale(model, scaling);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glDepthMask(GL_FALSE);
		cubemapProgram.use();
		cubemap.use();
		skybox.draw();
		glDepthMask(GL_TRUE);

		mainProgram.use();
		mainProgram.bindVertexBuffers(model, projection, camera.calculateViewMatrix());
		mainProgram.bindFragmentBuffers(useTexture, camera.getPosition(), material, light);
		earthTexture.use();
		sphere.draw();


		gui.render();

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		mainWindow.swapBuffers();
	}
}