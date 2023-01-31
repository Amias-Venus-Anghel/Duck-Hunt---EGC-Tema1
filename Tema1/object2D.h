#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	Mesh* CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color);

	Mesh* CreateWing(std::string name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color);
	
	Mesh* CreateCircle(std::string name, float radius, glm::vec3 color);

	Mesh* CreateHeart(std::string name, glm::vec3 baseCorner, float height, glm::vec3 color);

}
