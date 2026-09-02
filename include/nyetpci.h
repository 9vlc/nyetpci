/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _NYETPCI_H
#define _NYETPCI_H

#include <stdint.h>

/*
 * Stub the NYETPCI_PLAT_ stuff
 */
#if defined(__FreeBSD__)
#  ifndef NYETPCI_PLAT_FREEBSD
#    define NYETPCI_PLAT_FREEBSD
#  endif
#endif

/*
 * Stuff some platforms might want
 */
struct nyetpci_plat {
#if defined(NYETPCI_PLAT_FREEBSD)
	int pcifd;
#else
	int _dummy;
#endif
};

/*
 * Some structs
 */

/* PCI device selector */
struct nyetpci_sel {
	uint16_t domain;
	uint8_t bus;
	uint8_t dev;
	uint8_t func;
};

/* Context passed to functions */
struct nyetpci_ctx {
	struct nyetpci_plat plat;
	struct nyetpci_sel sel;
};

/* Filter for pci_find_devices() */
struct nyetpci_filter {
	uint16_t vendor;
	uint16_t device;
	uint16_t subvendor;
	uint16_t subdevice;
	uint32_t class;
	uint32_t subclass;
};

/* Flags for pci_find_devices() */
#define NYETPCI_FILTER_VENDOR		((uint32_t)1 << 0)
#define NYETPCI_FILTER_DEVICE		((uint32_t)1 << 1)
#define NYETPCI_FILTER_SUBVENDOR	((uint32_t)1 << 2)
#define NYETPCI_FILTER_SUBDEVICE	((uint32_t)1 << 3)
#define NYETPCI_FILTER_CLASS		((uint32_t)1 << 4)
#define NYETPCI_FILTER_SUBCLASS		((uint32_t)1 << 5)
#define NYETPCI_FILTER_ANYTHING		((uint32_t)1 << 6)

/*
 * Provided by platform
 */
int nyetpci_init(struct nyetpci_ctx *pci);
int nyetpci_free(struct nyetpci_ctx *pci);
int nyetpci_cfg_read(struct nyetpci_ctx *pci, int reg, int width,
	uint32_t *val);
int nyetpci_cfg_write(struct nyetpci_ctx *pci, int reg, int width,
	uint32_t *val);
int nyetpci_device_exists(struct nyetpci_ctx *pci);

/* Not platform dependent but for now guarded because of libc */
#if !defined(NYETPCI_PLAT_METAL_CAM) && !defined(NYETPCI_PLAT_METAL_ECAM)
int nyetpci_parse_sel(const char *str, struct nyetpci_sel *sel);
#endif


/*
 * Platform-independent
 */
int nyetpci_is_gpu(struct nyetpci_ctx *pci);

int nyetpci_get_vendor(struct nyetpci_ctx *pci, uint16_t *vendor);
int nyetpci_get_device(struct nyetpci_ctx *pci, uint16_t *device);
int nyetpci_get_subvendor(struct nyetpci_ctx *pci, uint16_t *subvendor);
int nyetpci_get_subdevice(struct nyetpci_ctx *pci, uint16_t *subdevice);

int nyetpci_bar_is_ioport(uint32_t bar);
int nyetpci_bar_is_64bit(uint32_t bar);
int nyetpci_bar_is_prefetchable(uint32_t bar);
uint64_t nyetpci_bar_get_paddr(struct nyetpci_ctx *pci, int reg,
	uint64_t *len);

uint32_t nyetpci_find_devices(struct nyetpci_ctx *pci,
	struct nyetpci_filter *filt, int flags,
	struct nyetpci_sel *matches, uint32_t matches_len);

#endif /* _NYETPCI_H */
