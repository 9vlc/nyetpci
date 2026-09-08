/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/pciio.h>

#include <errno.h>
#include <stdint.h>

#include <nyetpci.h>

int
nyetpci_device_exists(struct nyetpci_ctx *pci)
{
	struct pci_io io = {
		.pi_sel = {
			pci->sel.dom,
			pci->sel.bus,
			pci->sel.dev,
			pci->sel.func
		},
		.pi_reg = 0,
		.pi_width = 4,
	};

	if (ioctl(pci->plat.pcifd, PCIOCREAD, &io) < 0) {
		return (0);
	}

	return (1);
}
