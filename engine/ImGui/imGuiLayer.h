#pragma once

#include "../Core/Layer.h"


namespace Circe {
    
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnEvent(Event& event) override;

    private:
        void Begin();
        void End();
    };

}