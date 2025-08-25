#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include "random.h"

class vec3 {
    public:
        vec3() : v{0,0,0} {};
        vec3(double e0, double e1, double e2) : v{e0,e1,e2} {};

        double x() const { return v[0]; }
        double y() const { return v[1]; }
        double z() const { return v[2]; }

        vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
        double operator[](int i) const { return v[i]; }
        double& operator[](int i) { return v[i]; }

        vec3& operator+=(const vec3& v2) {
            v[0] += v2.x();
            v[1] += v2.y();
            v[2] += v2.z();
            return *this;
        }

        vec3& operator*=(double t) {
            v[0] *= t;
            v[1] *= t;
            v[2] *= t;
            return *this;
        }

        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
        }


    private:
        double v[3];

};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}


inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}


inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}


inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.x(), t*v.y(), t*v.z());
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.x() * v.x()
         + u.y() * v.y()
         + u.z() * v.z();
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y() * v.z() - u.z() * v.y(),
                u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 rand_vec(double min, double max) {
    return vec3(Random::getRand(min, max), Random::getRand(min, max), Random::getRand(min, max));
}

inline vec3 rand_unit_vec() {
    return unit_vector(rand_vec(-1,1));
}

inline vec3 vec_on_surface(const vec3& normal) {
    vec3 vec = rand_unit_vec();
    if (dot(vec, normal) > 0.0) {
        return vec;
    } else {
        return -vec;
    }

}


using pointV = vec3;
//using colorV = vec3;

#endif