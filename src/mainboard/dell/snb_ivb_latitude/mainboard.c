/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/device.h>
#include <drivers/intel/gma/int15.h>
#include <pc80/keyboard.h>

static void mainboard_enable(struct device *dev)
{

	/* FIXME: fix these values. */
	install_intel_vga_int15_handler(GMA_INT15_ACTIVE_LFP_INT_LVDS,
					GMA_INT15_PANEL_FIT_DEFAULT,
					GMA_INT15_BOOT_DISPLAY_DEFAULT, 0);
	pc_keyboard_init(NO_AUX_DEVICE);
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
