#pragma once

#include "Buffer.hpp"
#include "RGBColor.hpp"
#include "Math.hpp"

#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace q3 {

struct ObjData {
    std::shared_ptr<DataBuffer<Vector3>> vertices;
    std::shared_ptr<DataBuffer<Vector2>> uvs;
    std::shared_ptr<DataBuffer<Vector3>> normals;
    std::shared_ptr<DataBuffer<uint32_t>> indices;
};

ObjData loadObjFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<float> v;
    std::vector<float> vt;
    std::vector<float> vn;
    std::map<std::string, uint32_t> indices_map;
    uint32_t vertex_map_size = 0;
    DataBuffer<uint32_t> indices;

    std::string line;
    while (std::getline(file, line)) {
        // truncate string after # (comments)
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        // split line by spaces
        std::istringstream line_iss(line);
        std::vector<std::string> line_sp;
        std::string segment;
        while (line_iss >> segment) { line_sp.push_back(segment); }
        // skip empty lines
        if (line_sp.empty()) { continue; }
        // TODO:
        //   1. handle more obj file features

        if (line_sp[0] == "v") {
            v.push_back(std::stof(line_sp[1]));
            v.push_back(std::stof(line_sp[2]));
            v.push_back(std::stof(line_sp[3]));
        } else if (line_sp[0] == "vt") {
            vt.push_back(std::stof(line_sp[1]));
            vt.push_back(std::stof(line_sp[2]));
        } else if (line_sp[0] == "vn") {
            vn.push_back(std::stof(line_sp[1]));
            vn.push_back(std::stof(line_sp[2]));
            vn.push_back(std::stof(line_sp[3]));
        } else if (line_sp[0] == "f") {
            for (size_t i = 1; i < line_sp.size(); ++i) {
                if (indices_map.find(line_sp[i]) == indices_map.end()) {
                    indices_map[line_sp[i]] = vertex_map_size++;
                }
            }
            for (size_t i = 2; i < line_sp.size() - 1; ++i) {
                indices.push_back(indices_map[line_sp[1]]);
                indices.push_back(indices_map[line_sp[i]]);
                indices.push_back(indices_map[line_sp[i + 1]]);
            }
        }
    }

    DataBuffer<Vector3> vertices(vertex_map_size);
    DataBuffer<Vector2> uvs(vertex_map_size);
    DataBuffer<Vector3> normals(vertex_map_size);

    for (const auto& [key, value] : indices_map) {
        std::istringstream key_iss(key);
        std::vector<std::string> sp;
        std::string segment;
        while (std::getline(key_iss, segment, '/')) {
            sp.push_back(segment);
        }
        int vi = 3 * (std::stoi(sp[0]) - 1);
        vertices[value] = {v[vi], v[vi + 1], v[vi + 2]};
        if (sp.size() >= 2 && !sp[1].empty()) {
            int uvi = 2 * (std::stoi(sp[1]) - 1);
            uvs[value] = {vt[uvi], vt[uvi + 1]};
        }
        if (sp.size() >= 3 && !sp[2].empty()) {
            int ni = 3 * (std::stoi(sp[2]) - 1);
            normals[value] = {vn[ni], vn[ni + 1], vn[ni + 2]};
        }
    }
    return {
        std::make_shared<DataBuffer<Vector3>>(std::move(vertices)),
        std::make_shared<DataBuffer<Vector2>>(std::move(uvs)),
        std::make_shared<DataBuffer<Vector3>>(std::move(normals)),
        std::make_shared<DataBuffer<uint32_t>>(std::move(indices))
    };
}

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t type;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t depth;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_meter;
    uint32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t important_colors;
};
#pragma pack(pop)

std::shared_ptr<GraphicsBuffer<RGBColor>> loadBmpTexture(const std::string& filename, RGBColor transparency_key = RGBColor{0, 0, 0, 0}) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    // read BMP header
    BMPHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    // check if BMP file
    if (header.type != 0x4D42) {
        throw std::runtime_error("Invalid BMP file: " + filename);
    }
    // check if BMP file is valid
    if (header.planes != 1) {
        throw std::runtime_error("Invalid number of planes in BMP file");
    }
    // check if 24-bit BMP file
    if (header.depth != 24) {
        throw std::runtime_error("Unsupported BMP depth: " + std::to_string(header.depth));
    }
    // check if BMP file is compressed
    // (note: BMP compression is not supported in this implementation)
    if (header.compression != 0) {
        throw std::runtime_error("Compressed BMP not supported");
    }
    // check if image size is 0
    if (header.image_size == 0) {
        uint32_t row_size = (header.width * 3 + 3) & ~3; // row size padded to 4 bytes
        header.image_size = row_size * header.height;
    }
    // check if file size is valid
    file.seekg(0, std::ios::end);
    auto file_size = file.tellg();
    if (file_size < header.offset + header.image_size) {
        throw std::runtime_error("Invalid BMP file size: " + filename);
    }
    // create image buffer
    auto imagebuffer = std::make_shared<GraphicsBuffer<RGBColor>>(header.width, header.height);
    // read image data
    file.seekg(header.offset, std::ios::beg); // move to pixel data
    std::vector<uint8_t> data(header.image_size);
    file.read(reinterpret_cast<char*>(data.data()), header.image_size);
    if (!file) {
        throw std::runtime_error("Failed to read BMP file: " + filename);
    }
    // std::vector<uint8_t> row_data(row_size); // temporary buffer for one row
    uint32_t padding = (4 - (header.width * 3) % 4) % 4; // padding for each row
    uint32_t index = 0;
    for (int32_t y = header.height - 1; y >= 0; --y) {
        for (uint32_t x = 0; x < header.width; ++x) {
            size_t i = x * 3;
            // BMP image data is stored in BGR format
            uint8_t b = data[index++];
            uint8_t g = data[index++];
            uint8_t r = data[index++];
            uint8_t a = 255;
            // check if pixel is transparent
            if (r == transparency_key.r && g == transparency_key.g && b == transparency_key.b && a == transparency_key.a) {
                a = 0;
            }
            // set pixel color
            imagebuffer->setValue(x, y, RGBColor{r, g, b, a});
        }
        index += padding; // skip padding bytes
    }
    return imagebuffer;
}

}
