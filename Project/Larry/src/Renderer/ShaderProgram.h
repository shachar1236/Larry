#pragma once
#include "Shader.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <span>

namespace Larry {
    class ShaderProgram {
        private:
            std::vector<Shader> shaders;
            std::unordered_map<std::string, unsigned int> uniforms;
            unsigned int shaderProgram;
            bool finished = false;
        public:
            ShaderProgram() {};
            ShaderProgram(const Shader& vertexShder, const Shader& fragmentShader, const std::span<std::string>& uniform_names);
            ~ShaderProgram() {};
            void AddShader(const Shader& shader);
            void AddUniform(const std::string& uniform_name);
            unsigned int AttachAndLink();
            void Use();
            unsigned int GetShaderProgram() {
                return shaderProgram;
            }
            void SetUniform(const std::string& name, const float& x, const float& y, const float& z, const float& w);
    };
}
