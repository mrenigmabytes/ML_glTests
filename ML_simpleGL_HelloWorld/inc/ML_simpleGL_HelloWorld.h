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

#include <stdio.h>
#include <stdlib.h>

#pragma region WindowManager selections
#if USE_GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#else
#ifndef EGL_EGLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#endif
#pragma endregion

struct graphics_context_t {

#if USE_GLFW
	GLFWwindow* window;
#else
	EGLDisplay egl_display;
	EGLContext egl_context;
#endif

	GLuint framebuffer_id;
	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;

	graphics_context_t();
	~graphics_context_t();

	void makeCurrent();
	void swapBuffers();
	void unmakeCurrent();
};