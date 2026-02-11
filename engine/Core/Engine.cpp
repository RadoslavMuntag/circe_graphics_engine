#include "Engine.h"
#include "Window.h"
#include "Time.h"
#include "../Renderer/Renderer.h"
#include "../Scene/Scene.h"
#include "Logging/ErrorReporting.h"

namespace Circe {

    Engine::Engine(int width, int height, const char* title) {
        m_Window = std::make_unique<Window>(width, height, title);
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
    }

    void Engine::Shutdown() {
        m_Running = false;
    }

    void Engine::SetScene(Scene* scene) {
        m_ActiveScene = scene;
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