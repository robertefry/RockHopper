
#include <catch2/catch_all.hpp>

#include "RockHopper/Util/Singleton.hh"

TEST_CASE("Util::Singleton")
{
    static size_t instance_count = 0;

    struct Object
    {
        explicit Object()
        {
            instance_count += 1;
        }
        virtual ~Object()
        {
            instance_count -= 1;
        }

        Object(Object const&) = delete;
        Object& operator=(Object const&) = delete;
    };
    using Singleton = RockHopper::Util::Singleton<Object>;

    REQUIRE(instance_count == 0);

    Singleton* obj_1 = new Singleton;
    REQUIRE(instance_count == 1);

    Singleton* obj_2 = new Singleton;
    REQUIRE(instance_count == 1); // no object is constructed

    delete obj_2;
    REQUIRE(instance_count == 1); // no object is destructed

    delete obj_1;
    REQUIRE(instance_count == 0);
}
