#include "VertexArrayObject.h"
#include "Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Larry {
    VertexArrayObject::VertexArrayObject() {
        glGenVertexArrays(1, &vao);
        LA_CORE_INFO("Created new Vertex Array Object {}", vao);
    };

    VertexArrayObject::~VertexArrayObject() {
        LA_CORE_INFO("Deleted Vertex Array Object {}", vao);
        glDeleteVertexArrays(1, &vao);
    }

    void VertexArrayObject::Bind() {
        glBindVertexArray(vao);
    }

    void VertexArrayObject::Unbind() {
        glBindVertexArray(0);
    }
}
