/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stddef.h>
#include <stdint.h>

#include <nyetpci.h>
#include <build/debug.h>

/*
 * Check if a BAR is an I/O port
 *
 * Returns:
 *   1 - Yes
 *   0 - No
 */
int
nyetpci_bar_is_ioport(uint32_t bar)
{
	return ((bar & 0x01) != 0);
}

/*
 * Check if a BAR is 64-bit
 *
 * Returns:
 *   1 - Yes
 *   0 - No
 */
int
nyetpci_bar_is_64bit(uint32_t bar)
{
	return ((bar & 0x06) == 0x04);
}

/*
 * Check if a BAR is prefetchable
 *
 * Returns:
 *   1 - Yes
 *   0 - No
 */
int
nyetpci_bar_is_prefetchable(uint32_t bar)
{
	if (nyetpci_bar_is_ioport(bar)) {
		return (0);
	}

	return ((bar & 0x08) != 0);
}


/*
 * Get the physical address of a BAR
 *
 * Returns:
 *   0 - Error
 *  >0 - Address
 *
 * Takes and writes to a pointer to a length variable.
 * If the pointer is NULL, do not size the BAR.
 */
uint64_t
nyetpci_bar_get_paddr(struct nyetpci_ctx *pci, int reg, uint64_t *len)
{
	if (!(	reg == 0x10 ||
		reg == 0x14 ||
		reg == 0x18 ||
		reg == 0x1C ||
		reg == 0x20 ||
		reg == 0x24 ||
		reg == 0x30 )) {

		LOG("Incorrect register provided");
		return (0);
	}

	uint32_t tmp;
	uint32_t bar_save;

	/* Need different mask for 0x30 */
	uint32_t base_mask = ((reg == 0x30) ? ~0x7FF : ~0xF);

	if (nyetpci_cfg_read(pci, reg, 4, &bar_save) < 0) {
		return (0);
	}

	if (bar_save == 0 || bar_save == UINT32_MAX) {
		LOG("BAR is empty");
		return (0);
	}

	int is_64 = nyetpci_bar_is_64bit(bar_save);

	/* ROM BAR is never 64-bit */
	if (is_64 && reg == 0x30) {
		LOG("ROM BAR is never 64-bit");
		return (0);
	}

	uint32_t upper_save;
	uint64_t bar_base;

	if (is_64) {
		LOG("64-bit BAR");
		if (nyetpci_cfg_read(pci, reg + 4, 4, &upper_save)) {
			return (0);
		}

		bar_base = ((uint64_t)upper_save << 32) |
			(uint64_t)(bar_save & base_mask);
	} else {
		bar_base = bar_save & base_mask;
	}

	if (len == NULL) {
		LOG("Not measuring the BAR");
		LOGV("BAR 0x%02X B:0x%02lX L:N/A", reg, bar_base);
		return (bar_base);
	}

	/*
	 * Disable memory and IO space
	 */
	LOG("Disabling BAR IO");
	if (nyetpci_cfg_read(pci, 0x04, 4, &tmp) < 0) {
		return (0);
	}

	tmp &= ~0x03;

	if (nyetpci_cfg_write(pci, 0x04, 4, &tmp) < 0) {
		return (0);
	}

	/*
	 * Get the size of the BAR
	 */
	uint64_t size_mask;
	tmp = UINT32_MAX;
	if (is_64) {
		uint32_t lo, hi;

		if ((nyetpci_cfg_write(pci, reg, 4, &tmp) < 0) ||
			(nyetpci_cfg_write(pci, reg + 4, 4, &tmp) < 0)) {

			bar_base = 0;
			goto cleanup_io;
		} else if ((nyetpci_cfg_read(pci, reg, 4, &lo) < 0) ||
			(nyetpci_cfg_read(pci, reg + 4, 4, &hi) < 0)) {

			bar_base = 0;
			goto cleanup_bar;
		}

		lo &= base_mask;
		size_mask = ((uint64_t)hi << 32) | (uint64_t)lo;
	} else {
		if (nyetpci_cfg_write(pci, reg, 4, &tmp) < 0) {
			bar_base = 0;
			goto cleanup_io;
		} else if (nyetpci_cfg_read(pci, reg, 4, &tmp) < 0) {
			bar_base = 0;
			goto cleanup_bar;
		}

		tmp &= base_mask;
		size_mask = (uint64_t)tmp;
	}

	/* Restore BAR and re-enable memory/IO */
cleanup_bar:
	LOG("Cleaning up the BAR");
	if (nyetpci_cfg_write(pci, reg, 4, &bar_save) < 0) {
		bar_base = 0;
	} else if (is_64) {
		if (nyetpci_cfg_write(pci, reg + 4, 4, &upper_save) < 0) {
			bar_base = 0;
		}
	}

cleanup_io:
	LOG("Enabling BAR IO");
	if (nyetpci_cfg_read(pci, 0x04, 4, &tmp) < 0) {
		/* Early return to not write garbage to reg 4 */
		return (0);
	}

	tmp |= 0x03;

	if (nyetpci_cfg_write(pci, 0x04, 4, &tmp) < 0) {
		bar_base = 0;
	}

	if (size_mask == 0) {
		return (0);
	}

	uint64_t len_calc = (~size_mask) + 1;
	*len = (is_64 ? len_calc : len_calc & UINT32_MAX);
	LOGV("BAR 0x%02X Base 0x%02lX Length 0x%02lX", reg, bar_base, *len);
	return (bar_base);
}
