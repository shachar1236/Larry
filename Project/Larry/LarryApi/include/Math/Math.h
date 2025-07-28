#pragma once

#include <yaml-cpp/yaml.h>
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

        Mat4 lookAt(const Vec3& position, const Vec3& target, const Vec3& up);
    }
}

namespace YAML {

    template<>
    struct convert<Larry::Math::Vec1> {
        static Node encode(const Larry::Math::Vec1& rhs) {
            Node node;
            node.push_back(rhs.x);
            return node;
        }

        static bool decode(const Node& node, Larry::Math::Vec1& rhs) {
            if(!node.IsSequence() || node.size() != 1) {
                return false;
            }

            rhs.x = node[0].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Larry::Math::Vec2> {
        static Node encode(const Larry::Math::Vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, Larry::Math::Vec2& rhs) {
            if(!node.IsSequence() || node.size() != 2) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Larry::Math::Vec3> {
        static Node encode(const Larry::Math::Vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, Larry::Math::Vec3& rhs) {
            if(!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Larry::Math::Vec4> {
        static Node encode(const Larry::Math::Vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, Larry::Math::Vec4& rhs) {
            if(!node.IsSequence() || node.size() != 4) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Larry::Math::Mat4> {
        static Node encode(const Larry::Math::Mat4& rhs) {
            Node node;
            node.push_back(rhs[0]);
            node.push_back(rhs[1]);
            node.push_back(rhs[2]);
            node.push_back(rhs[3]);
            return node;
        }

        static bool decode(const Node& node, Larry::Math::Mat4& rhs) {
            if(!node.IsSequence() || node.size() != 4) {
                return false;
            }

            rhs[0] = node[0].as<Larry::Math::Vec4>();
            rhs[1] = node[1].as<Larry::Math::Vec4>();
            rhs[2] = node[2].as<Larry::Math::Vec4>();
            rhs[3] = node[3].as<Larry::Math::Vec4>();
            return true;
        }
    };
}
