#include "Engine.h"
#include "Window.h"
#include "Log.h"
#include "Time.h"
#include "../Renderer/Renderer.h"
#include "../Scene/Scene.h"
#include "Logging/ErrorReporting.h"
#include "../Events/EventDispatcher.h"
#include "../Events/ApplicationEvent.h"

namespace Circe {

    Engine::Engine(int width, int height, const char* title) {
        Log::Init();
        Log::Info("Circe Engine initializing...");
        m_Window = std::make_unique<Window>(width, height, title);
        m_Window->SetEventCallback([this](Event& event) {
            OnEvent(event);
        });
        m_Renderer = std::make_unique<Renderer>();
        Initialize();
    }
    
    Engine::~Engine() {
        Shutdown();
    }
    
    void Engine::Initialize() { 
        m_Renderer->Initialize();
        m_Running = true;
        enableReportGlErrors();
        
        Log::Info("Circe Engine initialized successfully");
    }

    void Engine::Shutdown() {
        Log::Info("Shutting down Circe Engine");
        m_Running = false;
        Log::Shutdown();
    }

    void Engine::SetScene(Scene* scene) {
        m_ActiveScene = scene;
    }

    void Engine::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent&) {
            m_Running = false;
            return true;
        });

        if (m_ActiveScene && !event.Handled) {
            m_ActiveScene->DispatchEvent(event);
        }
    }

    void Engine::Run() {
        Time::Reset();
        
        if (m_ActiveScene) {
            m_ActiveScene->OnInit();
        }
        
        while (m_Running && !m_Window->ShouldClose()) {
            float deltaTime = Time::GetDeltaTime();
            Time::Update();

            m_Window->PollEvents();
            
            Update(deltaTime);
            Render();
            
            m_Window->SwapBuffers();
        }
        
        if (m_ActiveScene) {
            m_ActiveScene->OnShutdown();
        }
    }

    void Engine::Update(float deltaTime) {
        if (m_ActiveScene) {
            m_ActiveScene->Update(deltaTime);
        }
    }

    void Engine::Render() {
        m_Renderer->Clear();
        
        if (m_ActiveScene) {
            m_ActiveScene->Render(*m_Renderer);
        }
        
        m_Renderer->Present();
    }

}