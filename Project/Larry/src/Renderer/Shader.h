#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
        bool compiled = false;
    public:
        Shader(std::string path, enum ShaderType type);
        ~Shader();

        bool Compile(); // returns if compiling succeded
        void Delete();

        unsigned int GetShader() { return shader; };
    };
}
