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
	Window mainWindow = Window(1920, 1080);
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

	Cubemap diffuseCubemap = Cubemap();
	faces.clear();
	faces.push_back("LancellottiChapel/diffuse-right.jpg");
	faces.push_back("LancellottiChapel/diffuse-left.jpg");
	faces.push_back("LancellottiChapel/diffuse-top.jpg");
	faces.push_back("LancellottiChapel/diffuse-bottom.jpg");
	faces.push_back("LancellottiChapel/diffuse-front.jpg");
	faces.push_back("LancellottiChapel/diffuse-back.jpg");
	diffuseCubemap.initialise(faces);

	Texture earthTexture = Texture();
	earthTexture.initialise("textures/map.jpg");

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3(0.0f, 0.0f, -1.0f));

	// Control
	bool useTexture = true;

	// Model
	glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaling = { 0.4f, 0.4f, 0.4f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };

	// Projection
	float aspectRatio = (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// Material
	Material material;
	material.ambient = 0.1f;
	material.shininess = 1.0f;
	material.diffuse = 1.0f;
	material.specular = 1.0f;

	//glm::mat4 model(1.0f);
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();
	
		gui.update(useTexture, translation.x, scaling.x, rotation.x, camera.getRotation(), material);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);

		mainWindow.clear(0.0f, 0.0f, 0.0f, 1.0f);

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
		diffuseCubemap.use(1);
		skybox.draw();
		glDepthMask(GL_TRUE);

		mainProgram.use();
		mainProgram.bindVertexBuffers(model, projection, camera.calculateViewMatrix());
		mainProgram.bindFragmentBuffers(useTexture, camera.getPosition(), material);
		earthTexture.use();
		sphere.draw();

		gui.render();

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		mainWindow.swapBuffers();
	}
}