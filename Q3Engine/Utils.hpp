#pragma once

#include "Buffer.hpp"
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
    std::shared_ptr<q3::DataBuffer<q3::Vector3>> vertices;
    std::shared_ptr<q3::DataBuffer<q3::Vector2>> uvs;
    std::shared_ptr<q3::DataBuffer<q3::Vector3>> normals;
    std::shared_ptr<q3::DataBuffer<uint32_t>> indices;
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
    q3::DataBuffer<uint32_t> indices;

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

    q3::DataBuffer<q3::Vector3> vertices(vertex_map_size);
    q3::DataBuffer<q3::Vector2> uvs(vertex_map_size);
    q3::DataBuffer<q3::Vector3> normals(vertex_map_size);

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
        std::make_shared<q3::DataBuffer<q3::Vector3>>(std::move(vertices)),
        std::make_shared<q3::DataBuffer<q3::Vector2>>(std::move(uvs)),
        std::make_shared<q3::DataBuffer<q3::Vector3>>(std::move(normals)),
        std::make_shared<q3::DataBuffer<uint32_t>>(std::move(indices))
    };
}

}
