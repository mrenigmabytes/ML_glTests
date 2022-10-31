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

#include "ML_simpleGL_HelloWorld.h"

#include <chrono>
#include <cmath>

#include <ml_graphics.h>
#include <ml_head_tracking.h>
#include <ml_perception.h>
#include <ml_lifecycle.h>
#include <ml_logging.h>

// Adding GL_WINDOW directive mixed with DEBUG mode. This is to specifically omit Magic Leap
// calls and just try and render vanilla OpenGL windows. When DEBUG and NOT GL_WINDOW 
// then it calls perception to render to virtual environment.


// Constants
const char application_name[] = "com.magicleap.helloworld";
#if USE_GLFW
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
#endif

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#if USE_GLFW

graphics_context_t::graphics_context_t() {
	if (!glfwInit()) {
		ML_LOG(Error, "glfwInit() failed");
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#if ML_OSX
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// We open a 1x1 window here -- this is not where the action happens, however;
	// this program renders to a texture.  This is done merely to make GL happy.
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, application_name, NULL, NULL);
	if (!window) {
		ML_LOG(Error, "glfwCreateWindow() failed");
		exit(1);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		ML_LOG(Error, "gladLoadGLLoader() failed");
		exit(1);
	}
}

void graphics_context_t::makeCurrent() {
	glfwMakeContextCurrent(window);
}

void graphics_context_t::unmakeCurrent() {
}

void graphics_context_t::swapBuffers() {
	glfwSwapBuffers(window);
}

graphics_context_t::~graphics_context_t() {
	glfwDestroyWindow(window);
	window = nullptr;
}

#else // !USE_GLFW

graphics_context_t::graphics_context_t() {
	egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	EGLint major = 4;
	EGLint minor = 0;
	eglInitialize(egl_display, &major, &minor);
	eglBindAPI(EGL_OPENGL_API);

	EGLint config_attribs[] = {
	  EGL_RED_SIZE, 5,
	  EGL_GREEN_SIZE, 6,
	  EGL_BLUE_SIZE, 5,
	  EGL_ALPHA_SIZE, 0,
	  EGL_DEPTH_SIZE, 24,
	  EGL_STENCIL_SIZE, 8,
	  EGL_NONE
	};
	EGLConfig egl_config = nullptr;
	EGLint config_size = 0;
	eglChooseConfig(egl_display, config_attribs, &egl_config, 1, &config_size);

	EGLint context_attribs[] = {
	  EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
	  EGL_CONTEXT_MINOR_VERSION_KHR, 0,
	  EGL_NONE
	};
	egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);
}

void graphics_context_t::makeCurrent() {
	eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_context);
}

void graphics_context_t::unmakeCurrent() {
	eglMakeCurrent(NULL, EGL_NO_SURFACE, EGL_NO_SURFACE, NULL);
}

void graphics_context_t::swapBuffers() {
	// buffer swapping is implicit on device (MLGraphicsEndFrame)
}

graphics_context_t::~graphics_context_t() {
	eglDestroyContext(egl_display, egl_context);
	eglTerminate(egl_display);
}

#endif // USE_GLFW

int main() 
{
	/*
	if (glfwInit() == GLFW_FALSE)
	{
		ML_LOG(Error, "glfwInit() Failure.");
		exit(1);
	}
	//NOTE** 2022 this is present in the above precompile #if definitions
	//	Tells GLFW how to handle the window and with what version of OpenGL to use.
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef ML_OSX
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	*/
	graphics_context_t graphics;
#if GL_WINDOW and DEBUG
	glfwSetWindowSize(graphics.window, SCR_WIDTH, SCR_HEIGHT);
#endif
	/*GLFWwindow* window = glfwCreateWindow(800, 600, application_name, NULL, NULL);
	if (!window)
	{
		ML_LOG(Error, "glfwCreateWindow() Failed to create window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		ML_LOG(Error, "gladLoadGLLoader() failed");
		exit(1);
	}
		*/
	ML_LOG(Info, "Hello World!");
	auto start = std::chrono::steady_clock::now();

	// Begin painting window
	while (!glfwWindowShouldClose(graphics.window))
	{
		auto msRuntime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
		auto factor = labs(msRuntime % 2000 - 1000) / 1000.0;

		processInput(graphics.window);

		glClearColor(1.0f - factor, 0.1f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		graphics.swapBuffers();// glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

#if GL_WINDOW and DEBUG
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
#endif
