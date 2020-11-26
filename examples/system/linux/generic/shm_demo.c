
#include <stdio.h>
#include <metal/device.h>
#include <metal/alloc.h>
#include "sys_init.h"

#define BUS_NAME	"platform"
#define SHM_DEV_NAME	"90000000.shm"

#define LPRINTF(format, ...) \
	printf("\r\nSERVER> " format, ##__VA_ARGS__)

#define LPERROR(format, ...) LPRINTF("ERROR: " format, ##__VA_ARGS__)

/* Shared memory offsets */
#define SHM_DEMO_CNTRL_OFFSET      0x0
#define SHM_TX_AVAIL_OFFSET        0x04
#define SHM_RX_AVAIL_OFFSET        0x0C
#define SHM_TX_BUFFER_OFFSET       0x14
#define SHM_RX_BUFFER_OFFSET       0x800

#define DEMO_STATUS_IDLE         0x0
#define DEMO_STATUS_START        0x1 /* Status value to indicate demo start */

struct msg_hdr_s {
	uint32_t index;
	uint32_t len;
};

#define TEST_MSG "Hello World - libmetal shared memory demo"

/**
 * @breif dump_buffer() - print hex value of each byte in the buffer
 *
 * @param[in] buf - pointer to the buffer
 * @param[in] len - len of the buffer
 */
static inline void dump_buffer(void *buf, unsigned int len)
{
	unsigned int i;
	unsigned char *tmp = (unsigned char *)buf;

	for (i = 0; i < len; i++) {
		printf(" %02x", *(tmp++));
		if (!(i % 20))
			printf("\n");
	}
	printf("\n");
}

int send_msg_test(struct metal_io_region *shm_io)
{
	void *tx_data = NULL;
	void *rx_data = NULL;
	unsigned int tx_count = 0;
	unsigned int rx_count = 0;
	struct msg_hdr_s *msg_hdr;
	unsigned int data_len;
	int ret;

	LPRINTF("Setting up shared memory demo.\n");
	/* clear demo status value */
	metal_io_write32(shm_io, SHM_DEMO_CNTRL_OFFSET, 0);
	/* Clear TX/RX avail */
	metal_io_write32(shm_io, SHM_TX_AVAIL_OFFSET, 0);
	metal_io_write32(shm_io, SHM_RX_AVAIL_OFFSET, 0);

	LPRINTF("Starting shared memory demo.\n");
	/* Notify the remote the demo starts */
	metal_io_write32(shm_io, SHM_DEMO_CNTRL_OFFSET, DEMO_STATUS_START);

	/* preparing data to send */
	data_len = sizeof(struct msg_hdr_s) + strlen(TEST_MSG) + 1;
	tx_data = metal_allocate_memory(data_len);
	if (!tx_data) {
		LPERROR("Failed to allocate memory.\n");
		ret = -1;
		goto out;
	}
	msg_hdr = (struct msg_hdr_s *)tx_data;
	msg_hdr->index = tx_count;
	msg_hdr->len = strlen(TEST_MSG) + 1;
	sprintf(tx_data + sizeof(*msg_hdr), TEST_MSG);
	LPRINTF("Sending message: %s\n",
		(char *)(tx_data + sizeof(*msg_hdr)));

	/* write data to the shared memory*/
	ret = metal_io_block_write(shm_io, SHM_TX_BUFFER_OFFSET,
		tx_data, data_len);
	if (ret < 0){
		LPERROR("Unable to metal_io_block_write()\n");
		goto out;
	}
	/* Increase number of buffers available to notify the remote */
	tx_count++;
	metal_io_write32(shm_io, SHM_TX_AVAIL_OFFSET, tx_count);

	/* wait for remote to echo back the data */
	while (metal_io_read32(shm_io, SHM_RX_AVAIL_OFFSET) == rx_count);
	rx_count++;
	/* New RX data is available, allocate buffer to received data */
	rx_data = metal_allocate_memory(data_len);
	if (!rx_data) {
		LPERROR("Failed to allocate memory\n");
		ret = -1;
		goto out;
	}
	/* read data from the shared memory*/
	metal_io_block_read(shm_io, SHM_RX_BUFFER_OFFSET,
		 rx_data, data_len);
	if (ret < 0){
		LPERROR("Unable to metal_io_block_read()\n");
		goto out;
	}
	/* verify the received data */
	ret = memcmp(tx_data, rx_data, data_len);
	if (ret) {
		LPERROR("Received data verification failed.\n");
		LPRINTF("Expected:");
		dump_buffer(tx_data, data_len);
		LPRINTF("Actual:");
		dump_buffer(rx_data, data_len);
	} else {
		LPRINTF("Message Received: %s\n",
			(char *)(rx_data + sizeof(*msg_hdr)));
	}
	/* Notify the remote the demo has finished. */
	metal_io_write32(shm_io, SHM_DEMO_CNTRL_OFFSET, DEMO_STATUS_IDLE);

out:
	if (tx_data)
		metal_free_memory(tx_data);
	if (rx_data)
		metal_free_memory(rx_data);
	LPRINTF("Shared memory demo: %s.\n", ret ? "Failed": "Passed" );
	return ret;
}

int main(int argc, char *argv[])
{
	struct metal_device *device = NULL;
	struct metal_io_region *io = NULL;
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

	/* get shared memory device IO region */
	io = metal_device_io_region(device, 0);
	if (!io) {
		LPERROR("Failed to get io region for %s.\n", device->name);
		ret = -ENODEV;
		goto out;
	}

	(void)send_msg_test(io);
out:
	if (device)
		metal_device_close(device);
	return ret;
}

