/* SPDX-License-Identifier: GPL-2.0-only */

#include <memlayout.h>
#include <arch/header.ld>
#include <psp_verstage/psp_transfer.h>

#define EARLY_RESERVED_DRAM_START(addr)		REGION_START(early_reserved_dram, addr)
#define EARLY_RESERVED_DRAM_END(addr)		REGION_END(early_reserved_dram, addr)

#define PSP_SHAREDMEM_DRAM_START(addr)		REGION_START(psp_sharedmem_dram, addr)
#define PSP_SHAREDMEM_DRAM_END(addr)		REGION_END(psp_sharedmem_dram, addr)

BOOTBLOCK_END = CONFIG_ROMSTAGE_ADDR;
BOOTBLOCK_ADDR = BOOTBLOCK_END - CONFIG_C_ENV_BOOTBLOCK_SIZE;

/*
 *
 *                     +--------------------------------+
 *                     |                                |
 *                     |                                |
 *                     |                                |
 *                     |                                |
 *                     |                                |
 *                     |                                |
 *                     |                                |
 *   reserved_dram_end +--------------------------------+
 *                     |                                |
 *                     |      cbfs_cache (if reqd)      |
 *                     |        (CBFS_CACHE_SIZE)       |
 *                     +--------------------------------+ VERSTAGE_ADDR + VERSTAGE_SIZE + PRERAM_CBMEM_CONSOLE_SIZE
 *                     |      Preram CBMEM console      |
 *                     |   (PRERAM_CBMEM_CONSOLE_SIZE)  |
 *                     +--------------------------------+ VERSTAGE_ADDR + VERSTAGE_SIZE
 *                     |                                |
 *                     |       verstage (if reqd)       |
 *                     |          (VERSTAGE_SIZE)       |
 *                     +--------------------------------+ VERSTAGE_ADDR
 *                     |                                |
 *                     |            FSP-M               |
 *                     |         (FSP_M_SIZE)           |
 *                     +--------------------------------+ FSP_M_ADDR
 *                     |           romstage             |
 *                     |        (ROMSTAGE_SIZE)         |
 *                     +--------------------------------+ ROMSTAGE_ADDR = BOOTBLOCK_END
 *                     |                                | X86_RESET_VECTOR = BOOTBLOCK_END - 0x10
 *                     |           bootblock            |
 *                     |     (C_ENV_BOOTBLOCK_SIZE)     |
 *                     +--------------------------------+ BOOTBLOCK_ADDR = BOOTBLOCK_END - C_ENV_BOOTBLOCK_SIZE
 *                     |          Unused hole           |
 *                     +--------------------------------+
 *                     |     FMAP cache (FMAP_SIZE)     |
 *                     +--------------------------------+
 *                     | CBFS mcache (CBFS_MCACHE_SIZE) |
 *                     +--------------------------------+ PSP_SHAREDMEM_BASE + 0x40 + VB2_FIRMWARE_WORKBUF_RECOMMENDED_SIZE + PRE_X86_CBMEM_CONSOLE_SIZE + 0x200
 *                     |  Early Timestamp region (512B) |
 *                     +--------------------------------+ PSP_SHAREDMEM_BASE + 0x40 + VB2_FIRMWARE_WORKBUF_RECOMMENDED_SIZE + PRE_X86_CBMEM_CONSOLE_SIZE
 *                     |   PSP Verstage CBMEM console   |
 *                     |  (PRE_X86_CBMEM_CONSOLE_SIZE)  |
 *                     +--------------------------------+ PSP_SHAREDMEM_BASE + 0x40 + VB2_FIRMWARE_WORKBUF_RECOMMENDED_SIZE
 *                     |   PSP shared (vboot workbuf)   |
 *                     |(VB2_FIRMWARE_WORKBUF_RECOMMENDED_SIZE)      |
 *                     +--------------------------------+ PSP_SHAREDMEM_BASE + 0x40
 *                     |     Transfer Info Structure    |
 *                     +--------------------------------+ PSP_SHAREDMEM_BASE
 *                     |    APOB (PSP_APOB_DRAM_SIZE)   |
 *                     +--------------------------------+ PSP_APOB_DRAM_ADDRESS
 *                     |        Early BSP stack         |
 *                     |   (EARLYRAM_BSP_STACK_SIZE)    |
 * reserved_dram_start +--------------------------------+ EARLY_RESERVED_DRAM_BASE
 *                     |              DRAM              |
 *                     +--------------------------------+ 0x100000
 *                     |           Option ROM           |
 *                     +--------------------------------+ 0xc0000
 *                     |           Legacy VGA           |
 *                     +--------------------------------+ 0xa0000
 *                     |              DRAM              |
 *                     +--------------------------------+ 0x0
 */
SECTIONS
{
	DRAM_START(0x0)

	EARLY_RESERVED_DRAM_START(CONFIG_EARLY_RESERVED_DRAM_BASE)

	EARLYRAM_STACK(., CONFIG_EARLYRAM_BSP_STACK_SIZE)
	REGION(apob, CONFIG_PSP_APOB_DRAM_ADDRESS, CONFIG_PSP_APOB_DRAM_SIZE, 1)

#if CONFIG(VBOOT_STARTS_BEFORE_BOOTBLOCK)
	PSP_SHAREDMEM_DRAM_START(CONFIG_PSP_SHAREDMEM_BASE)
#endif

#include "memlayout_transfer_buffer.inc"

#if CONFIG(VBOOT_STARTS_BEFORE_BOOTBLOCK)
	PSP_SHAREDMEM_DRAM_END(CONFIG_PSP_SHAREDMEM_BASE + CONFIG_PSP_SHAREDMEM_SIZE)
#endif
	_ = ASSERT(BOOTBLOCK_END == ((BOOTBLOCK_END + 0xFFFF) & 0xFFFF0000), "Bootblock end must be 16 bit aligned");
	ROMSTAGE(CONFIG_ROMSTAGE_ADDR, CONFIG_ROMSTAGE_SIZE)

#if CONFIG(PLATFORM_USES_FSP2_0)
	REGION(fspm, CONFIG_FSP_M_ADDR, CONFIG_FSP_M_SIZE, 1)
#endif

#if CONFIG(VBOOT_SEPARATE_VERSTAGE) && !CONFIG(VBOOT_STARTS_BEFORE_BOOTBLOCK)
	VERSTAGE(CONFIG_VERSTAGE_ADDR, CONFIG_VERSTAGE_SIZE)
#endif

        PRERAM_CBMEM_CONSOLE(., CONFIG_PRERAM_CBMEM_CONSOLE_SIZE)

#if CONFIG_CBFS_CACHE_SIZE > 0
        . = ALIGN(ARCH_CACHELINE_ALIGN_SIZE);
        CBFS_CACHE(., CONFIG_CBFS_CACHE_SIZE)
#endif

	EARLY_RESERVED_DRAM_END(.)

	/* Relocated at runtime in cbmem so the address does not matter. */
	RAMSTAGE(512M, 8M)
}

#if ENV_BOOTBLOCK

gdtptr_offset = gdtptr & 0xffff;
nullidt_offset = nullidt & 0xffff;

MEMORY
{
	/* Make sure all the 16bit code is in the same 64K segment as the reset vector */
	resetsection (rwx) : ORIGIN = BOOTBLOCK_END - 64K, LENGTH = 64K
}

SECTIONS {
	/* Page tables need to be at a 4K boundary so align the bootblock downwards */
	. = (BOOTBLOCK_TOP - PROGRAM_SZ) & ~(4096 - 1);
	_bootblock = .;

	INCLUDE "bootblock/lib/program.ld"

	PROGRAM_SZ = SIZEOF(.text) + SIZEOF(.bss) + SIZEOF(.data);

	. = _X86_RESET_VECTOR - EARLYASM_SZ;
	. = ALIGN(16);
	BOOTBLOCK_TOP = .;
	.init BOOTBLOCK_TOP : {
		*(.init._start);
		*(.init);
		*(.init.*);
	} >resetsection

	/*
	 * Allocation reserves extra space here. Alignment requirements
	 * may cause the total size of a section to change when the start
	 * address gets applied.
	 */
	EARLYASM_SZ = SIZEOF(.init) + 16;

	_X86_RESET_VECTOR = BOOTBLOCK_END - 0x10;
	_bogus = ASSERT((_X86_RESET_VECTOR & 0xffff) == 0xfff0, "IP needs to be 0xfff0");
	.reset (_X86_RESET_VECTOR) : {
		*(.reset);
	} >resetsection
	.last_byte (BOOTBLOCK_END - 1) : {
		BYTE(0xff);
	} >resetsection
	_ebootblock = BOOTBLOCK_END;
}
#endif  /* ENV_BOOTBLOCK */
