#include "ShaderProgram.h"
#include "BufferObject.h"
#include "Log.h"
#include "Shader.h"

namespace Larry {
    ShaderProgram::ShaderProgram(const Shader& vertexShder, const Shader& fragmentShader)  {
        shaders.reserve(2);
        shaders.push_back(vertexShder);
        shaders.push_back(fragmentShader);

    }

    void ShaderProgram::AddShader(const Shader& shader) {
        shaders.push_back(shader);
    }

    unsigned int ShaderProgram::AddUniform(const std::string& uniform_name) {
        auto uniform = uniforms.find(uniform_name);
        if (uniform == uniforms.end()) {
             unsigned int res = glGetUniformLocation(shaderProgram, uniform_name.c_str());
             this->uniforms[uniform_name] = res;
             return res;
        } else {
            return uniform->second;
        }
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
        glUniform4f(AddUniform(name), x, y, z, w);
    }

    void ShaderProgram::SetUniform(const std::string& name, const int& number) {
        glUniform1i(AddUniform(name), number);
    }

    void ShaderProgram::SetUniform(const std::string& name, const int* arr, const int& arr_size) {
        glUniform1iv(AddUniform(name), arr_size, arr);
    }

}




