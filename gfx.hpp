#pragma once
#include <algorithm>
#include <string>
#include <GL/glew.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "utils.hpp"

using namespace glm;

inline void normalCoordToOpenGL(vec2 &v)
{
    v.x = 2*(v.x/double(Width))-1;
    v.y = 2*(v.y/double(Height))-1;
}
inline void normalCoordToOpenGL(vec3 &v)
{
    v.x = 2*(v.x/double(Width)) -1;
    v.y = 2*(v.y/double(Height))-1;
    v.z = std::clamp(v.z, -1.0f, 1.0f);
}
GLuint compileShader(const char *shaderSource, GLenum shaderType);
GLuint loadShaderFromFile(std::string fp, GLenum shaderType);
GLuint linkProgram(GLuint &vertShader, GLuint &fragShader);
GLuint createProgram(const char *vertPath, const char *fragPath);
void drawLine(vec3 p0, vec3 p1, GLuint shaderProgram, u32 color=0xFFFFFF);
void drawQuad(vec3 p0, vec3 p1, vec3 p2, vec3 p3, GLuint shaderProgram, u32 color=0xFFFFFF);
