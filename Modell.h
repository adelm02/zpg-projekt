#ifndef ZPGPROJ_MODELL_H
#define ZPGPROJ_MODELL_H

#include <GL/glew.h>

class Modell {
private:
    GLuint vao;
    GLuint vbo;
    GLsizei vertexCount;

public:
    Modell();

    void loadData(const float* data, int dataSize, int stride);
    void draw() const;
};

#endif