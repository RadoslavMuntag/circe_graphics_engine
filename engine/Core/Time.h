#pragma once

namespace Circe {

    class Time {
    public:
        static void Reset();
        static void Update();
        
        static float GetDeltaTime() { return s_DeltaTime; }
        static float GetTime() { return s_Time; }
        static int GetFPS() { return s_FPS; }

    private:
        static float s_DeltaTime;
        static float s_Time;
        static float s_LastFrame;
        static int s_FPS;
        static float s_FPSTimer;
        static int s_FrameCount;
    };

}