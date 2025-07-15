#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <iostream>

class Texture {
public:
    unsigned int textureID;
    bool loadedSuccessfully;
    Texture(const std::string& imagePath);
    void bind() const;
};

#endif