#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Larry {
    enum ShaderType {
        ShaderVertex = GL_VERTEX_SHADER,
        ShaderFragment = GL_FRAGMENT_SHADER,
    };

    class Shader {
    private:
        std::string shader_text;
        unsigned int shader;
        enum ShaderType type;
        std::unordered_map<std::string, std::string> compile_type_definations;
        bool compiled = false;
    public:
        Shader(const std::string& path, const enum ShaderType& type, const std::unordered_map<std::string, std::string>& compile_type_definations_);
        ~Shader();

        bool Compile(); // returns if compiling succeded
        void Delete();

        unsigned int GetShader() { return shader; };
    };
}
