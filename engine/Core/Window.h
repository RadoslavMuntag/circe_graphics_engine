#pragma once

#include "Export.h"
#include <functional>

struct GLFWwindow;

namespace Circe {

    class Event;

    class CIRCE_API Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(int width, int height, const char* title);
        ~Window();

        void PollEvents();
        void SwapBuffers();
        bool ShouldClose() const;

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        GLFWwindow* GetNativeWindow() const { return m_Window; }

        void SetVSync(bool enabled);
        void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }

    private:
        GLFWwindow* m_Window;
        int m_Width;
        int m_Height;
        EventCallbackFn m_EventCallback;
    };

}