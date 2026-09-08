/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stdint.h>

#include <nyetpci.h>
#include <build/debug.h>

/*
 * Find PCI devices
 *
 * Returns:
 * >=0 - Number of devices found
 *  -1 - Error
 */
uint32_t
nyetpci_find_devices(struct nyetpci_ctx *pci, struct nyetpci_filter *filt,
	int flags, struct nyetpci_sel *matches, uint32_t matches_len)
{
	struct nyetpci_ctx p = *pci;
	uint32_t matches_num = 0;
	int match_ok = 1;

	int bus_count = 1;
	uint8_t bus_queue[256] = {0};

	for (int bus = 0; bus < bus_count; bus++) {
		p.sel.bus = bus_queue[bus];

		for (p.sel.dev = 0; p.sel.dev < 32; p.sel.dev++) {
			p.sel.func = 0;

			if (!nyetpci_device_exists(&p)) {
				continue;
			}

			uint32_t header_type;
			if (nyetpci_cfg_read(&p, 0x0E, 1, &header_type) < 0) {
				continue;
			}
			uint8_t func_max = ((header_type & 0x80) ? 8 : 1);

			for (p.sel.func = 0; p.sel.func < func_max; p.sel.func++) {
				match_ok = 1;

				if (!nyetpci_device_exists(&p)) {
					continue;
				}

				uint32_t
					vendor,
					device,
					subvendor,
					subdevice,
					class,
					subclass,
					header_type;

#define GET16(WHAT, ADDR) \
	if (nyetpci_cfg_read(&p, (ADDR), 2, &(WHAT)) < 0) { \
		continue; \
	}

#define GET8(WHAT, ADDR) \
	if (nyetpci_cfg_read(&p, (ADDR), 1, &(WHAT)) < 0) { \
		continue; \
	}
				GET16(vendor, 0x00);
				GET16(device, 0x02);
				GET16(subvendor, 0x2C);
				GET16(subdevice, 0x2E);

				GET8(class, 0x0B);
				GET8(subclass, 0x0A);
				GET8(header_type, 0x0E);
#undef GET16

#define CHK(FLAG, WHAT) do { \
	if (flags & (FLAG) && (WHAT) != filt->WHAT) { \
		match_ok = 0; \
	} \
} while (0)
				CHK(NYETPCI_FILTER_VENDOR, vendor);
				CHK(NYETPCI_FILTER_DEVICE, device);
				CHK(NYETPCI_FILTER_CLASS, class);
				CHK(NYETPCI_FILTER_SUBCLASS, subclass);

				if (flags & NYETPCI_FILTER_ANYTHING) {
					match_ok = 1;
					LOGV("Found a device at pci0:%u:%u:%u",
						p.sel.bus, p.sel.dev, p.sel.func);
				}

				if ((header_type & 0x7F) == 0x00) {
					CHK(NYETPCI_FILTER_SUBVENDOR, subvendor);
					CHK(NYETPCI_FILTER_SUBDEVICE, subdevice);
				}
#undef CHK
				if ((header_type & 0x7F) == 0x01) {
					uint32_t b;
					GET8(b, 0x19);
					if (b != 0 && bus_count < 256) {
						bus_queue[bus_count++] = (uint8_t)b;
					}
				}
#undef GET8
				if (match_ok) {
					if (matches_num < matches_len) {
						matches[matches_num] = p.sel;
					} else {
						return (matches_len + 1);
					}
					matches_num++;
				}
			}
		}
	}

	return (matches_num);
}
