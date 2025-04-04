#pragma once

#include "Log.h"
#include <span>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Larry {
    enum DrawType {
        STATIC_DRAW = GL_STATIC_DRAW,
        STREAM_DRAW = GL_STREAM_DRAW,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    };

    template<class T>
    class BufferObject {
        private:
            int buffer_type;
            enum DrawType draw_type;
            unsigned int vbo;
            bool generated = false;
        public:
            BufferObject() {};
            BufferObject(const int& buffer_type_, enum DrawType draw_type_) :
                draw_type(draw_type_),
                buffer_type(buffer_type_)
            {}
            ~BufferObject() {
                this->Delete();
            }

            unsigned int GetVbo() { return vbo; };
            bool Generated() { return generated; };
            
            // binds the vbo
            void Bind() {
                glBindBuffer(buffer_type, vbo);  
            }
            
            // generates the buffer and returns the VBO
            unsigned int Generate(const T* data, const unsigned int& data_size) {
                if (!generated) {
                    generated = true;
                    // generate the buffer
                    glGenBuffers(1, &vbo);  

                    this->Bind();
                    // copy the data to the buffer
                    glBufferData(buffer_type, data_size, data, draw_type);

                    LA_CORE_INFO("Created BufferObject {}", vbo);
                }
                return vbo;
            }

            // set attribute on GPU, needs to be bind before callind
            void SetAttributeOnVBO(const GLuint& index, const GLint& size, const GLenum& type, const GLboolean& normalized, const GLsizei& stride, const void *pointer) {
                glVertexAttribPointer(index, size, type, normalized, stride, pointer);
                glEnableVertexAttribArray(index);  
            }
            
            // Deletes the vbo
            void Delete() {
                if (generated) {
                    generated = false;
                    LA_CORE_INFO("Deleted BufferObject {}", vbo);
                    glDeleteBuffers(1, &vbo);
                }
            }
            
            static void DeleteBufferArray(std::span<BufferObject<T>> objects) {
                std::vector<unsigned int> vec;
                vec.reserve(objects.size());
                for (auto vbo : objects) {
                    vec.push_back(vbo.GetVbo());
                    vbo.generated = false;
                }

                glDeleteBuffers(vec.size(), vec.data());
                LA_CORE_INFO("Deleted array of BufferObject");
            }

            void ChangeData(const unsigned int& offset,  const T* data, const unsigned int& data_size) {
                this->Bind();
                glBufferSubData(buffer_type, offset, data_size, data);
            }
    };

}


