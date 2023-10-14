#include <iostream>
#include <cmath>
#include <cstring>
#include <SDL.h>

const auto Width = 600, Height = 600;
bool escaped = false;

const auto mapWidth = 24, mapHeight = 24;
int worldMap[mapWidth][mapHeight] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

enum KeyConfig
{
    P_LEFT=0,
    P_RIGHT,
    P_UP,
    P_DOWN,
    P_COUNT
};

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_CreateWindowAndRenderer(Width, Height, SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED,
                                &window, &renderer);

    double posX = 100, posY = 50;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

    uint64_t lastTick = 0;
    uint64_t currentTick = SDL_GetPerformanceCounter();
    double deltaTime = 0;

    bool keys[P_COUNT] = {};
    std::memset(keys, 0, sizeof(keys));

    constexpr double velocity = 80;
    constexpr double rotSpeed = 3;

    while (!escaped)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                escaped = true;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                printf("keysym %d -- %c\n", e.key.keysym.sym, e.key.keysym.sym);
                #define k(c, n) case c: keys[n] = e.type==SDL_KEYDOWN; break;
                switch (e.key.keysym.sym)
                {
                    k('w', P_UP) k('a', P_LEFT) k('s', P_DOWN) k('d', P_RIGHT)
                }
                #undef k
            }
        }

        if (keys[P_UP]) // forward
        {
            posX += velocity * deltaTime * dirX;
            posY += velocity * deltaTime * dirY;
        }
        if (keys[P_DOWN]) // backward
        {
            posX -= velocity * deltaTime * dirX;
            posY -= velocity * deltaTime * dirY;
        }
        auto geometricRotation = [deltaTime](double &dx, double &dy, double rot) -> void
        {
            double oldDx = dx;
            dx = dx*cos(-rot*deltaTime)
                 - dy*sin(-rot*deltaTime);
            dy = oldDx*sin(-rot*deltaTime)
                 + dy*cos(-rot*deltaTime);
        };
        if (keys[P_LEFT]) // rot left
        {
            geometricRotation(dirX, dirY, -rotSpeed);
            geometricRotation(planeX, planeY, -rotSpeed);
        }
        if (keys[P_RIGHT]) // rot right
        {
            geometricRotation(dirX, dirY, rotSpeed);
            geometricRotation(planeX, planeY, rotSpeed);
        }

        lastTick = currentTick;
        currentTick = SDL_GetPerformanceCounter();
        deltaTime = (double)((currentTick - lastTick) / (double)SDL_GetPerformanceFrequency());
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
        SDL_RenderClear(renderer);

        static constexpr int rectSz = Width / mapWidth;
        SDL_Rect tRect = {0, 0, rectSz, rectSz};
        for (auto y=0; y<mapHeight; ++y)
        {
            for (auto x=0; x<mapWidth; ++x)
            {
                tRect.x = x*rectSz; tRect.y = y*rectSz;
                if (worldMap[y][x]) SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                else                SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
                SDL_RenderFillRect(renderer, &tRect);
            }
        }

        static constexpr int playerSz = 20;
        tRect = {int(posX - playerSz/2), int(posY - playerSz/2), 20, 20};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &tRect);

        double c=0;
        for (; ; c+=.05)
        {
            int destX = int(posX+c*dirX), destY = int(posX+c*dirX);
            if (destX<0 || destX>=mapWidth || destY<0 || destY>=mapHeight) break;
            if (worldMap[destY][destX]) break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderDrawLine(renderer, posX, posY, posX+c*dirX, posY+c*dirY);


        SDL_Delay(1000.0 / 60.0);
    }

    return(0);
}
