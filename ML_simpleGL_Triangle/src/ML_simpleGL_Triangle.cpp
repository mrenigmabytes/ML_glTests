// %BANNER_BEGIN%
// ---------------------------------------------------------------------
// %COPYRIGHT_BEGIN%
//
// Copyright (c) 2018 Magic Leap, Inc. All Rights Reserved.
// Use of this file is governed by the Creator Agreement, located
// here: https://id.magicleap.com/creator-terms
//
// %COPYRIGHT_END%
// ---------------------------------------------------------------------
// %BANNER_END%

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Header contains our shader as a string.
#include "ML_simpleGL_Triangle.h"

// Magic Leap library for logging. You can replace with std out
#include <ml_logging.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
	if (glfwInit() == GLFW_FALSE)
	{
		ML_LOG(Error, "glfwInit() Failed.");
		exit(1);
	}

	// Window hints to glfw - specify openGl version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// then specity the openGl profile to use
	// remember that CORE_PROFILE essentially removes old depricated gl functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Stupid compatibility snipped for OSX losers
#ifdef ML_OSX
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create window object, push it to the current graphics context and also add callback.
	// The callback registration is so that modifications to the window call our 'reset' function.
	GLFWwindow* window = glfwCreateWindow(800, 600, "Draw Triangle", NULL, NULL);
	if (!window)
	{
		ML_LOG(Error, "Could not Create window - glfwCreateWindow()");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// if setting the context above failed, then the 'glfwGetProcAddress' function below fails.
	// and will return: GLFW_NO_CURRENT_CONTEXT I am not checking for the error since it is being set above.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ML_LOG(Error,"Glad load error, Invalid context address.");
		exit(1);
	}

	ML_LOG(Info, "Drawing Triangle");

	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f,
		0.5f,-0.5f,0.0f
	};

	// Create a buffer to store our vertex data and assign it to a buffer type
	// in our case we want a GL_ARRAY_BUFFER. vert_BO holds an ID representing the buffer we just created
	unsigned int vert_BO;
	glGenBuffers(1, &vert_BO);
	// Any buffer function calls are now operating on ver_BO becase it is the 
	// bound buffer and therefore what we are operating on unless we change
	// to a different buffer. 
	glBindBuffer(GL_ARRAY_BUFFER, vert_BO);;
	// You can only bind 1 object to a buffer type at a time. GL_ARRAY_BUFFER is bound to the 
	// to the buffer ID stored in vert_BO. 
	// Now we tell opengl to pipe our data (vertices) into the buffer that is bound (vert_BO) to the
	// GL_ARRAY_BUFFER and we specify the sze of teh data , the data, and the management type of the data:
	// GL_STATIC_DRAW - the data has little changes.
	// GL_DYNAMIC_DRAW - the data changes a bit.
	// GL_STREAM_DRAW - data will change on every call to draw. (frame).
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#pragma region VertexShader
	// Time to load our vertex shader data into openGl.
	// Here we are creating a vertex shader object and storing the generated
	// object id to our variable 'vertexShader'
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// After creating the shader object and assigning it an id we must assign the 
	// actual source code for the shader that will need to be compiled.
	// We also instruct the framework to go ahead and compile the shader.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Because the shader compilation is different from this class compilation we want to 
	// verify compilation. Remember that shaders are external code files that get loaded and compiled
	// at run time. We can perform a runtime compile check to see if all things got executed.
	GLint result;
	// NOTE: 'glGetShaderiv()' returns information about a shader. The second parameter is limited to
	// GL_SHADER_TYPE, GL_DELETE_STATUS, GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH,GL_SHADER_SOURCE_LENGTH
	// We are interested in GL_COMPILE_STATUS which resturns GL_TRUE or GL_FALSE which are GLint datatypes.
	// See Also: http://docs.gl/gl4/glGetShader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	GLint logSize;
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
	GLchar shaderLog[1024];
	glGetShaderInfoLog(vertexShader, logSize, NULL, shaderLog);
	if (!result)
	{
		ML_LOG(Error, shaderLog);
		exit(1);
	}
	else
	{
		ML_LOG(Info, "Shader Compilation Succesful\n");
		ML_LOG(Info, shaderLog);
	}
#pragma endregion

#pragma region FragmentShader
	// GLuint is same as unsigned int, here we use the gl version to conceptually prove 
	// it also is meant to be used instead of unsigned int to guarantee the code works across multiple systems.
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	result, logSize = NULL;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
	glGetShaderInfoLog(fragmentShader, logSize, NULL, shaderLog);

	if (!result)
	{
		ML_LOG(Error, shaderLog);
		exit(1);
	}
	else
	{
		ML_LOG(Info, "Shader Compilation Succesful\n");
		ML_LOG(Info, shaderLog);
	}
#pragma endregion

	result, logSize = NULL;
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logSize);
	glGetProgramInfoLog(shaderProgram, logSize, NULL, shaderLog);
	
	if (!result)
	{
		ML_LOG(Error, "Shader Assembly Unsuccseful.");
		exit(1);
	}
	else
	{
		ML_LOG(Info, "Shader Program Succesful\n");
		ML_LOG(Info, shaderLog);
	}
	// This is the final assembly. Fromn now on OpenGL wil use our shader program
	// every time it needs to render vertex data.
	glUseProgram(shaderProgram);
	// Since the shader objects have been added to the program now we can actually 
	// delete them because they are copied over.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
