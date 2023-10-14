#include <GL/glew.h>
#include "utils.hpp"

struct Game
{
    bool keyboard[128];
    bool quit;
    double deltaTime;
    u64 lastTick;
    u64 currentTick;
    // for now, this is enough
    GLuint programID;

    Game();
    void Init();
    void Main();
    void Shutdown();
    void Update();
    void Render();
    void onClose();
    void handleKey();
};
