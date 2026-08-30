/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stdint.h>

#include <nyetpci.h>
#include <build/debug.h>

#define A_PCI_CAM_CA 0xCF8 /* Write to select address */
#define A_PCI_CAM_CD 0xCFC /* Read/write to access it */

/*
 * Convert enable,bus,device,function,register to an IO port access value
 * Send it to 0xCF8 / 0xCFC
 */
#define PCI_CAM_QUERY(ENABLE, BUS, DEV, FUNC, REG) ( \
		(((uint32_t)(ENABLE) & 0x01) << 31) | \
		(((uint32_t)(BUS) & 0xFF) << 16) | \
		(((uint32_t)(DEV) & 0x1F) << 11) | \
		(((uint32_t)(FUNC) & 0x07) << 8) | \
		((uint32_t)(REG) & 0xFC) \
	)

static inline uint8_t
_inb(uint16_t port)
{
	uint8_t ret;
	__asm__ __volatile__("inb %w1, %0" : "=a"(ret) : "Nd"(port));
	return (ret);
}

static inline void
_outb(uint16_t port, uint8_t byte)
{
	__asm__ __volatile__("outb %0, %w1" :: "a"(byte), "Nd"(port));
}

static inline uint16_t
_inw(uint16_t port)
{
	uint16_t ret;
	__asm__ __volatile__("inw %w1, %0" : "=a"(ret) : "Nd"(port));
	return (ret);
}

static inline void
_outw(uint16_t port, uint16_t word)
{
	__asm__ __volatile__("outw %0, %w1" :: "a"(word), "Nd"(port));
}

static inline uint32_t
_inl(uint16_t port)
{
	uint32_t ret;
	__asm__ __volatile__("inl %w1, %0" : "=a"(ret) : "Nd"(port));
	return (ret);
}

static inline void
_outl(uint16_t port, uint32_t dword)
{
	__asm__ __volatile__("outl %0, %w1" :: "a"(dword), "Nd"(port));
}

int
nyetpci_cfg_read(struct nyetpci_ctx *pci, int reg, int width, uint32_t *val)
{
	if ((width != 1 && width != 2 && width != 4) || (reg % width) != 0) {
		LOG("Invalid width or unaligned register");
		return (-1);
	}

	if (pci->sel.domain != 0) {
		LOG("CAM can only access domain 0");
		return (-1);
	}

	_outl(A_PCI_CAM_CA, PCI_CAM_QUERY(
		1, pci->sel.bus, pci->sel.dev, pci->sel.func, reg
	));

	switch (width) {
	case 1:
		*val = (_inb(A_PCI_CAM_CD + (reg & 3)));
		break;

	case 2:
		*val = (_inw(A_PCI_CAM_CD + (reg & 2)));
		break;

	default: /* 4 */
		*val = (_inl(A_PCI_CAM_CD));
	}

	return (0);
}

int
nyetpci_cfg_write(struct nyetpci_ctx *pci, int reg, int width, uint32_t *val)
{
	if ((width != 1 && width != 2 && width != 4) || (reg % width) != 0) {
		LOG("Invalid width or unaligned register");
		return (-1);
	}

	if (pci->sel.domain != 0) {
		LOG("CAM can only access domain 0");
		return (-1);
	}

	_outl(A_PCI_CAM_CA, PCI_CAM_QUERY(
		1, pci->sel.bus, pci->sel.dev, pci->sel.func, reg
	));

	switch (width) {
	case 1:
		_outb(A_PCI_CAM_CD + (reg & 3), (uint8_t)*val);
		break;

	case 2:
		_outw(A_PCI_CAM_CD + (reg & 2), (uint16_t)*val);
		break;

	default: /* 4 */
		_outl(A_PCI_CAM_CD, (uint32_t)*val);
	}

	return (0);
}
