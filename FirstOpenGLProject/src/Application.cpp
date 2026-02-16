#define GLAD_GL_IMPLEMENTATION
#include <GLAD/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Shader.h"
#include "../Camera.h"
#include "../Light.h"
#include "../Model.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;
bool firstMovement = true;
Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.cameraPos += cameraSpeed * glm::normalize(camera.cameraFront);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.cameraPos -= cameraSpeed * glm::normalize(camera.cameraFront);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.cameraPos -= cameraSpeed * glm::normalize(cross(camera.cameraFront, camera.cameraUp));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.cameraPos += cameraSpeed * glm::normalize(cross(camera.cameraFront, camera.cameraUp));
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.cameraPos.y += cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.cameraPos.y -= cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMovement)
	{
		firstMovement = false;
		lastX = xpos;
		lastY = ypos;
	}
	float xOffset = xpos - lastX;
	float yOffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	camera.yaw += xOffset;
	camera.pitch -= yOffset;

	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.zoom -= (float)yoffset;
	if (camera.zoom < 1.0f)
		camera.zoom = 1.0f;
	if (camera.zoom > 45.0f)
		camera.zoom = 45.0f;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	// 2. Center the window
	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);
	int windowWidth = 800; // Your desired width
	int windowHeight = 600; // Your desired height

	glfwSetWindowPos(
		window,
		monitorX + (mode->width - windowWidth) / 2,
		monitorY + (mode->height - windowHeight) / 2
	);

	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	camera = Camera(fbWidth, fbHeight);

	// Rendering commands here

	Shader ourShader("Shaders/VertexShader.glsl","Shaders/FragmentShader.glsl");
	ourShader.use();

	float vertices[] = {
			// positions		// UV coords
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	glm::vec3 objectColors[] = {
		glm::vec3(1.0f, 0.0f, 0.0f),  // Red
		glm::vec3(0.0f, 1.0f, 0.0f),  // Green
		glm::vec3(0.0f, 0.0f, 1.0f),  // Blue
		glm::vec3(1.0f, 1.0f, 0.0f),  // Yellow
		glm::vec3(1.0f, 0.0f, 1.0f),  // Magenta
		glm::vec3(0.0f, 1.0f, 1.0f),  // Cyan
		glm::vec3(1.0f, 0.5f, 0.0f),  // Orange
		glm::vec3(0.5f, 0.0f, 1.0f),  // Purple
		glm::vec3(0.6f, 0.3f, 0.1f),  // Brown
		glm::vec3(0.2f, 0.8f, 0.2f)   // Lime green
	};
	unsigned int VBO;
	//unsigned int EBO;
	unsigned int VAO;
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int textures[2];
	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Textures/container-diffuse.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture.\n";
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	data = stbi_load("Textures/container-specular.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture.\n";
	}
	stbi_image_free(data);
	/*ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);*/

	//loading model
	Model ourModel("Models/backpack.obj");

	glEnable(GL_DEPTH_TEST);


	// Lighting Shader
	// point light cube
	Shader lightsShader("Shaders/LightsVertexShader.glsl","Shaders/LightsFragmentShader.glsl");
	lightsShader.use();
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// directional light arrow
	Model arrowModel1("Models/Arrow1.obj");
	Model arrowModel2("Models/Arrow2.obj");
	// -----

	// Light properties
	std::vector<Light> lights;
	lights.push_back(Light::DirectionalLight(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f), glm::vec3(1.0f, -0.5f, 0.0f)));
	
	lights.push_back(Light::PointLight(glm::vec3(0.0f, 0.0f, 0.3f), glm::vec3(0.3f), glm::vec3(2.0f, 1.0f, 0.0f)));
	lights.push_back(Light::PointLight(glm::vec3(0.0f, 0.3f, 0.0f), glm::vec3(0.3f), glm::vec3(-2.0f, 1.0f, 0.0f)));
	lights.push_back(Light::SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.7f), camera.cameraPos, camera.cameraFront, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f))));


	int frame = 0;
	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//rendering commands here
		ourShader.use();
		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ourShader.setMat4("view", camera.view());
		ourShader.setMat4("projection", camera.projection());
		ourShader.setInt("numLights", lights.size());
		lights[lights.size() - 1].UpdateFlashlight(camera); // Update spotlight to follow the camera
		for (int i = 0; i < lights.size(); i++)
		{
			ourShader.setVec3( "lights[" + std::to_string(i) + "].ambient",     lights[i].ambient);
			ourShader.setVec3( "lights[" + std::to_string(i) + "].diffuse",     lights[i].diffuse);
			ourShader.setVec3( "lights[" + std::to_string(i) + "].specular",    lights[i].specular);
			ourShader.setVec3( "lights[" + std::to_string(i) + "].position",    lights[i].position);
			ourShader.setVec3( "lights[" + std::to_string(i) + "].direction",   lights[i].direction);
			ourShader.setFloat("lights[" + std::to_string(i) + "].cutOff",      lights[i].cutOff);
			ourShader.setFloat("lights[" + std::to_string(i) + "].outerCutOff", lights[i].outerCutOff);
			ourShader.setInt(  "lights[" + std::to_string(i) + "].type",        lights[i].type); // 0: directional light, 1: point light, 2: spotlight
			ourShader.setFloat("lights[" + std::to_string(i) + "].constant",    lights[i].constant);
			ourShader.setFloat("lights[" + std::to_string(i) + "].linear",      lights[i].linear);
			ourShader.setFloat("lights[" + std::to_string(i) + "].quadratic",   lights[i].quadratic);
		}
		ourShader.setVec3("viewPos", camera.cameraPos);
		//material properties
		ourShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		ourShader.setFloat("material.shininess", 32.0f);
		//
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		ourShader.setInt("material.diffuse", 0);
		ourShader.setInt("material.specular", 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (int i = 0; i < _countof(cubePositions); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle),
				glm::vec3(1.0f, 0.3f, 0.5f));

			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", objectColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);
		lightsShader.use();
		for (int i = 0; i < lights.size(); i++)
		{
			// directional light arrow
			if (lights[i].type == 0)
			{
				// draw overlay in screen space but keep full 3D orientation (no flattening to 2D)
				glm::mat4 orthoProj = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -100.0f, 100.0f);
				lightsShader.setMat4("projection", orthoProj);
				lightsShader.setMat4("view", glm::mat4(1.0f)); // identity view for HUD overlay

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(windowWidth - 80.0f, windowHeight - 80.0f, 0.0f));

				// world -> view rotation (we want arrow oriented relative to camera)
				glm::vec3 worldDir = glm::normalize(lights[i].direction);
				glm::mat3 viewRot = glm::mat3(camera.view());
				glm::vec3 dirInView = glm::normalize(viewRot * worldDir);

				// Because orthographic overlay uses screen coords with Y down (top=windowHeight),
				// flip Y so arrow orientation matches screen space.
				dirInView.y = -dirInView.y;

				// Model's forward vector: change if your arrow model's 'forward' isn't +Y.
				glm::vec3 from = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
				glm::vec3 to = glm::normalize(dirInView);

				// robust angle-axis rotation from 'from' to 'to'
				float cosTheta = glm::clamp(glm::dot(from, to), -1.0f, 1.0f);
				glm::mat4 rotationMat = glm::mat4(1.0f);

				if (cosTheta > 0.9999f)
				{
					// vectors nearly equal -> no rotation
				}
				else if (cosTheta < -0.9999f)
				{
					// vectors opposite -> rotate 180 degrees around any perpendicular axis
					glm::vec3 axis = glm::cross(from, glm::vec3(1.0f, 0.0f, 0.0f));
					if (glm::dot(axis, axis) < 0.0001f)
						axis = glm::cross(from, glm::vec3(0.0f, 0.0f, 1.0f));
					axis = glm::normalize(axis);
					rotationMat = glm::rotate(glm::mat4(1.0f), glm::pi<float>(), axis);
				}
				else
				{
					glm::vec3 axis = glm::normalize(glm::cross(from, to));
					float angle = acos(cosTheta);
					rotationMat = glm::rotate(glm::mat4(1.0f), angle, axis);
				}

				// apply rotation and scale (scale after rotation so arrow keeps correct orientation)
				model = model * rotationMat;
				model = glm::scale(model, glm::vec3(10.0f));

				lightsShader.setMat4("model", model);
				lightsShader.setVec3("lightColor", lights[i].diffuse * 0.7f);
				arrowModel1.Draw(lightsShader); // render an arrow for the directional light
				lightsShader.setVec3("lightColor", lights[i].diffuse);
				arrowModel2.Draw(lightsShader);
			}
			if (lights[i].type != 1) continue; // Skip directional and spot lights for rendering
			lightsShader.setMat4("view", camera.view());
			lightsShader.setMat4("projection", camera.projection());
			glBindVertexArray(lightVAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lights[i].position);
			model = glm::scale(model, glm::vec3(0.2f));
			lightsShader.setVec3("lightColor", lights[i].diffuse);
			lightsShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0);
		
		//lightColor = glm::vec3(fmod(lastFrame, 5.0f) / 5.0f);
		frame++;
		// -----
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}