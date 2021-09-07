#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>

// #include "common_headers.h"

namespace motion_animation {

namespace util {

class ImageLoader {
public:
  struct GLTextureDesc {
    GLsizei width = 0;
    GLsizei height = 0;
    GLenum format = GL_RGBA;
  };
  // static int LoadImageDataFromFile(std::vector<uint8_t>& imageData, D3D12_RESOURCE_DESC& texture_desc, const std::string& filename, int& bytesPerRow);
  static int LoadImageDataFromFile(std::vector<uint8_t>& image_data, GLTextureDesc& gl_texture_desc, const std::string& file_name, int& bytes_per_row);
};  // class ImageLoader

}  // namespace util

}  // namespace motion_animation
