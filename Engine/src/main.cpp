#include "iostream"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void p(const char* text) {
	std::cout << text << std::endl;
}
// on window resize callback (stretch the context)
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
// processing input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		p("a press");
}

// vertex shader source code
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // get first 3 value in a vertex
"layout (location = 1) in vec3 aCol;\n" // get second 3 value in a vertex
"out vec3 oCol;\n"
"uniform vec2 customPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y + customPos.x, aPos.z, 1.0);\n"
"	oCol = aCol;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 oCol;\n"
//"uniform vec4 customColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(oCol, 1.0f);\n"
"}\n";

void CreateShader(unsigned int shaderId, const bool isVertex) {
	// attach the source code, then compile the vertex shader
	glShaderSource(shaderId, 1, isVertex ? &vertexShaderSource : &fragmentShaderSource, NULL);
	glCompileShader(shaderId);

	// error handling
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}
void CreateProgram(unsigned int shaderProgram, unsigned int vertId, unsigned int fragId) {
	// attach vertex and fragment shader to the program
	glAttachShader(shaderProgram, vertId);
	glAttachShader(shaderProgram, fragId);
	glLinkProgram(shaderProgram);

	// error handling
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}
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
		p("GLFW Window goes wrong :P");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Init :GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		p("Failed to initialize GLAD");
		return -1;
	}

	// size of the rendering window
	glViewport(0, 0, 400, 400);
	// registering on window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#pragma endregion

	// **every object we create in opengl have unique id
	
#pragma region Rendering

	// vertex data
	float vertices[] = {
		// positions		 // colors
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f

		/*-0.5f,  0.5f,  .0f,
		 0.0f,  1.0f,  .0f,
		-1.0f,  1.0f,  .0f*/
	};
	// triangles data
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3,   // second triangle
		4, 5, 6
	};

	// VERTEX BUFFER
	// store unique id for vbo
	unsigned int VBO;
	// generate buffer for vertex buffer object (size, return id)
	glGenBuffers(1, &VBO);

	// TRIANGLES INDICES
	unsigned int EBO;
	glGenBuffers(1, &EBO);															
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);							


	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// SHADER
	// vertex and fragment shader id
	unsigned int vertexShader;
	unsigned int fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// compile the shader and link it with the shader object
	CreateShader(vertexShader,true);
	CreateShader(fragmentShader, false);
	// SHADER PROGRAM
	// create the program that contain both shader
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// attatch the shader to the program
	CreateProgram(shaderProgram, vertexShader, fragmentShader);
	// use current program to the state machine
	glUseProgram(shaderProgram);
	// cleaning up
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// LAYOUTS
	// how is the format of our vertex variable will be
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//											   VVV => size of data in one vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 0. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float) ));
	glEnableVertexAttribArray(1);
	// 2. use our shader program when we want to render an object
	glUseProgram(shaderProgram);
	// 3. now draw the object 


	// ..:: Drawing code (in render loop) :: ..
	// 4. draw the object
	glBindVertexArray(VAO);
	//someOpenGLFunctionThatDrawsOurTriangle();

#pragma endregion

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// clear previous frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangles
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0); // count of vertex drawn

		// wiremode
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// UNIFORM
		float timeValue = glfwGetTime();
		float p = (cos(timeValue) / 2.0f);
		float r = (cos(timeValue) / 2.0f) + 0.5f;
		float g = (sin(timeValue) / 2.0f) + 0.5f;
		// access uniform id
		// int vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");
		int customPosId = glGetUniformLocation(shaderProgram, "customPos");
		// use uniform id to change it
		// glUniform4f(vertexColorLocation, r, g, 0.0f, 1.0f);
		glUniform2f(customPosId, p, 0);

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