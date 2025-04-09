#include "Shader.h"
#include "ErrorEvents.h"
#include "EventSystem.h"
#include "LarryMemory.h"
#include "Log.h"
#include <cstdio>
#include <fstream>
#include <regex>
#include <string>

namespace Larry {
    Shader::Shader(const std::string& path, const enum ShaderType& type_, const std::unordered_map<std::string, std::string>& compile_type_definations_) {
        std::ifstream shader_file = std::ifstream(path.c_str());
        /* std::string str; */
        while(!shader_file.eof())
        {
            shader_text += shader_file.get();
        }

        type = type_;
        compile_type_definations = compile_type_definations_;
    };

    Shader::~Shader() {
        this->Delete();
    };

    bool Shader::Compile() {
        if (!this->compiled) {
            // replace definations
            for (auto key_value : compile_type_definations) {
                shader_text = std::regex_replace(shader_text, std::regex(key_value.first), key_value.second);
            }
            // compile
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
                Ref<Events::ShaderCompilationFailedErrorEvent> err = CreateRef<Events::ShaderCompilationFailedErrorEvent>(infoLog, shader_text);
                EventSystem::HandleEvent(err);
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

