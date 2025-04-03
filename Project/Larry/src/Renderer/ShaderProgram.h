#pragma once
#include "Shader.h"
#include <vector>

namespace Larry {
    class ShaderProgram {
        private:
            std::vector<Shader> shaders;
            unsigned int shaderProgram;
            bool finished = false;
        public:
            ShaderProgram() {};
            ShaderProgram(const Shader& vertexShder, const Shader& fragmentShader);
            ~ShaderProgram() {};
            void AddShader(const Shader& shader);
            unsigned int AttachAndLink();
            void Use();
            unsigned int GetShaderProgram() {
                return shaderProgram;
            }
    };
}
