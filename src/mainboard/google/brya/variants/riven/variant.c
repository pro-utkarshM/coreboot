/* SPDX-License-Identifier: GPL-2.0-only */

#include <baseboard/variants.h>
#include <console/console.h>
#include <device/device.h>
#include <fw_config.h>

void variant_devtree_update(void)
{
	struct device *emmc = DEV_PTR(emmc);
	struct device *ufs = DEV_PTR(ufs);
	struct device *ish = DEV_PTR(ish);

	if (!fw_config_is_provisioned()) {
		printk(BIOS_INFO, "fw_config unprovisioned so enable all storage devices\n");
		return;
	}

	if (!fw_config_probe(FW_CONFIG(STORAGE, STORAGE_EMMC))) {
		printk(BIOS_INFO, "eMMC disabled by fw_config\n");
		emmc->enabled = 0;
	}

	if (!fw_config_probe(FW_CONFIG(STORAGE, STORAGE_UFS))) {
		printk(BIOS_INFO, "UFS disabled by fw_config\n");
		ufs->enabled = 0;
		ish->enabled = 0;
	}
}
