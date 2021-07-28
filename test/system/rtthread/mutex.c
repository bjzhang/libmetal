/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "metal-test.h"
#include <metal/log.h>
#include <metal/sys.h>
#include <metal/mutex.h>
#include <rtthread.h>

static const int mutex_test_count = 2;
static metal_mutex_t lock;

static void libmetal_usleep(int ms)
{
    rt_thread_mdelay(ms);
}

static void *mutex_thread(void *arg)
{
    metal_mutex_t *l = arg;
    struct rt_thread *t = rt_thread_self();
    int i;

     rt_kprintf("20:00 %s\n", t->name);
//
    for (i = 0; i < mutex_test_count; i++)
    {
        rt_kprintf("%s: before metal_mutex_acquire\n", t->name);
        metal_mutex_acquire(l);
        rt_kprintf("%s: before usleep\n", t->name);
        libmetal_usleep(1);
        rt_kprintf("%s: before metal_mutex_release\n", t->name);
        metal_mutex_release(l);
//	if (i % 100 == 0)
//		rt_kprintf("count: %d\n", i);
    }
    return;
}

int mutex(void)
{
    const int threads = 5;
    int rc;

    metal_mutex_init(&lock);

    rc = metal_run(threads, mutex_thread, &lock);

    metal_mutex_deinit(&lock);

    return rc;
}
METAL_ADD_TEST(mutex);
