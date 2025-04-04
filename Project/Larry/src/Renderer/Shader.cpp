#include "Shader.h"
#include "Log.h"
#include <cstdio>
#include <fstream>
#include <string>

namespace Larry {
    Shader::Shader(std::string path, enum ShaderType type_) {
        std::ifstream shader_file = std::ifstream(path.c_str());
        /* std::string str; */
        while(!shader_file.eof())
        {
            shader_text += shader_file.get();
        }

        type = type_;
    };

    Shader::~Shader() {
        this->Delete();
    };

    bool Shader::Compile() {
        if (!this->compiled) {
            shader = glCreateShader(type);

            const char* text = shader_text.c_str();
            glShaderSource(shader, 1, &text, NULL);
            glCompileShader(shader);

            int  success;
            char infoLog[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                LA_CORE_WARN("Shader compilation failed. {}", infoLog);
            } else {
                LA_CORE_INFO("Compiled shader {}", shader);
                compiled = true;
            }
        }

        return compiled;
    };

    void Shader::Delete() {
        if (compiled) {
            LA_CORE_INFO("Deleting shader {}", shader);
            glDeleteShader(shader);
            compiled = false;
        }
    }
}

