#include "TicksAndFPS.h"
#include <chrono>
#include "Game.h"
long long TicksAndFPS::getRealTimeMS()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

TicksAndFPS::TicksAndFPS(int tps) : ticksPerSecond(tps), msPerTick(1000.0 / (double)tps)
{
    lastFrameTime = getRealTimeMS();
    applicationTime = getRealTimeMS();
    currentFrameTime = getRealTimeMS();
}

void TicksAndFPS::updateFPS()
{
    /*updating FPS*/
    lastFrameTime = currentFrameTime;
    currentFrameTime = getRealTimeMS();
    deltaFrameTime = (currentFrameTime - lastFrameTime) / 1000.0;
    timer += deltaFrameTime;

    if (timer >= 1.0)
    {
        FPS = frames;
        frames = 0;
        timer -= 1.0;
    }
    frames++;
}

void TicksAndFPS::doOnTickUntillRealtimeSync(class Game* instance)
{
    if (paused)//if paused then just update timer without doing ontick
    {
        while ((applicationTime + msPerTick) < getRealTimeMS())
        {
            applicationTime += (long)msPerTick;
        }
        return;
    }
    //run ontick and update timer untill caught up with realtime
    while ((applicationTime + msPerTick) < getRealTimeMS())
    {
        instance->onTick();
        applicationTime += (long)msPerTick;
    }
    percentToNextTick = (double)(getRealTimeMS() - applicationTime) / msPerTick;
}
