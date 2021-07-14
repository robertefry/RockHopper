
#ifndef __HH_ROCKHOPPER_RENDERING_BUFFER_
#define __HH_ROCKHOPPER_RENDERING_BUFFER_

#include <vector>
#include <memory>

namespace RockHopper
{

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        explicit VertexBuffer() = default;

        static std::unique_ptr<VertexBuffer> Create();

        virtual void upload(std::vector<float> const& data) = 0;

        virtual void bind() = 0;
        virtual void unbind() = 0;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        explicit IndexBuffer() = default;

        static std::unique_ptr<IndexBuffer> Create();

        virtual void upload(std::vector<uint32_t> const& data) = 0;

        virtual void bind() = 0;
        virtual void unbind() = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_BUFFER_ */
