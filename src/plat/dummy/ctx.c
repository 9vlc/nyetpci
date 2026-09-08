/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

#include <nyetpci.h>

int
nyetpci_init(struct nyetpci_ctx *pci)
{
	(void)pci;
	return (0);
}

int
nyetpci_free(struct nyetpci_ctx *pci)
{
	(void)pci;
	return (0);
}
