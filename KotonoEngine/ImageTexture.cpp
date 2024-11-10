#include "ImageTexture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::vector<float> vertices =
{
    // Positions         // Texture Coords
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // Top Right
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Bottom Right
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // Bottom Left
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f  // Top Left 
};

const std::vector<int> indices =
{
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
};

ImageTexture::ImageTexture(const std::string& path) :
    _handle(0),
    _path(path)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    GLenum format;
    switch (nrChannels)
    {
    case 1:
        format = GL_RED;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        std::cerr << "Invalid format for texture: " << path << std::endl;
        return;
    }

    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_2D, _handle);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free image data after uploading to GPU
    stbi_image_free(data);

    _vertexArraySetup.VertexArrayObject.Bind();
    _vertexArraySetup.VertexBufferObject.SetData(vertices);
    _vertexArraySetup.ElementBufferObject.SetData(indices);
}

void ImageTexture::SetShader(const Shader& shader) const
{
    _vertexArraySetup.VertexArrayObject.SetVertexAttributesLayout(shader);
}

const std::string& ImageTexture::GetPath() const
{
    return _path;
}

void ImageTexture::Draw() const
{
    glBindTexture(GL_TEXTURE_2D, _handle);
    _vertexArraySetup.VertexArrayObject.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
