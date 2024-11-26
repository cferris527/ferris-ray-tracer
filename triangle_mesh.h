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
    triangle_mesh(const std::string& filename, shared_ptr<material> mat)
        : mat(mat)
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
                vertices.emplace_back(x, y, z);
            } else if (prefix == "f") { // Face definition
                std::array<int, 3> indices; // Fix 1: Ensure <array> is included

                for (int i = 0; i < 3; ++i) {
                    std::string vertex_str;
                    iss >> vertex_str;

                    std::istringstream vertex_ss(vertex_str);
                    int vertex_index;

                    // Parse vertex index (ignoring texture/normal indices)
                    vertex_ss >> vertex_index;
                    vertex_index--; // OBJ files are 1-indexed
                    indices[i] = vertex_index;
                }

                // Validate indices
                if (indices[0] >= vertices.size() || indices[1] >= vertices.size() || indices[2] >= vertices.size()) {
                    throw std::runtime_error("Error: Face index out of bounds in file " + filename);
                }

                // Create triangle from vertices
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
