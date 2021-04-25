#pragma once

#include <memory>

namespace Rendering
{
    class Shader;
    class Texture;
    class Sampler;

    class Material
    {
    public:
        void setShader(const std::shared_ptr<Shader>& shader) { m_shader = shader; }
        void setTexture(const std::shared_ptr<Texture>& texture) { m_texture = texture; }
        void setSampler(const std::shared_ptr<Sampler>& sampler) { m_sampler = sampler; }

        const std::shared_ptr<Shader>& getShader() const { return m_shader; }
        const std::shared_ptr<Texture>& getTexture() const { return m_texture; }
        const std::shared_ptr<Sampler>& getSampler() const { return m_sampler; }

    private:
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Texture> m_texture;
        std::shared_ptr<Sampler> m_sampler;
    };
}