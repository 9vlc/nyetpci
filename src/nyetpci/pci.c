/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stdint.h>

#include <nyetpci.h>
#include <build/debug.h>

/*
 * Check if a PCI device is a graphics adapter
 *
 * Returns:
 *   1 - Yes
 *   0 - No
 *  -1 - Error
 */
int
nyetpci_is_gpu(struct nyetpci_ctx *pci)
{
	uint32_t r2;

	if (nyetpci_cfg_read(pci, 0x08, 4, &r2) < 0) {
		return (-1);
	} else {
		return ((r2 >> 24) == 0x03);
	}
}
