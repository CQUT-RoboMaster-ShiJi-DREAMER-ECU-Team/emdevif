/**
 * @file thread.cpp
 * @brief
 */

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module;
#else
#include "emdevif/system/thread.hpp"

#include "emdevif/core/error_handler.hpp"
#endif

#include "emdevif/core/fatal_handler.h"

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include <cassert>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#else
#include "FreeRTOS.h"
#include "task.h"
#endif

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module emdevif.sys.thread;

import emdevif.core.error_handler;
#endif

namespace emdevif {

Thread::StronglyTypedHandle Thread::create(const Attribute& attribute,
                                           const ThreadEntry entry,
                                           void* arguments) noexcept
{
    TaskHandle_t handle = nullptr;

    if (attribute.static_instance != nullptr && attribute.stack_size != 0U) {
        // 由于此处的 Thread::StaticInstance 是指针类型，且它的自定义类型转换过程不涉及这个模板参数，
        // 因此它的模板参数的取值不影响结果。但不能为 0，因为这样做会构造出零长数组。
        auto& static_instance = *static_cast<Thread::StaticInstance<1>*>(attribute.static_instance);

        handle = xTaskCreateStatic(entry,
                                   attribute.name,
                                   attribute.stack_size,
                                   arguments,
                                   priorityMapToSystem(attribute.priority),
                                   &static_cast<StackType_t&>(static_instance),
                                   &static_cast<StaticTask_t&>(static_instance));
    }
    else {
        const auto result = xTaskCreate(entry,
                                        attribute.name,
                                        attribute.stack_size,
                                        arguments,
                                        priorityMapToSystem(attribute.priority),
                                        &handle);
        if (result != pdPASS) {
            handle = nullptr;
        }
    }

    return {.value = handle};
}

ErrorCode Thread::destroy(Thread& obj) noexcept
{
    if (obj.getHandle() == nullptr) {
        return ErrorCode::InvalidArgument;
    }

    const auto handle_value = static_cast<TaskHandle_t>(obj.getHandle());
    obj.handle_ = nullptr;

    if (eTaskGetState(handle_value) != eDeleted) {
        vTaskDelete(handle_value);
    }
    else {
        return ErrorCode::InvalidArgument;
    }

    return ErrorCode::Success;
}

Thread::~Thread() noexcept
{
    if (handle_ != nullptr) {
        destroy(*this);
        handle_ = nullptr;
    }

    if (func_wrapper_memory_block_ != nullptr) {
        // todo 任意参数的函数包装器还是存在缺陷：只能构造，不能正确析构。待修改
        EMDEVIF_FATAL_HANDLER("Could not delete.");
        // heap::destruct(func_wrapper_memory_block_);
    }
}

}  // namespace emdevif
