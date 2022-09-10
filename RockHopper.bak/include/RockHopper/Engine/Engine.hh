
#ifndef __HH_ROCKHOPPER_ENGINE_
#define __HH_ROCKHOPPER_ENGINE_

#include "RockHopper/Engine/EngineEvents.hh"
#include "RockHopper/Engine/EngineThread.hh"
#include "RockHopper/Event/EventHandler.hh"

namespace RockHopper
{

    class Engine
        : public EngineThread
        , public EventHandler<EngineEvent>
    {
    public:
        virtual ~Engine() = default;
        explicit Engine(std::string const& debug_name);

    private:
        virtual void init() override;
        virtual void tick() override;
        virtual void dispose() override;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_ */
