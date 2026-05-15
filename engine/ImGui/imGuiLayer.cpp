#include "Core/Engine.h"
#include "Core/Window.h"
#include "pch.h"
#include "imGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// include your window/app headers here
// #include "Core/Application.h"

namespace Circe {

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGui") {
    }

    ImGuiLayer::~ImGuiLayer() {
    }

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport

        ImGui::StyleColorsDark();

        // Replace this with your native GLFWwindow* access:
        // GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        GLFWwindow* window = Engine::Get().GetWindow()->GetNativeWindow();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnUpdate(float deltaTime) {
        (void)deltaTime;
    }

    void ImGuiLayer::OnEvent(Event& event) {
        // Optional: block engine input when ImGui wants mouse/keyboard.
        // ImGuiIO& io = ImGui::GetIO();
        // if (io.WantCaptureMouse || io.WantCaptureKeyboard) { ... }
        (void)event;
    }

    void ImGuiLayer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}