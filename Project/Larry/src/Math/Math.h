#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Larry {
    namespace Math {
        using Vec1 = glm::vec1;
        using Vec2 = glm::vec2;
        using Vec3 = glm::vec3;
        using Vec4 = glm::vec4;

        using Mat4 = glm::mat4;

        // functions
        Mat4 translate(const Mat4& a, const Vec3& b);
        Mat4 rotate(const Mat4& a, const float& b, const Vec3& c);
        Mat4 scale(const Mat4& a, const Vec3& b);
        Mat4 ortho(const float& left, const float& right, const float& bottom, const float& top, const float& nearDist, const float& farDist); 
        Mat4 perspective(const float& fov, const float& aspectRatio, const float& near, const float& far);
        const float* value_ptr(const Mat4& mat);
    }
}
