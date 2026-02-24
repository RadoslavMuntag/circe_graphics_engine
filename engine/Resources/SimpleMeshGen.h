#pragma once 

#include <memory>

namespace Circe {
    class Mesh;

    std::shared_ptr<Mesh> GenerateQuadMesh(float width, float height);
    std::shared_ptr<Mesh> GenerateSphereMesh(float radius, unsigned int sectorCount, unsigned int stackCount);
    std::shared_ptr<Mesh> GenerateCubeMesh(float size);
}