#pragma once

#define DRY_RETURN_IF(CONDITION, CODE)                                        \
    do                                                                        \
    {                                                                         \
        if (CONDITION)                                                        \
        {                                                                     \
            DRY_LOG_ERROR("%s, ret: %d", #CONDITION, static_cast<int>(CODE)); \
            return CODE;                                                      \
        }                                                                     \
    } while (0)

#define DRY_RETURN_IF_F(CONDITION, CODE, FMT, ARGS...)                                     \
    do                                                                                     \
    {                                                                                      \
        if (CONDITION)                                                                     \
        {                                                                                  \
            DRY_LOG_ERROR("(%s, ret: %d) " FMT, #CONDITION, static_cast<int>(CODE), ARGS); \
            return CODE;                                                                   \
        }                                                                                  \
    } while (0)

#define DRY_CALL_LOG(FUNC, ARGS...)                                           \
    do                                                                        \
    {                                                                         \
        const int __DRY_ret__ = (FUNC)(ARGS);                                 \
        if (__DRY_ret__ != 0)                                                 \
        {                                                                     \
            DRY_LOG_ERROR("CallFunc %s failed, ret: %d", #FUNC, __DRY_ret__); \
            return __DRY_ret__;                                               \
        }                                                                     \
    } while (0)
