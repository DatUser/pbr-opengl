#pragma once

#include <vector>
#include "vector3.hh"

class Cube
{
public:
    Cube(Point3& center, float size);

    //Returns vertices and indices
    void generate();

    std::vector<float>& get_vertices() { return vertices; }

    ~Cube() = default;
private:
    Point3 center;
    float size;

    //positions-normals-color
    std::vector<float> vertices;
};
