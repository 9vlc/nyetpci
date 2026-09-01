.if ${PLAT_REAL} == "METAL_CAM"
SRCS:= metal/cam/io.c metal/cam/pci.c dummy/ctx.c

#.elif ${PLAT_REAL} == "METAL_ECAM"
#.  if !defined(NYETPCI_ECAM_BASE)
#.    error "Must provide NYETPCI_ECAM_BASE to compile for ECAM!"
#.  endif
#SRCS:= metal/ecam/io.c metal/ecam/pci.c dummy/ctx.c

.elif ${PLAT_REAL} == "FREEBSD"
SRCS:= freebsd/io.c freebsd/pci.c freebsd/ctx.c

.elif ${PLAT_REAL} == "DUMMY"
SRCS:= dummy/io.c dummy/pci.c dummy/ctx.c

.else
.  error "Unknown platform: ${PLAT_REAL}"
.endif

.include "${MBDIR}/gen.mk"
