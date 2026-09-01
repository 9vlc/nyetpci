#
# Platform
#
.if defined(NYETPCI_PLAT)
PLAT_REAL:= ${NYETPCI_PLAT:tu}
.else
PLAT_REAL:= ${.MAKE.OS:tu}
.endif

#
# Toolchain flags
#
CFLAGS:= ${CFLAGS_ADD} -DNYETPCI_PLAT_${PLAT_REAL} -I${INCDIR} -Wall -Wextra -pedantic -std=c99 -Os -fPIC
LDFLAGS:= ${LDFLAGS_ADD} -L${OBJDIR}/lib -s
