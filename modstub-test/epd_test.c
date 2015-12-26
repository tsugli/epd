#include <stdlib.h>
#include <stdio.h>

#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/init.h>

#include "../epd_g1.h"

static struct g1_platform_data pdata = {
	.type = G1_TYPE_2_7,
	.gpio_panel_on = 1,
	.gpio_reset = 2,
	.gpio_border = 3,
	.gpio_busy = 4,
	.gpio_discharge = 5,
};

static struct spi_device spi_dev = {
	.dev = {
		.platform_data = &pdata,
	}
};

/* /dev/epdctl file stub */
static struct inode epdctl = {
	.i_rdev = MKDEV(1, 0),
};

/* /dev/epd0 file stub */
static struct inode epd0 = {
	.i_rdev = MKDEV(1, 1),
};

int main(void)
{
	loff_t off = 0;
	int fctl;

	devices_init();
	_spi_driver->probe(&spi_dev);

	fctl = cdev_open(&epdctl);
	if(fctl < 0) {
		printk("Cannot open /dev/epdctl\n");
		return -1;
	}
	cdev_write(fctl, "W0", 2, &off);

	cdev_close(fctl);

	_spi_driver->remove(&spi_dev);
	devices_exit();
	return 0;
}
