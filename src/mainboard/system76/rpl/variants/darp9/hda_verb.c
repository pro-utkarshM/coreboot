/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/azalia_device.h>

const u32 cim_verb_data[] = {
	/* Realtek, ALC256 */
	0x10ec0256, /* Vendor ID */
	0x155851b1, /* Subsystem ID */
	19, /* Number of entries */
	0x0205001a, 0x02048003, 0x0205001a, 0x0204c003,
	AZALIA_SUBVENDOR(0, 0x155851b1),
	AZALIA_RESET(1),
	AZALIA_PIN_CFG(0, 0x12, 0x90a60130),
	AZALIA_PIN_CFG(0, 0x13, 0x40000000),
	AZALIA_PIN_CFG(0, 0x14, 0x90170110),
	AZALIA_PIN_CFG(0, 0x18, AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, 0x19, AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, 0x1a, AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, 0x1b, AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, 0x1d, 0x41700001),
	AZALIA_PIN_CFG(0, 0x1e, AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, 0x21, 0x02211020),
	0x02050038, 0x02047901, 0x02050007, 0x02040202,
	0x02050008, 0x02046a0e, 0x0205001b, 0x02040a4b,
	0x0205003c, 0x02040354, 0x0205003c, 0x02040314,
	0x02050046, 0x02040004, 0x05750003, 0x057409a2,
	0x02050010, 0x02040020, 0x02050036, 0x02043050,
	0x00170503, 0x0143b000, 0x0213b000, 0x02170740,
};

const u32 pc_beep_verbs[] = {};

AZALIA_ARRAY_SIZES;
