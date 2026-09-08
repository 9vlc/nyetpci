/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <nyetpci.h>

int
nyetpci_device_exists(struct nyetpci_ctx *pci)
{
	uint32_t val;
	if ((nyetpci_cfg_read(pci, 0x00, 4, &val) < 0) || val == 0 ||
		val == UINT32_MAX) {

		return (0);
	}

	return (1);
}
