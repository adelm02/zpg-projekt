//
// Created by Adéla Martynková on 26.09.2025.
//

#include "Modell.h"

Modell::Modell() : vao(0), vbo(0), vertexCount(0) {
    glGenVertexArrays(1, &vao); //vytvoří 1 a uloží id do vao
    glGenBuffers(1, &vbo);
}

void Modell::loadData(const float* float_array, int float_array_size, int stride) {//stride xyz nebo s barvou + 3
    vertexCount = float_array_size / stride;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, float_array_size * sizeof(float), float_array, GL_STATIC_DRAW);

    // první 3 floaty
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);

    // barva >= 6
    if (stride >= 6) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    }
}

void Modell::draw() const{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}