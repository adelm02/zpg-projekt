//
// Created by Adéla Martynková on 26.09.2025.
//

#include "Modell.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include "tiny_obj_loader/tiny_obj_loader.h"


Modell::Modell() : vao(0), vbo(0), vertexCount(0) {
    glGenVertexArrays(1, &vao); //create and set vao vbo
    glGenBuffers(1, &vbo);
}

void Modell::loadData(const float* float_array, int float_array_size, int stride) {
    vertexCount = float_array_size / stride;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, float_array_size * sizeof(float), float_array, GL_STATIC_DRAW);

    // first 3 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);

    // color >= 6
    if (stride >= 6) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    }
}

void Modell::draw() const{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


bool Modell::loadOBJ(const std::string& path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string basedir;
    if (auto pos = path.find_last_of("/\\"); pos != std::string::npos) {
        basedir = path.substr(0, pos + 1);
    }

    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                               path.c_str(),
                               basedir.empty() ? nullptr : basedir.c_str(), true);
    if (!warn.empty()) std::cerr << "[tinyobj] " << warn << "\n";
    if (!ok) {
        std::cerr << "[tinyobj] " << err << " (path='" << path << "')" << "\n";
        return false;
    }

    std::vector<float> verts;
    verts.reserve(shapes.size() * 3000);

    for (const auto& sh : shapes) {
        for (const auto& idx : sh.mesh.indices) {
            int vi = 3 * idx.vertex_index;
            verts.push_back(attrib.vertices[vi + 0]);
            verts.push_back(attrib.vertices[vi + 1]);
            verts.push_back(attrib.vertices[vi + 2]);

            float nx = 0.f, ny = 0.f, nz = 1.f;
            if (idx.normal_index >= 0 && !attrib.normals.empty()) {
                int ni = 3 * idx.normal_index;
                nx = attrib.normals[ni + 0];
                ny = attrib.normals[ni + 1];
                nz = attrib.normals[ni + 2];
            }
            verts.push_back(nx); verts.push_back(ny); verts.push_back(nz);
        }
    }

    loadData(verts.data(), (int)verts.size(), 6);

    if (!materials.empty()) {
        const auto& m = materials[0];
        material.ra = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
        material.rd = glm::vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
        material.rs = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
        material.h  = m.shininess;

        printf("%f %f %f\n", m.ambient[0], m.ambient[1], m.ambient[2]);
        printf("%f %f %f\n", m.diffuse[0], m.diffuse[1], m.diffuse[2]);

        printf("%f %f %f\n", m.specular[0], m.specular[1], m.specular[2]);


    }


    return true;
}

Material & Modell::getMaterial() {
    return material;
}
