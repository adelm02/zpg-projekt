#ifndef ZPGPROJ_MODELL_H
#define ZPGPROJ_MODELL_H


#include <string>
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
    bool loadOBJ(const std::string& path);
};

#endif