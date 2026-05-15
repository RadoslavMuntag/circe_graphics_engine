#include "pch.h"
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
    Engine* Engine::s_Instance = nullptr;

    Engine::Engine(int width, int height, const char* title) {
        if (s_Instance) {
            Log::Critical("Attempted to create multiple instances of Engine");
            throw std::runtime_error("Engine instance already exists");
        }
        
        s_Instance = this;

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

    void Engine::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Engine::PushOverlay(Layer* overlay) {
        m_LayerStack.PushOverlay(overlay);
    }

    void Engine::SetScene(Scene* scene) {
        // TODO: Erase secene stuff from engine
        m_ActiveScene = scene;
    }

    void Engine::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent&) {
            m_Running = false;
            return true;
        });

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (event.Handled) {
                break;
            }
            (*it)->OnEvent(event);
        }


        // TODO: Erase secene stuff from engine
        if (m_ActiveScene && !event.Handled) {
            m_ActiveScene->DispatchEvent(event);
        }
    }

    void Engine::Run() {
        Time::Reset();
        
        // TODO: Erase secene stuff from engine
        if (m_ActiveScene) {
            m_ActiveScene->OnInit();
        }
        
        while (m_Running && !m_Window->ShouldClose()) {
            float deltaTime = Time::GetDeltaTime();
            Time::Update();

            m_Window->PollEvents();
            
            Update(deltaTime);
            Render(); // TODO: Move render call to a layer, Engine should not know about rendering
            
            m_Window->SwapBuffers();
        }
        
        // TODO: Erase secene stuff from engine
        if (m_ActiveScene) {
            m_ActiveScene->OnShutdown();
        }
    }

    void Engine::Update(float deltaTime) {

        // TODO: Erase secene stuff from engine
        if (m_ActiveScene) {
            m_ActiveScene->Update(deltaTime);
        }

        for (Layer* layer : m_LayerStack) {
            layer->OnUpdate(deltaTime);
        }
    }

    void Engine::Render() {
        m_Renderer->Clear();

        // TODO: Erase secene stuff from engine
        if (m_ActiveScene) {
            m_ActiveScene->Render(*m_Renderer);
        }
        
        m_Renderer->Present();
    }

}