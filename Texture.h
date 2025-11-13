#ifndef ZPGPROJ_TEXTURE_H
#define ZPGPROJ_TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
private:
    GLuint textureID;
    int width, height;
    int channels; //color

public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& path);
    bool loadCubeMap(const std::string paths[6]);

    void bind(int textureUnit = 0) const;
    void unbind() const;

    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setWrapMode(GLenum wrapS, GLenum wrapT);
    void setFilterMode(GLenum minFilter, GLenum magFilter);
    void generateMipmap();
};

#endif