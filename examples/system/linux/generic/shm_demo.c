
#include <stdio.h>
#include <metal/device.h>
#include "sys_init.h"

#define BUS_NAME	"platform"
#define SHM_DEV_NAME	"90000000.shm"

#define LPERROR	printf

int main(int argc, char *argv[])
{
	struct metal_device *device = NULL;
	int ret = 0;

	(void)argc;
	(void)argv;

	ret = sys_init();
	if (ret) {
		LPERROR("Failed to initialize system.\n");
		return ret;
	}

	/* Open the shared memory device */
	ret = metal_device_open(BUS_NAME, SHM_DEV_NAME, &device);
	if (ret) {
		printf("Failed to open device %s.\n", SHM_DEV_NAME);
		return ret;
	}

	return ret;
}

