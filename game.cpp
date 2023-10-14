#include "game.hpp"
#include <cstdio>
#include <cstring>

#include <SDL.h>
#include <GL/glew.h>
#include "glm/vec3.hpp"
#include "gfx.hpp"

Game::Game()
: quit(false), deltaTime(0), lastTick(0)
{
    std::memset(keyboard, 0, sizeof(keyboard));
    currentTick = SDL_GetPerformanceCounter();
}
void Game::Init()
{
    programID = createProgram("shaders/simple.vert", "shaders/simple.frag");
}
void Game::Main()
{
    handleKey();
    Update();
    Render();

    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentTick - lastTick) / (double)SDL_GetPerformanceFrequency() * 1000);
    // printf("%f\n", deltaTime);
}
void Game::Shutdown()
{
    glDeleteProgram(programID);
}
void Game::Update()
{
}
void Game::Render()
{
    glClearColor(unpackRGBA(0x181818FF));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawQuad(vec3(0,0,1), vec3(Width/2,0,1), vec3(Width/2,Height/2,0), vec3(0,Height/2,0), programID);
}
void Game::onClose()
{
    quit = true;
}
void Game::handleKey()
{
    if (keyboard['w'])
    {
        printf("move up\n");
    }
    if (keyboard['q'])
    {
        quit = true;
    }
}
