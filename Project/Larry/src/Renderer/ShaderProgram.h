#pragma once
#include "Shader.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Math.h"

namespace Larry {
    class ShaderProgram {
        private:
            std::vector<Shader> shaders;
            std::unordered_map<std::string, unsigned int> uniforms;
            unsigned int shaderProgram;
            bool finished = false;
        public:
            ShaderProgram() {};
            ShaderProgram(const Shader& vertexShder, const Shader& fragmentShader);
            ~ShaderProgram() {};
            void AddShader(const Shader& shader);
            unsigned int AddUniform(const std::string& uniform_name);
            unsigned int AttachAndLink();
            void Use();
            int GetUniform(const std::string& name);
            void SetUniform(const std::string& name, const float& x, const float& y, const float& z, const float& w);
            void SetUniform(const std::string& name, const int& number);
            void SetUniform(const std::string& name, const int* arr, const int& arr_size);
            void SetUniform(const std::string& name, const Math::Mat4& mat);
    };
}
