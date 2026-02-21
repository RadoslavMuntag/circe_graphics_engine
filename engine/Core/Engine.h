#include <memory>
#include "Export.h"

namespace Circe {
    class Event;
    class Window;
    class Renderer;
    class Scene;

    class CIRCE_API Engine {
    public:
        Engine(int width, int height, const char* title);
        ~Engine();

        // Main loop - call this from main()
        void Run();
        
        // Access to subsystems
        Window* GetWindow() const { return m_Window.get(); }
        Renderer* GetRenderer() const { return m_Renderer.get(); }
        
        // Game sets the active scene
        void SetScene(Scene* scene);
        void OnEvent(Event& event);

    private:
        void Initialize();
        void Shutdown();
        void Update(float deltaTime);
        void Render();

        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Renderer> m_Renderer;
        Scene* m_ActiveScene = nullptr;
        
        bool m_Running = false;
    };
}