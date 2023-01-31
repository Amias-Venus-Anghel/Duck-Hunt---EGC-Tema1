#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object2D::CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length / 2, height, 0), color),
        VertexFormat(corner + glm::vec3(length , 0, 0), color),
    };

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2 };

	triangle->SetDrawMode(GL_TRIANGLES);

    triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* object2D::CreateWing(std::string name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color)
{
	/* more complex wing than a simple triagle */
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length/2, height, 0), color),
		VertexFormat(corner + glm::vec3(length , 0, 0), color),
		VertexFormat(corner + glm::vec3(length/8, -height/4, 0), color),
		VertexFormat(corner + glm::vec3(length/4, 0, 0), color),
		VertexFormat(corner + glm::vec3(3 * length/8, -height/4, 0), color),
		VertexFormat(corner + glm::vec3(length/2, 0, 0), color),
		VertexFormat(corner + glm::vec3(5 * length/8, -height/4, 0), color),
		VertexFormat(corner + glm::vec3(3 * length/4, 0, 0), color),
		VertexFormat(corner + glm::vec3(7 * length/8, -height/4, 0), color),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2,
											0, 3, 4,
											4, 5, 6,
											6, 7, 8,
											8, 9, 2};

	triangle->SetDrawMode(GL_TRIANGLES);

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* object2D::CreateCircle(std::string name, float radius, glm::vec3 color)
{
	const float PI = 3.1415926f;
	float n = 2 * PI / 100;

	float currAngle;
	std::vector<VertexFormat> vertices = {};
	/* add the center of the circle */
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
	/* iterate thro the next point of the circle */
	for (currAngle = 0; currAngle < 2 * PI; currAngle += n)
	{
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(currAngle), radius * sin(currAngle), 0), color));
	}
	vertices.push_back(VertexFormat(glm::vec3(radius * cos(2 * PI), radius * sin(2 * PI), 0), color));

	Mesh* circle = new Mesh(name);
	std::vector<unsigned int> indices = {};
	for (int i = 1; i <= vertices.size(); i += 1) {
		indices.push_back(i);
		indices.push_back(0);
		indices.push_back(i + 1);
	}


	circle->SetDrawMode(GL_TRIANGLES);
	circle->InitFromData(vertices, indices);
	return circle;
}

Mesh* object2D::CreateHeart(std::string name, glm::vec3 baseCorner, float height, glm::vec3 color)
{
	glm::vec3 corner = baseCorner;
	float high = height;
	float low = 2 * height / 3;
	float len = height;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(len/2, low, 0), color),
		VertexFormat(corner + glm::vec3(len/4, high, 0), color),
		VertexFormat(corner + glm::vec3(0, low, 0), color),
		VertexFormat(corner + glm::vec3(-len/4, high, 0), color),
		VertexFormat(corner + glm::vec3(-len/2, low, 0), color),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 
											0, 2, 3,
											0, 3, 4,
											0, 4, 5
	};

	triangle->SetDrawMode(GL_TRIANGLES);

	triangle->InitFromData(vertices, indices);
	return triangle;
}


