/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "os_wrapper/thread.h"
#include "os_wrapper/mutex.h"
#include "os_wrapper/semaphore.h"

#include <string.h>
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "stdlib.h"

/* This is an example OS abstraction layer for CMSIS-RTOSv2 */

void *os_wrapper_thread_new(const char *name, int32_t stack_size,
                            os_wrapper_thread_func func, void *arg,
                            uint32_t priority)
{
    osRtxThread_t *thread;
    void *stack;

    thread = (osRtxThread_t *)malloc(osRtxThreadCbSize);
    if(thread == NULL){
        return NULL;
    }

    stack = (void *)malloc(stack_size);
    if(stack == NULL){
        free(thread);
        return NULL;
    }

    const osThreadAttr_t thread_attribs = {
#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
        .tz_module = 1,
#endif
        .name = name,
        .priority = (osPriority_t) priority,
        .cb_mem = thread,
        .cb_size = osRtxThreadCbSize,
        .stack_size = stack_size,
        .stack_mem = stack};

    return (void *)osThreadNew(func, arg, &thread_attribs);
}

void *os_wrapper_semaphore_create(uint32_t max_count, uint32_t initial_count,
                                  const char *name)
{
    osRtxSemaphore_t *semaphore;

    semaphore = (osRtxSemaphore_t *)malloc(osRtxSemaphoreCbSize);
    if(semaphore == NULL){
        return NULL;
    }

    const osSemaphoreAttr_t sema_attrib = {
        .name = name,
        .attr_bits = 0,
        .cb_mem = semaphore,
        .cb_size = osRtxSemaphoreCbSize
    };

    return (void *)osSemaphoreNew(max_count, initial_count, &sema_attrib);
}

uint32_t os_wrapper_semaphore_acquire(void *handle, uint32_t timeout)
{
    osStatus_t status;

    status = osSemaphoreAcquire((osSemaphoreId_t)handle,
                                (timeout == OS_WRAPPER_WAIT_FOREVER) ?
                                osWaitForever : timeout);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_semaphore_release(void *handle)
{
    osStatus_t status;

    status = osSemaphoreRelease((osSemaphoreId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_semaphore_delete(void *handle)
{
    osStatus_t status;

    status = osSemaphoreDelete((osSemaphoreId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    free(handle);

    return OS_WRAPPER_SUCCESS;
}

void *os_wrapper_mutex_create(void)
{
    osRtxMutex_t *mutex;

    mutex = (osRtxMutex_t *)malloc(osRtxMutexCbSize);
    if(mutex == NULL){
        return NULL;
    }

    const osMutexAttr_t attr = {
        .name = NULL,
        .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
        .cb_mem = mutex,
        .cb_size = osRtxMutexCbSize
    };

    return (void *)osMutexNew(&attr);
}

uint32_t os_wrapper_mutex_acquire(void *handle, uint32_t timeout)
{
    osStatus_t status = osOK;

    if (!handle) {
        return OS_WRAPPER_ERROR;
    }

    status = osMutexAcquire((osMutexId_t)handle,
                            (timeout == OS_WRAPPER_WAIT_FOREVER) ?
                             osWaitForever : timeout);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_mutex_release(void *handle)
{
    osStatus_t status = osOK;

    if (!handle) {
        return OS_WRAPPER_ERROR;
    }

    status = osMutexRelease((osMutexId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_mutex_delete(void *handle)
{
    osStatus_t status = osOK;

    if (!handle) {
        return OS_WRAPPER_ERROR;
    }

    status = osMutexDelete((osMutexId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    free(handle);

    return OS_WRAPPER_SUCCESS;
}

void *os_wrapper_thread_get_handle(void)
{
    return (void *)osThreadGetId();
}

uint32_t os_wrapper_thread_get_priority(void *handle, uint32_t *priority)
{
    osPriority_t prio;

    prio = osThreadGetPriority((osThreadId_t)handle);
    if (prio == osPriorityError) {
        return OS_WRAPPER_ERROR;
    }

    *priority = (uint32_t)prio;

    return OS_WRAPPER_SUCCESS;
}

void os_wrapper_thread_exit(void)
{
    osThreadExit();
}

uint32_t os_wrapper_thread_delete(void *handle)
{
    osStatus_t status = osOK;

    if (!handle) {
        return OS_WRAPPER_ERROR;
    }

    osRtxThread_t *thread = (osRtxThread_t *)handle;
    void *st_mem = thread->stack_mem;

    status = osThreadTerminate((osThreadId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    free(st_mem);
    free(thread);

    return OS_WRAPPER_SUCCESS;
}
