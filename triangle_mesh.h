#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include <vector>
#include <array> // Fix 1: Include <array>
#include <fstream>
#include <sstream>
#include "triangle.h"
#include "hittable_list.h"

class triangle_mesh : public hittable {
  public:
    triangle_mesh(const std::string& filename, shared_ptr<material> mat, const point3& center = point3(0, 0, 0))
        : mat(mat), center(center)
    {
        load_mesh(filename);
        set_bounding_box();
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        return triangles.hit(r, ray_t, rec); // Delegate to hittable_list
    }

    aabb bounding_box() const override {
        return bbox;
    }

  private:
    hittable_list triangles; // Collection of all triangles in the mesh
    shared_ptr<material> mat;
    aabb bbox;
    point3 center;

    void load_mesh(const std::string& filename) {
    std::vector<point3> vertices;
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error: Cannot open file " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") { // Vertex definition
            double x, y, z;
            iss >> x >> y >> z;

            // Apply the center offset
            vertices.emplace_back(x + center.x(), y + center.y(), z + center.z());
        } else if (prefix == "f") { // Face definition
            std::array<int, 3> indices;

            for (int i = 0; i < 3; ++i) {
                std::string vertex_str;
                iss >> vertex_str;

                std::istringstream vertex_ss(vertex_str);
                int vertex_index;
                vertex_ss >> vertex_index;
                vertex_index--; // OBJ files are 1-indexed
                indices[i] = vertex_index;
            }

            // Validate and create the triangle
            const auto& v0 = vertices[indices[0]];
            const auto& v1 = vertices[indices[1]];
            const auto& v2 = vertices[indices[2]];

            triangles.add(make_shared<triangle>(v0, v1 - v0, v2 - v0, mat));
        }
    }
}

    void set_bounding_box() {
        bbox = triangles.bounding_box(); // Fix 3: Use hittable_list's bounding_box method
    }
};

#endif
