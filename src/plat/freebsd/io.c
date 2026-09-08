/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/pciio.h>

#include <nyetpci.h>
#include <build/debug.h>

int
nyetpci_cfg_read(struct nyetpci_ctx *pci, int reg, int width,
	uint32_t *val)
{
	if ((width != 1 && width != 2 && width != 4) || (reg % width) != 0) {
		LOG("Invalid width or unaligned register");
		return (-1);
	}

	struct pci_io io = {
		.pi_sel = {
			pci->sel.dom,
			pci->sel.bus,
			pci->sel.dev,
			pci->sel.func
		},
		.pi_reg = reg,
		.pi_width = width,
	};

	if (ioctl(pci->plat.pcifd, PCIOCREAD, &io) < 0) {
		LOGV("ioctl(PCIOCREAD) on pci%u:%u:%u:%u @ 0x%02X W%d threw %d",
			pci->sel.dom, pci->sel.bus, pci->sel.dev,
			pci->sel.func, reg, width, errno);
		return (-1);
	}

	*val = io.pi_data;
	return (0);
}

int
nyetpci_cfg_write(struct nyetpci_ctx *pci, int reg, int width,
	uint32_t *val)
{
	if ((width != 1 && width != 2 && width != 4) || (reg % width) != 0) {
		LOG("Invalid width or unaligned register");
		return (-1);
	}

	struct pci_io io = {
		.pi_sel = {
			pci->sel.dom,
			pci->sel.bus,
			pci->sel.dev,
			pci->sel.func
		},
		.pi_reg = reg,
		.pi_width = width,
		.pi_data = *val
	};

	if (ioctl(pci->plat.pcifd, PCIOCWRITE, &io) < 0) {
		LOGV("ioctl(PCIOCWRITE) on pci%u:%u:%u:%u @ 0x%02X W%d threw %d",
			pci->sel.dom, pci->sel.bus, pci->sel.dev,
			pci->sel.func, reg, width, errno);
		return (-1);
	}

	return (0);
}
