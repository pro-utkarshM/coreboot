/* SPDX-License-Identifier: GPL-2.0-only */

/* RAM driver for the SMSC LPC47M15X Super I/O chip */

#include <device/device.h>
#include <device/pnp.h>
#include <superio/conf_mode.h>
#include <pc80/keyboard.h>

#include "lpc47m15x.h"

/* Forward declarations */
static void enable_dev(struct device *dev);
static void lpc47m15x_init(struct device *dev);

struct chip_operations superio_smsc_lpc47m15x_ops = {
	.name = "SMSC LPC47M15x/192/997 Super I/O",
	.enable_dev = enable_dev
};

static struct device_operations ops = {
	.read_resources   = pnp_read_resources,
	.set_resources    = pnp_set_resources,
	.enable_resources = pnp_enable_resources,
	.enable           = pnp_alt_enable,
	.init             = lpc47m15x_init,
	.ops_pnp_mode     = &pnp_conf_mode_55_aa,
};

static struct pnp_info pnp_dev_info[] = {
	{ NULL, LPC47M15X_FDC, PNP_IO0 | PNP_IRQ0 | PNP_DRQ0, 0x07f8, },
	{ NULL, LPC47M15X_PP,  PNP_IO0 | PNP_IRQ0 | PNP_DRQ0, 0x07f8, },
	{ NULL, LPC47M15X_SP1, PNP_IO0 | PNP_IRQ0, 0x07f8, },
	{ NULL, LPC47M15X_SP2, PNP_IO0 | PNP_IRQ0, 0x07f8, },
	{ NULL, LPC47M15X_KBC, PNP_IO0 | PNP_IO1 | PNP_IRQ0 | PNP_IRQ1,
		0x07ff, 0x07ff, },
};

static void enable_dev(struct device *dev)
{
	pnp_enable_devices(dev, &ops, ARRAY_SIZE(pnp_dev_info), pnp_dev_info);
}

static void lpc47m15x_init(struct device *dev)
{
	if (!dev->enabled)
		return;

	switch (dev->path.pnp.device) {
	case LPC47M15X_KBC:
		pc_keyboard_init(NO_AUX_DEVICE);
		break;
	}
}
