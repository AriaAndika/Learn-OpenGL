#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// on window resize callback (stretch the context)
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
// processing input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		std::cout << "a press" << std::endl;
}

int main()
{

#pragma region Init

	// Init :GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create, error check, and set the window to current "Context" :GLFW
	auto* window = glfwCreateWindow(400, 400, "Nice", NULL, NULL);
	if (!window) {
		std::cout << "GLFW Window goes wrong :P" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Init :GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// size of the rendering window
	glViewport(0, 0, 400, 400);
	// registering on window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#pragma endregion
		
#pragma region Rendering

	// vertex data
	//float vertices[] = {
	//	// positions		 // colors
	//	-0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	//	 0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

	//	 0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

	//	-0.5f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
	//	 0.5f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f
	//};
	float vertices[] = {
		-0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 0.5f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f,  7.0f, 0.0f,

		 0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f,  0.5f, 7.0f,

		-0.5f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 0.5f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f,  7.0f, 0.0f
	};						
	// triangles data
	unsigned int indices[] = {  // note that we start from 0!
		0, 2, 1,
		2, 3, 4
	};

	// VERTEX BUFFER
	// store unique id for vbo

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// for color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float) ));
	glEnableVertexAttribArray(1);
	// for tex coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// SHADER
	Shader ourShader("res/Shaders/Basic.shader");
	ourShader.use();


	// TEXTURE
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	// "res/Textures/Image.jpg"
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("res/Textures/thinking.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);

#pragma endregion

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// clear previous frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangles
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0); // count of vertex drawn

		
		
		// UNIFORM
		auto time = sin(glfwGetTime()) / 2;
		ourShader.setFloat("Input", time, 0.0f);
		ourShader.setFloat("offset", time * 7, 0.0f);

		// call process input
		// processInput(window);

		// swap the buffers and get event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// cleanup
	glfwTerminate();
	return 0;
}
