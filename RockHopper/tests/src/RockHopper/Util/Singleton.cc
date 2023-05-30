
#include <catch2/catch_all.hpp>

#include "RockHopper/Util/Singleton.hh"

TEST_CASE("Util::Singleton")
{
    static int instance_count = 0;

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

    SECTION("lifetime")
    {
        Singleton* obj_1 = new Singleton;
        REQUIRE(instance_count == 1);

        Singleton* obj_2 = new Singleton;
        REQUIRE(instance_count == 1);

        delete obj_2;
        REQUIRE(instance_count == 1);

        delete obj_1;
        REQUIRE(instance_count == 0);
    }

    SECTION("copying")
    {
        Singleton* obj_1 = new Singleton;
        REQUIRE(instance_count == 1);

        Singleton* obj_2 = new Singleton{*obj_1};
        REQUIRE(instance_count == 1);

        delete obj_2;
        REQUIRE(instance_count == 1);

        delete obj_1;
        REQUIRE(instance_count == 0);
    }

    SECTION("moving")
    {
        Singleton* obj_1 = new Singleton;
        REQUIRE(instance_count == 1);

        Singleton* obj_2 = new Singleton{std::move(*obj_1)};
        REQUIRE(instance_count == 1);

        delete obj_2;
        REQUIRE(instance_count == 0);

        delete obj_1;
        REQUIRE(instance_count == 0);
    }

    SECTION("Util::NullSingleton lifetime")
    {
        Singleton* obj_1 = new Singleton{RockHopper::Util::NullSingleton};
        REQUIRE(instance_count == 0);

        Singleton* obj_2 = new Singleton{RockHopper::Util::NullSingleton};
        REQUIRE(instance_count == 0);

        *obj_1 = Singleton{};
        REQUIRE(instance_count == 1);

        *obj_2 = Singleton{};
        REQUIRE(instance_count == 1);

        *obj_2 = RockHopper::Util::NullSingleton;
        REQUIRE(instance_count == 1);

        *obj_1 = RockHopper::Util::NullSingleton;
        REQUIRE(instance_count == 0);

        delete obj_2;
        REQUIRE(instance_count == 0);

        delete obj_1;
        REQUIRE(instance_count == 0);
    }
}
