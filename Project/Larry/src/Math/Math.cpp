#include "Math.h"

namespace Larry {
    namespace Math {
        Mat4 translate(const Mat4& a, const Vec3& b) {
            return glm::translate(a, b);
        }
        Mat4 rotate(const Mat4& a, const float& b, const Vec3& c) {
            return glm::rotate(a, b, c);
        }
        Mat4 scale(const Mat4& a, const Vec3& b) {
            return glm::scale(a, b);
        }
        Mat4 ortho(const float& left, const float& right, const float& bottom, const float& top, const float& nearDist, const float& farDist) {
            return glm::ortho(left, right, bottom, top, nearDist, farDist);
        }
        Mat4 perspective(const float& fov, const float& aspectRatio, const float& near, const float& far) {
            return glm::perspective(fov, aspectRatio, near, far);
        }
        const float* value_ptr(const Mat4& mat) {
            return glm::value_ptr(mat);
        }
    }
}
