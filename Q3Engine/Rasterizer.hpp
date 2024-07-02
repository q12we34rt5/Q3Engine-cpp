#pragma once

#include "Buffer.hpp"
#include "RGBColor.hpp"
#include "Math.hpp"
#include "Shader.hpp"

#include <algorithm>
#include <memory>

#ifdef _WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

namespace q3 {

class Rasterizer {
public:
    Rasterizer() = default;
    Rasterizer(std::shared_ptr<GraphicsBuffer<RGBColor>> framebuffer, std::shared_ptr<GraphicsBuffer<float>> depthbuffer)
        : framebuffer_(framebuffer), depthbuffer_(depthbuffer), framebuffer_ptr_(framebuffer.get()), depthbuffer_ptr_(depthbuffer.get()) {}

    void setFramebuffer(std::shared_ptr<GraphicsBuffer<RGBColor>> framebuffer) {
        framebuffer_ = framebuffer;
        framebuffer_ptr_ = framebuffer.get();
    }
    void setDepthbuffer(std::shared_ptr<GraphicsBuffer<float>> depthbuffer) {
        depthbuffer_ = depthbuffer;
        depthbuffer_ptr_ = depthbuffer.get();
    }

    std::shared_ptr<GraphicsBuffer<RGBColor>> getFramebuffer() const { return framebuffer_; }
    std::shared_ptr<GraphicsBuffer<float>> getDepthbuffer() const { return depthbuffer_; }

    inline void clearFrameBuffer(const RGBColor& color = RGBColor()) { framebuffer_ptr_->fill(color); }
    inline void clearDepthBuffer(float value = 1.0f) { depthbuffer_ptr_->fill(value); }

    inline void drawBuffer(const DataBuffer<Vector3>& vertices, const DataBuffer<uint32_t>& indices, Shader& shader, BaseDataBufferSampler& sampler) {
        for (size_t i = 0; i < indices.size(); i += 3) {
            const Vector3& v0 = vertices[indices[i]];
            const Vector3& v1 = vertices[indices[i + 1]];
            const Vector3& v2 = vertices[indices[i + 2]];
            void* data0 = sampler.getValue(i);
            void* data1 = sampler.getValue(i + 1);
            void* data2 = sampler.getValue(i + 2);
            drawTriangle(v0, v1, v2, shader, data0, data1, data2);
        }
    }

    inline void drawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, Shader& shader, void* data0 = nullptr, void* data1 = nullptr, void* data2 = nullptr) {
        Vertex v0_(v0);
        Vertex v1_(v1);
        Vertex v2_(v2);
        void* context = alloca(shader.getContextSize());

        bool drawable = shader.vertexShader(v0_, v1_, v2_, data0, data1, data2, context);
        if (!drawable) return;

        viewportTransform(v0_);
        viewportTransform(v1_);
        viewportTransform(v2_);

        Triangle triangle{v0_.position, v1_.position, v2_.position};

        int32_t bbox_min_x = std::min({static_cast<int32_t>(v0_.position.x), static_cast<int32_t>(v1_.position.x), static_cast<int32_t>(v2_.position.x)});
        int32_t bbox_min_y = std::min({static_cast<int32_t>(v0_.position.y), static_cast<int32_t>(v1_.position.y), static_cast<int32_t>(v2_.position.y)});
        int32_t bbox_max_x = std::max({static_cast<int32_t>(v0_.position.x), static_cast<int32_t>(v1_.position.x), static_cast<int32_t>(v2_.position.x)});
        int32_t bbox_max_y = std::max({static_cast<int32_t>(v0_.position.y), static_cast<int32_t>(v1_.position.y), static_cast<int32_t>(v2_.position.y)});
        bbox_min_x = std::max(0, bbox_min_x);
        bbox_min_y = std::max(0, bbox_min_y);
        bbox_max_x = std::min(static_cast<int32_t>(framebuffer_ptr_->getWidth() - 1), bbox_max_x);
        bbox_max_y = std::min(static_cast<int32_t>(framebuffer_ptr_->getHeight() - 1), bbox_max_y);

        for (int32_t y = bbox_min_y; y <= bbox_max_y; y++) {
            for (int32_t x = bbox_min_x; x <= bbox_max_x; x++) {
                Barycentric barycentric = calculateBarycentric(triangle, {static_cast<float>(x), static_cast<float>(y)});
                if (barycentric.l0 < 0 || barycentric.l1 < 0 || barycentric.l2 < 0) continue;

                float z = v0_.position.z * barycentric.l0 + v1_.position.z * barycentric.l1 + v2_.position.z * barycentric.l2;
                if (z < -1.0f || z > 1.0f) continue;
                if (z > depthbuffer_ptr_->getValue(x, y)) continue;
                depthbuffer_ptr_->setValue(x, y, z);

                q3::RGBColor color = shader.fragmentShader(triangle, barycentric, data0, data1, data2, context);
                framebuffer_ptr_->setValue(x, y, color);
            }
        }
    }

private:
    inline void viewportTransform(q3::Vertex& v) const {
        float width = static_cast<float>(framebuffer_ptr_->getWidth());
        float height = static_cast<float>(framebuffer_ptr_->getHeight());

        // perspective division
        v.position.x /= v.w;
        v.position.y /= v.w;
        v.position.z /= v.w;

        // NDC to screen space
        v.position.x = (v.position.x + 1.0f) * width * 0.5f;
        v.position.y = (1.0f - v.position.y) * height * 0.5f;
        v.position.z = (v.position.z + 1.0f) * 0.5f;

        // keep w for perspective correct interpolation
        v.w = 1.0f / v.w;
    }

private:
    std::shared_ptr<GraphicsBuffer<RGBColor>> framebuffer_;
    std::shared_ptr<GraphicsBuffer<float>> depthbuffer_;
    // for faster access
    GraphicsBuffer<RGBColor>* framebuffer_ptr_;
    GraphicsBuffer<float>* depthbuffer_ptr_;
};

}
