#include "gfx.hpp"
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <GL/glew.h>
#include "utils.hpp"

GLuint compileShader(const char *shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        fprintf(stderr, "ERROR: Could not create shader\n");
        exit(1);
    }
    glShaderSource(shader, 1, &shaderSource, 0);
    glCompileShader(shader);

    GLint compileStatus; glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        GLchar buffer[1024]; int length;
        glGetShaderInfoLog(shader, sizeof(buffer), &length, buffer);
        fprintf(stderr, "ERROR: Could not compile shader because of:\n%s\n", buffer);
        exit(1);
    }
    printf("Compiled shader successfully\n");
    return shader;
}


GLuint loadShaderFromFile(std::string fp, GLenum shaderType)
{
    std::ifstream fs(fp, std::ifstream::in);
    std::string res("");
    for (std::string line; fs.good(); ) {
        std::getline(fs, line);
        res.insert(res.length(), line);
        res.insert(res.length(), "\n");
    }
    fs.close();
    printf("Load shader from %s successfully\n", fp.c_str());
    return compileShader(res.c_str(), shaderType);
}

GLuint linkProgram(GLuint &vertShader, GLuint &fragShader)
{
    GLuint program = glCreateProgram();
    if (!program) {
        fprintf(stderr, "ERROR: Could not create shader program\n");
        exit(1);
    }
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        GLchar buffer[1024]; int length;
        glGetProgramInfoLog(program, sizeof(buffer), &length, buffer);
        fprintf(stderr, "ERROR: Could not link shader program because of:\n%s\n", buffer);
        exit(1);
    }
    printf("Link program successfully\n");
    return program;
}

GLuint createProgram(const char *vertPath, const char *fragPath)
{
    GLuint vs = loadShaderFromFile(vertPath, GL_VERTEX_SHADER);
    GLuint fs = loadShaderFromFile(fragPath, GL_FRAGMENT_SHADER);
    printf("Create program successfully\n");
    return linkProgram(vs, fs);
}

void drawLine(vec3 p0, vec3 p1, GLuint shaderProgram, u32 color)
{
    normalCoordToOpenGL(p0);
    normalCoordToOpenGL(p1);
    const GLfloat data[] = {
        p0.x, p0.y, p0.z,
        p1.x, p1.y, p1.z,
    };
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers     (1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0,        // attribute 0, just the id
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        0,        // stride
        (void*)0  // offset
    );
    glEnableVertexAttribArray(0);
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), unpackRGB(color));
    glDrawArrays(GL_LINES, 0, 2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawQuad(vec3 p0, vec3 p1, vec3 p2, vec3 p3, GLuint shaderProgram, u32 color)
{
    normalCoordToOpenGL(p0);
    normalCoordToOpenGL(p1);
    normalCoordToOpenGL(p2);
    normalCoordToOpenGL(p3);
    const GLfloat data[] = {
        p0.x, p0.y, p0.z,
        p1.x, p1.y, p1.z,
        p2.x, p2.y, p2.z,
        p3.x, p3.y, p3.z,
    };
    const GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers     (1, &vbo);
    glGenBuffers     (1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0,        // attribute 0, just the id
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        0,        // stride
        (void*)0  // offset
    );
    glEnableVertexAttribArray(0);
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), unpackRGB(color));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
