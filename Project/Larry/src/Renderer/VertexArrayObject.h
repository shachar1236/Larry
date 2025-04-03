#pragma once

namespace Larry {
    class VertexArrayObject {
        private:
            unsigned int vao;

        public:
            VertexArrayObject(); // generate VertexArrayObject
            ~VertexArrayObject(); // delete VertexArrayObject

            void Bind();
            void Unbind();
    };
}
