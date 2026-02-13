#define GLAD_GL_IMPLEMENTATION
#include <GLAD/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Rendering commands here
	Shader ourShader("Shaders/VertexShader.glsl","Shaders/FragmentShader.glsl");
	ourShader.use();

	float vertices[] = {
			// positions		// colors		// UV coords
		    -0.7f, -0.5f, 0.0f, 1.0f, 0.8f, 0.8f, 0.0f, 0.0f,
		    -0.4f,  0.2f, 0.0f, 0.8f, 1.0f, 1.0f, 0.5f, 1.0f,
		    -0.1f, -0.5f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f, 0.0f,
			 0.1f, -0.5f, 0.0f, 1.0f, 0.8f, 1.0f, 0.0f, 0.0f,
			 0.4f,  0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f,
			 0.7f, -0.5f, 0.0f, 1.0f, 0.8f, 1.0f, 1.0f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5
	};
	unsigned int VBO;
	unsigned int EBO;
	unsigned int VAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int textures[2];
	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Textures/Poliigon_BrickWallReclaimed_8320_BaseColor.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture.\n";
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	data = stbi_load("Textures/smile.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture.\n";
	}
	stbi_image_free(data);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);

	// -----
	//glm::vec4 vec(1.0f, 1.0f, 1.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(10.f), glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 2.0f));
	//trans = glm::translate(trans, glm::vec3(0.2f, -0.3f, 0.0f));
	//std::cout << vec.x << vec.y << vec.z << "\n";
	int frame = 0;
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//rendering commands here
		ourShader.use();
		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) * 0.4f) + 0.6f;

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(),
			glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.7f,0.7f,0.7f));
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE,glm::value_ptr(trans));
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		

		frame++;
		// -----
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}