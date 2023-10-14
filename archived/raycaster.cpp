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

    double posX = 2, posY = 2;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

    uint64_t lastTick = 0;
    uint64_t currentTick = SDL_GetPerformanceCounter();
    double deltaTime = 0;

    bool keys[16] = {};
    std::memset(keys, 0, sizeof(keys));

    constexpr double velocity = 4;
    constexpr double rotSpeed = 4;

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


        for (int x = 0; x < Width; ++x)
        {
            double cameraX = 2 * x / double(Width) - 1;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            // current tile of the ray
            int mapX = int(posX);
            int mapY = int(posY);

            // distance from the initial position to the side of the tile
            double sideDistX;
            double sideDistY;

            // distance from 1 x-side to next x-side,
            // or from 1 y-side to next y-side
            double deltaDistX = sqrt(1 + (rayDirY*rayDirY) / (rayDirX*rayDirX));
            double deltaDistY = sqrt(1 + (rayDirX*rayDirX) / (rayDirY*rayDirY));

            // will be used to calculate the length of the raycaster
            double perpWallDist;

            int stepX, stepY;
            bool hit = false;
            int side; // 0 for x-hit and 1 for y-hit

            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }

            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }

            // DDA
            while (!hit)
            {
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (worldMap[mapX][mapY] > 0) hit = true;
            }

            if (side == 0) perpWallDist = sideDistX - deltaDistX;
            else           perpWallDist = sideDistY - deltaDistY;


            int lineHeight                  = int(Height/perpWallDist);
            int drawStart                   = Height/2 - lineHeight/2;
            int drawEnd                     = Height/2 + lineHeight/2;
            if (drawStart <= 0) drawStart   = 0;
            if (drawEnd >= Height) drawEnd  = Height-1;

            SDL_Color color;
            switch (worldMap[mapX][mapY])
            {
            case 1:
                color = {255, 0, 0, 255};
                break;
            case 2:
                color = {0, 255, 0, 255};
                break;
            case 3:
                color = {0, 0, 255, 255};
                break;
            case 4:
                color = {255, 255, 255, 255};
                break;
            case 5:
                color = {255, 255, 0, 255};
                break;
            }

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
        }

        SDL_Delay(1000.0 / 60.0);
    }

    return(0);
}
