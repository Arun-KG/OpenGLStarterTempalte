#include <iostream>
#include <string>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ErrorHandle.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"

static std::string ReadFileAsString(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if(in)
	{
		in.seekg(0, std::ios::end);
		result.resize((size_t) in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		std::cout << "Could not open file '" << filepath << "'" << std::endl;
	}

	return result;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	IFDEBUG(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE));

	GLFWwindow* window = glfwCreateWindow(1280, 720, "LearnOpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "Using openGL version : " << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, 1280, 720);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	IFDEBUG(InitGLErrorHandling());

	//=======================================Draw code

	//float vertices[] = {
	// 0.5f,  0.5f, 0.0f,  // top right
	// 0.5f, -0.5f, 0.0f,  // bottom right
	//-0.5f, -0.5f, 0.0f,  // bottom left
	//-0.5f,  0.5f, 0.0f   // top left 
	//};

	float vertices[] = {
	 //Position          //Color
	 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 
	};


	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	//uint32_t VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, sizeof(indices));


	std::string vs = ReadFileAsString("src/shaders/test_vert.glsl");
	const char* vertexSource = vs.c_str();

	std::string fs = ReadFileAsString("src/shaders/test_frag.glsl");
	const char* fragmentSource = fs.c_str();


	uint32_t vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	int vertexShaderStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);
	if(vertexShaderStatus != GL_TRUE)
	{
		int log_length = 0;
		char message[1024];
		glGetShaderInfoLog(vertexShader, 1024, &log_length, message);

		std::cout << message << std::endl;
		return -1;
	}

	uint32_t fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	int fragmentShaderStatus;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderStatus);
	if(fragmentShaderStatus != GL_TRUE)
	{
		int log_length = 0;
		char message[1024];
		glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);

		std::cout << message << std::endl;
		return -1;
	}

	uint32_t shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if(linkStatus != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(shaderProgram, 1024, &log_length, message);

		std::cout << message << std::endl;
		return -1;
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//glBindVertexArray(0);
	va.Bind();
	ib.Bind();

	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

	

	//Draw as lines
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Imgui stuff
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void) io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	bool show_demo_window = false;
	float colo = 0.0f;

	int colorLocation;

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if(show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
						
			if(ImGui::Button( show_demo_window ? "Hide Demo Window" : "Show Demo Window"))
			{
				show_demo_window = !show_demo_window;
			}

			ImGui::DragFloat("color", &colo, 0.005f, -1.0f, 0.0f);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		

		colorLocation = glGetUniformLocation(shaderProgram, "clr");
		glUseProgram(shaderProgram);
		glUniform1f(colorLocation, colo);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}


	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}