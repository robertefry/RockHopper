
#ifndef __HH_ROCKHOPPER_RENDERING_MESH_
#define __HH_ROCKHOPPER_RENDERING_MESH_

#include <vector>
#include <memory>

namespace RockHopper
{

    class Mesh
    {
    public:
        virtual ~Mesh() = default;
        static std::unique_ptr<Mesh> Create();

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual size_t num_vertices() const = 0;
        virtual size_t num_indices() const = 0;

        struct Data
        {
            enum class Type
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
            std::vector<ElementLayout> layout{};
            std::vector<float> vertices{};
            std::vector<uint32_t> indices{};
        };
        virtual void upload(Data const& data) = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_MESH_ */
