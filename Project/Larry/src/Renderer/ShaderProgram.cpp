#include "ShaderProgram.h"
#include "BufferObject.h"

namespace Larry {
    ShaderProgram::ShaderProgram(const Shader& vertexShder, const Shader& fragmentShader, const std::span<std::string>& uniform_names)  {
        shaders.reserve(2);
        shaders.push_back(vertexShder);
        shaders.push_back(fragmentShader);

        for (auto uniform_name : uniform_names) {
            AddUniform(uniform_name);
        }
    }

    void ShaderProgram::AddShader(const Shader& shader) {
        shaders.push_back(shader);
    }

    void ShaderProgram::AddUniform(const std::string& uniform_name) {
        this->uniforms[uniform_name] = glGetUniformLocation(shaderProgram, uniform_name.c_str());
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

    void ShaderProgram::SetUniform(const std::string& name, const float& x, const float& y, const float& z, const float& w) {
        glUniform4f(uniforms[name], x, y, z, w);
    }
}




