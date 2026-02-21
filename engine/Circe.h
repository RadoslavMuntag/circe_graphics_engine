#pragma once

/**
 * @file Circe.h
 * @brief Main header file for the Circe Engine
 * 
 * Include this single header to get access to all Circe Engine functionality.
 * This eliminates the need to include individual subsystem headers.
 */

// ============================================================================
// Core Systems
// ============================================================================

#include "Core/Engine.h"
#include "Core/Window.h"
#include "Core/Export.h"
#include "Core/Log.h"

// ============================================================================
// Events System
// ============================================================================

#include "Events/Event.h"
#include "Events/EventDispatcher.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

// ============================================================================
// Rendering System
// ============================================================================

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"
#include "Renderer/Model.h"
#include "Renderer/Light.h"

// ============================================================================
// Scene System
// ============================================================================

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Behavior.h"

// ============================================================================
// Resources
// ============================================================================

#include "Resources/SimpleMeshGen.h"

// ============================================================================
// Utilities
// ============================================================================

#include "Math/Transform.h"
#include "Core/Time.h"

// ============================================================================
// Namespace convenience
// ============================================================================

using namespace Circe;

/**
 * Quick start example:
 * 
 * @code
 * #include <Circe.h>
 * 
 * int main() {
 *     Engine engine(800, 600, "My Game");
 *     
 *     auto scene = std::make_unique<Scene>();
 *     auto entity = std::make_unique<Entity>("MyEntity");
 *     
 *     entity->AddBehavior<MouseFollowBehavior>(0.1f);
 *     
 *     scene->AddEntity(std::move(entity));
 *     engine.SetScene(scene.get());
 *     
 *     engine.Run();
 *     return 0;
 * }
 * @endcode
 */
