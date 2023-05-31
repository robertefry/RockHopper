
#ifndef ROCKHOPPER_LOG_FATAL
#define ROCKHOPPER_LOG_FATAL
#endif
#define ROCKHOPPER_ASSERT_FATAL(assert,...) if (not (assert)) ROCKHOPPER_LOG_FATAL(__VA_ARGS__)

#ifndef ROCKHOPPER_LOG_ERROR
#define ROCKHOPPER_LOG_ERROR
#endif
#define ROCKHOPPER_ASSERT_ERROR(assert,...) if (not (assert)) ROCKHOPPER_LOG_ERROR(__VA_ARGS__)

#ifndef ROCKHOPPER_LOG_WARN
#define ROCKHOPPER_LOG_WARN
#endif
#define ROCKHOPPER_ASSERT_WARN(assert,...) if (not (assert)) ROCKHOPPER_LOG_WARN(__VA_ARGS__)

#ifndef ROCKHOPPER_LOG_INFO
#define ROCKHOPPER_LOG_INFO
#endif
#define ROCKHOPPER_ASSERT_INFO(assert,...) if (not (assert)) ROCKHOPPER_LOG_INFO(__VA_ARGS__)

#ifndef ROCKHOPPER_LOG_DEBUG
#define ROCKHOPPER_LOG_DEBUG
#endif
#define ROCKHOPPER_ASSERT_DEBUG(assert,...) if (not (assert)) ROCKHOPPER_LOG_DEBUG(__VA_ARGS__)

#ifndef ROCKHOPPER_LOG_TRACE
#define ROCKHOPPER_LOG_TRACE
#endif
#define ROCKHOPPER_ASSERT_TRACE(assert,...) if (not (assert)) ROCKHOPPER_LOG_TRACE(__VA_ARGS__)
