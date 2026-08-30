/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <nyetpci.h>
#include <build/debug.h>

int
nyetpci_init(struct nyetpci_ctx *pci)
{
	if ((pci->plat.pcifd = open("/dev/pci", O_RDWR)) < 0) {
		LOGV("open() threw errno %d", errno);
		return (-1);
	}

	return (0);
}

int
nyetpci_free(struct nyetpci_ctx *pci)
{
	if (pci->plat.pcifd >= 0) {
		if (close(pci->plat.pcifd) < 0) {
			LOGV("open() threw errno %d", errno);
			return (-1);
		}
	}

	return (0);
}
