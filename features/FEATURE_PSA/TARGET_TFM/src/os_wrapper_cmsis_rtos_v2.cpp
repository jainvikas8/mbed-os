/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "os_wrapper/thread.h"
#include "os_wrapper/mutex.h"
#include "os_wrapper/semaphore.h"
#include "rtos/Thread.h"
#include "rtos/ThisThread.h"
#include "rtos/Semaphore.h"
#include "rtos/Mutex.h"

extern "C" void *os_wrapper_semaphore_create(uint32_t max_count, uint32_t initial_count,
                                             const char *name)
{
    if (initial_count > max_count) {
        return NULL;
    }

    return (void *)new rtos::Semaphore(initial_count, max_count);
}

extern "C" uint32_t os_wrapper_semaphore_acquire(void *handle, uint32_t timeout)
{
    bool status;
    rtos::Semaphore *new_sema = (rtos::Semaphore *)handle;

    status = new_sema->try_acquire_for((timeout == OS_WRAPPER_WAIT_FOREVER) ?
                                       osWaitForever : timeout);
    if (false == status) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

extern "C" uint32_t os_wrapper_semaphore_release(void *handle)
{
    osStatus status;
    rtos::Semaphore *new_sema = (rtos::Semaphore *)handle;

    status = new_sema->release();
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

extern "C" uint32_t os_wrapper_semaphore_delete(void *handle)
{
    delete (rtos::Semaphore *)handle;

    return OS_WRAPPER_SUCCESS;
}

extern "C" void *os_wrapper_mutex_create(void)
{
    return (void *)new rtos::Mutex();
}

extern "C" uint32_t os_wrapper_mutex_acquire(void *handle, uint32_t timeout)
{
    bool status;
    rtos::Mutex *new_mutex = (rtos::Mutex *)handle;

    status = new_mutex->trylock_for((timeout == OS_WRAPPER_WAIT_FOREVER) ?
                                    osWaitForever : timeout);
    if (!status) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

extern "C" uint32_t os_wrapper_mutex_release(void *handle)
{
    osStatus status = osOK;
    rtos::Mutex *new_mutex = (rtos::Mutex *)handle;

    status = new_mutex->unlock();
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

extern "C" uint32_t os_wrapper_mutex_delete(void *handle)
{
    delete (rtos::Mutex *)handle;

    return OS_WRAPPER_SUCCESS;
}

extern "C" void *os_wrapper_thread_new(const char *name, int32_t stack_size,
                                       os_wrapper_thread_func func, void *arg,
                                       uint32_t priority)
{
    rtos::Thread *new_thread = NULL;
    /* This enables thread context management for trustzone */
    uint32_t tz_module = 1;

    new_thread = new rtos::Thread(tz_module,
                                  (osPriority_t) priority,
                                  (stack_size <= OS_WRAPPER_DEFAULT_STACK_SIZE) ? OS_STACK_SIZE : stack_size,
                                  NULL,
                                  name);
    if (new_thread != NULL) {
        new_thread->start(mbed::callback(func, arg));

        return (void *)new_thread;
    } else {
        return NULL;
    }
}

extern "C" uint32_t os_wrapper_thread_get_priority(void *handle, uint32_t *priority)
{
    osPriority_t prio;
    rtos::Thread *Thread = (rtos::Thread *)handle;

    prio = Thread->get_priority();
    if (prio == osPriorityError) {
        return OS_WRAPPER_ERROR;
    }

    *priority = (uint32_t)prio;

    return OS_WRAPPER_SUCCESS;
}

extern "C" uint32_t os_wrapper_current_thread_get_priority(uint32_t *priority)
{
    osPriority_t prio = rtos::ThisThread::get_priority();
    if (prio == osPriorityError) {
        return OS_WRAPPER_ERROR;
    }

    *priority = (uint32_t)prio;

    return OS_WRAPPER_SUCCESS;
}

extern "C" uint32_t os_wrapper_thread_terminate(void *handle)
{
    delete (rtos::Thread *)handle;

    return OS_WRAPPER_SUCCESS;
}

extern "C" uint32_t os_wrapper_current_thread_suspend(void)
{
    osStatus status = osOK;

    status = rtos::ThisThread::suspend();
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}
