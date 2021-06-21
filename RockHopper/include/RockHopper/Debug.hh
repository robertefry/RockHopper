
#ifndef __HH_ROCKHOPPER_DEBUG_
#define __HH_ROCKHOPPER_DEBUG_

#include "RockHopper/Logging/Logger.hh"

#define ROCKHOPPER_ASSERT_TRACE(assert,message) if (not #assert) { ROCKHOPPER_LOG_TRACE(#message); }
#define ROCKHOPPER_ASSERT_DEBUG(assert,message) if (not #assert) { ROCKHOPPER_LOG_DEBUG(#message); }
#define ROCKHOPPER_ASSERT_INFO(assert,message) if (not #assert) { ROCKHOPPER_LOG_INFO(#message); }
#define ROCKHOPPER_ASSERT_WARN(assert,message) if (not #assert) { ROCKHOPPER_LOG_WARN(#message); }
#define ROCKHOPPER_ASSERT_ERROR(assert,message) if (not #assert) { ROCKHOPPER_LOG_ERROR(#message); }
#define ROCKHOPPER_ASSERT_FATAL(assert,message) if (not #assert) { ROCKHOPPER_LOG_FATAL(#message); abort(); }

#define ROCKHOPPER_INTERNAL_ASSERT_INFO(assert,message) if (not #assert) { ROCKHOPPER_LOGINTERNAL_INFO(#message); }
#define ROCKHOPPER_INTERNAL_ASSERT_WARN(assert,message) if (not #assert) { ROCKHOPPER_LOGINTERNAL_WARN(#message); }
#define ROCKHOPPER_INTERNAL_ASSERT_ERROR(assert,message) if (not #assert) { ROCKHOPPER_LOGINTERNAL_ERROR(#message); }
#define ROCKHOPPER_INTERNAL_ASSERT_FATAL(assert,message) if (not #assert) { ROCKHOPPER_LOGINTERNAL_FATAL(#message); abort(); }

#endif /* __HH_ROCKHOPPER_DEBUG_ */
