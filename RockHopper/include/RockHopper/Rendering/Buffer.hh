
#ifndef __HH_ROCKHOPPER_RENDERING_BUFFER_
#define __HH_ROCKHOPPER_RENDERING_BUFFER_

#include "RockHopper/Debug.hh"

#include <vector>
#include <memory>
#include <type_traits>

namespace RockHopper
{

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        static std::unique_ptr<VertexBuffer> Create();

        enum Type
        {
            BOOL, INT, FLOAT,
        };
        struct ElementLayout
        {
            Type type;
            uint32_t count;
            bool normalized;
            std::string name;
        };

        struct Data
        {
            std::vector<float> vertices{};
            std::vector<ElementLayout> layout{};
        };
        virtual void upload(Data const& data) = 0;

        virtual void bind() = 0;
        virtual void unbind() = 0;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        static std::unique_ptr<IndexBuffer> Create();

        struct Data
        {
            std::vector<uint32_t> indices{};
        };
        virtual void upload(Data const& data) = 0;

        virtual void bind() = 0;
        virtual void unbind() = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_BUFFER_ */
