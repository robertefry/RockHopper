
#include "RockHopper/RockHopper.hh"
#include "Sandbox.hh"

#include <memory>

int main()
{
    std::unique_ptr sandbox = std::make_unique<Sandbox>();
    return sandbox->run();
}
