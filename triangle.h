#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

class triangle : public hittable {
  public:
    triangle(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
      : Q(Q), u(u), v(v), mat(mat)
    {
        // Compute the normal vector.
        normal = unit_vector(cross(u, v));
        D = dot(normal, Q);
        w = cross(u, v) / dot(cross(u, v), cross(u, v)); // Used for barycentric coordinates.

        set_bounding_box();
    }

    virtual void set_bounding_box() {
        // Compute the bounding box of the triangle's vertices.
        auto min_point = point3(
            std::fmin(Q.x(), std::fmin((Q + u).x(), (Q + v).x())),
            std::fmin(Q.y(), std::fmin((Q + u).y(), (Q + v).y())),
            std::fmin(Q.z(), std::fmin((Q + u).z(), (Q + v).z()))
        );
        auto max_point = point3(
            std::fmax(Q.x(), std::fmax((Q + u).x(), (Q + v).x())),
            std::fmax(Q.y(), std::fmax((Q + u).y(), (Q + v).y())),
            std::fmax(Q.z(), std::fmax((Q + u).z(), (Q + v).z()))
        );
        bbox = aabb(min_point, max_point);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denom) < 1e-8)
            return false;

        // Calculate the intersection point's t value.
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        // Calculate barycentric coordinates to test if the point is inside the triangle.
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;

        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));
        auto gamma = 1.0 - alpha - beta;

        if (!is_interior(alpha, beta, gamma, rec))
            return false;

        // Ray hits the triangle; set hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b, double g, hit_record& rec) const {
        // Check if barycentric coordinates are inside the triangle.
        if (a < 0 || b < 0 || g < 0)
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

  private:
    point3 Q;             // One vertex of the triangle.
    vec3 u, v;            // Edges of the triangle: Q+u, Q+v.
    vec3 w;               // Precomputed value for barycentric coordinates.
    shared_ptr<material> mat;
    aabb bbox;            // Bounding box of the triangle.
    vec3 normal;          // Surface normal of the triangle.
    double D;             // Plane constant for the triangle.
};

#endif