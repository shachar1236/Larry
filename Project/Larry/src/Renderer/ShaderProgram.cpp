#include "ShaderProgram.h"

namespace Larry {
    ShaderProgram::ShaderProgram(const Shader& vertexShder, const Shader& fragmentShader) {
        shaders.reserve(2);
        shaders.push_back(vertexShder);
        shaders.push_back(fragmentShader);
    }

    void ShaderProgram::AddShader(const Shader& shader) {
        shaders.push_back(shader);
    }

    unsigned int ShaderProgram::AttachAndLink() {
        if (!finished) {
            finished = true;
            shaderProgram = glCreateProgram();
            for (auto shader : shaders) {
                shader.Compile();
                glAttachShader(shaderProgram, shader.GetShader());
            }
            glLinkProgram(shaderProgram);
            for (auto shader : shaders) {
                shader.Delete();
            }
        }
        return shaderProgram;
    };

    void ShaderProgram::Use() {
        if (finished) {
            glUseProgram(shaderProgram);
        }
    }
}
