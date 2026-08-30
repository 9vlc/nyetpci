/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <nyetpci.h>

/*
 * Parse a PCI selector
 * Adapted from FreeBSD's /usr/src/usr.sbin/pciconf/pciconf.c
 *
 * Returns:
 *   0 - Success
 *  -1 - Failure
 */
int
nyetpci_parse_sel(const char *str, struct nyetpci_sel *sel)
{
	const char *ep;
	char *eppos;
	unsigned long selarr[4];
	int i;

	ep = strchr(str, '@');
	if (ep != NULL) {
		ep++;
	} else {
		ep = str;
	}

	if (strncmp(ep, "pci", 3) == 0) {
		ep += 3;
		i = 0;

		while (isdigit((unsigned char)*ep) && i < 4) {
			selarr[i++] = strtoul(ep, &eppos, 10);
			ep = eppos;

			if (*ep == ':') {
				ep++;
			}
		}

		if (i > 0 && *ep == '\0') {
			sel->func = ((i > 2) ? selarr[--i] : 0);
			sel->dev = ((i > 0) ? selarr[--i] : 0);
			sel->bus = ((i > 0) ? selarr[--i] : 0);
			sel->domain = ((i > 0) ? selarr[--i] : 0);

			return (0);
		}
	}

	return (-1);
}

