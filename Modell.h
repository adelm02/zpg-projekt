#ifndef ZPGPROJ_MODELL_H
#define ZPGPROJ_MODELL_H


#include <string>
#include <GL/glew.h>

#include "Material.h"
#include "tiny_obj_loader/tiny_obj_loader.h"

class Modell {
private:
    GLuint vao;
    GLuint vbo;
    GLsizei vertexCount;
    Material material;

public:
    Modell();

    void loadData(const float* data, int dataSize, int stride);
    void draw() const;
    bool loadOBJ(const std::string& path);
    Material & getMaterial();
};

#endif