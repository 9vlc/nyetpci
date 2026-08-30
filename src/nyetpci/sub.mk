SRCS:= pci.c bar.c find.c
.if ${PLAT_REAL} != "METAL_CAM" && ${PLAT_REAL} != "METAL_ECAM"
SRCS+= etc.c
.endif

.include "../../mk/gen.mk"
