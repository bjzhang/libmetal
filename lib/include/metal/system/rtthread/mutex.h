/*
 * Copyright (c) 2020, RTT
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file    rt-thread/mutex.h
 * @brief   rt-thread mutex primitives for libmetal.
 */

#ifndef __METAL_MUTEX__H__
    #error "Include metal/mutex.h instead of metal/rtthread/mutex.h"
#endif

#ifndef __METAL_RTTHREAD_MUTEX__H__
#define __METAL_RTTHREAD_MUTEX__H__

#include <metal/atomic.h>
#include <stdio.h>
#include <rtdef.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rt_mutex metal_mutex_t;
static inline void __metal_mutex_init(metal_mutex_t *m);

/*
 * METAL_MUTEX_INIT - used for initializing an mutex elmenet in a static struct
 * or global
 */
#define METAL_MUTEX_INIT(m)                     \
    do {                                \
        metal_mutex_t m;                    \
        __metal_mutex_init(&m);                 \
    } while (0);

/*
 * METAL_MUTEX_DEFINE - used for defining and initializing a global or
 * static singleton mutex
 */
#define METAL_MUTEX_DEFINE(m) metal_mutex_t m = METAL_MUTEX_INIT(m)

static inline void __metal_mutex_init(metal_mutex_t *m)
{
    //rt_err_t rt_mutex_init (rt_mutex_t mutex, const char* name, rt_uint8_t flag);
    rt_err_t err;
    char *name = "RTT_MUTEX_deadbeefdeadbeef";
    const char *mutex_name;

    sprintf(name, "RTT_MUTEX_%llx", m);
    mutex_name = name;
    err = rt_mutex_init(m, mutex_name, RT_IPC_FLAG_FIFO);
}

static inline void __metal_mutex_deinit(metal_mutex_t *m)
{
    rt_mutex_detach(m);
}

static inline int __metal_mutex_try_acquire(metal_mutex_t *m)
{
    rt_err_t err;

    err = rt_mutex_take(m, 0);
    if (RT_EOK == err)
        return -err;
    else
        return 0;

}

static inline int __metal_mutex_is_acquired(metal_mutex_t *m)
{
    return 0 == m->value;
}

static inline void __metal_mutex_acquire(metal_mutex_t *m)
{
    rt_mutex_take(m, RT_WAITING_FOREVER);
}

static inline void __metal_mutex_release(metal_mutex_t *m)
{
    rt_mutex_release(m);
}

#ifdef __cplusplus
}
#endif

#endif /* __METAL_ZEPHYR_MUTEX__H__ */
