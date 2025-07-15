#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <vector>

class Model {
public:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Model(const std::vector<float>& vertexData, const std::vector<unsigned int>& indexData, int stride, const std::vector<int>& attributeSizes);
    ~Model();
    void draw() const;
};

#endif