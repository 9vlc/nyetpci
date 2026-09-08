/*
 * SPDX-License-Identifier: BSD-2-Clause
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
	}

	return ((r2 >> 24) == 0x03);
}

/*
 * Get the vendor, device, subvendor, subdevice
 *
 * Returns:
 *   0 - Success
 *  -1 - Error
 */

int
nyetpci_get_vendor(struct nyetpci_ctx *pci, uint16_t *vendor)
{
	uint32_t t;

	if (nyetpci_cfg_read(pci, 0x00, 2, &t) < 0) {
		return (-1);
	}

	*vendor = (uint16_t)t;
	return (0);
}

int
nyetpci_get_device(struct nyetpci_ctx *pci, uint16_t *device)
{
	uint32_t t;

	if (nyetpci_cfg_read(pci, 0x02, 2, &t) < 0) {
		return (-1);
	}

	*device = (uint16_t)t;
	return (0);
}

int
nyetpci_get_subvendor(struct nyetpci_ctx *pci, uint16_t *subvendor)
{
	uint32_t t;

	if (nyetpci_cfg_read(pci, 0x2C, 2, &t) < 0) {
		return (-1);
	}

	*subvendor = (uint16_t)t;
	return (0);
}

int
nyetpci_get_subdevice(struct nyetpci_ctx *pci, uint16_t *subdevice)
{
	uint32_t t;

	if (nyetpci_cfg_read(pci, 0x2E, 2, &t) < 0) {
		return (-1);
	}

	*subdevice = (uint16_t)t;
	return (0);
}
