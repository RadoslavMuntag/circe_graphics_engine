#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"

namespace Circe {

    Model::Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
        : m_Mesh(mesh), m_Material(material) {
    }

    Model::~Model() {
    }

    glm::mat4 Model::GetModelMatrix() const {
        return m_Transform.GetModelMatrix();
    }

    void Model::Render(Renderer& renderer, const glm::mat4& parentMatrix) {
        if (!m_Mesh || !m_Material) {
            return;
        }

        glm::mat4 finalMatrix = parentMatrix * GetModelMatrix();
        renderer.SubmitMesh(m_Mesh, m_Material, finalMatrix);
    }

}
