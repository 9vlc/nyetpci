/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 *
 * Dummy platform implementation
 */

#include <stdint.h>

#include <nyetpci.h>

/*
 * Read from the config space
 */
int
nyetpci_cfg_read(struct nyetpci_ctx *pci, int reg, int width, uint32_t *val)
{
	(void)pci;
	(void)reg;
	(void)width;
	(void)val;
	return (-1);
}

/*
 * Write to the config space
 */
int
nyetpci_cfg_write(struct nyetpci_ctx *pci, int reg, int width, uint32_t *val)
{
	(void)pci;
	(void)reg;
	(void)width;
	(void)val;
	return (-1);
}
