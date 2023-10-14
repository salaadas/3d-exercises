#include <cstring>
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "gfx.hpp"
#include "game.hpp"

// What I would like to do is to create a layer
// of abstraction so that when I use my program
// I am semi-independent of the api
// so doesn't matter if I use sdl/win32api/x11/vulkan/opengl/...
// I would do the basic layer first
// and since Im doing a 3d exercise (similar to bisqwit)
// I would use opengl+sdl

// TODO: Texture --> sdl_image
// TODO: Audio   --> sdl

// TODO: camera
// TODO: player
// TODO: map editor

int main()
{
    Game game;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: could not init SDL\n");
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *window = SDL_CreateWindow("OpenGL", 0, 0, Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        fprintf(stderr, "ERROR: could not create SDL window\n");
        exit(1);
    }

    SDL_GLContext wContext = SDL_GL_CreateContext(window);
    if (wContext == nullptr) {
        fprintf(stderr, "ERROR: could not create SDL GL context: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_GL_MakeCurrent(window, wContext) < 0) {
        fprintf(stderr, "ERROR: could not create make context current: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        fprintf(stderr, "WARNING: unable to set VSync: %s\n", SDL_GetError());
        exit(1);
    }

    GLenum glewInitStatus = glewInit();
    if (glewInitStatus != GLEW_OK) {
        fprintf(stderr, "ERROR: could not init glew\n");
        exit(1);
    }

    if (!GLEW_VERSION_2_1) {
        fprintf(stderr, "ERROR: need glew to be >= 2.1\n");
        exit(1);
    }

    game.Init();

    while (!game.quit)
    {
        std::memset(game.keyboard, 0, sizeof(game.keyboard));
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                game.onClose();
            } break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                char khit=event.key.keysym.sym;
                // printf("keysym %c\n", khit);
                switch (khit)
                {
                    #define k(c) case c: game.keyboard[c] = event.type==SDL_KEYDOWN; break;
                    k('q') k('w') k('e') k('r') k('t')
                    k('a') k('s') k('d') k('f') k('g')
                    k('z') k('x') k('c') k('v') k('b')
                    #undef k
                default: break;
                }
            } break;
            default: break;
            }
        }

        game.Main();
        SDL_GL_SwapWindow(window);
    }

    game.Shutdown();

    return(0);
}
