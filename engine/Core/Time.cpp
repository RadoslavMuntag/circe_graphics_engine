#include "Time.h"
#include <GLFW/glfw3.h>

namespace Circe {

    float Time::s_DeltaTime = 0.0f;
    float Time::s_Time = 0.0f;
    float Time::s_LastFrame = 0.0f;
    int Time::s_FPS = 0;
    float Time::s_FPSTimer = 0.0f;
    int Time::s_FrameCount = 0;

    void Time::Reset() {
        s_Time = static_cast<float>(glfwGetTime());
        s_LastFrame = s_Time;
        s_DeltaTime = 0.0f;
        s_FPS = 0;
        s_FPSTimer = 0.0f;
        s_FrameCount = 0;
    }

    void Time::Update() {
        float currentFrame = static_cast<float>(glfwGetTime());
        s_DeltaTime = currentFrame - s_LastFrame;
        s_LastFrame = currentFrame;
        s_Time = currentFrame;

        // FPS calculation
        s_FrameCount++;
        s_FPSTimer += s_DeltaTime;
        if (s_FPSTimer >= 1.0f) {
            s_FPS = s_FrameCount;
            s_FrameCount = 0;
            s_FPSTimer = 0.0f;
        }
    }

}