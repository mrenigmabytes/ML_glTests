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

struct graphics_context_t 
{
	GLFWwindow* window;

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

const char *vertexShaderSource = "#version 430 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 430 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
