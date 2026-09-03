#
# Platform
#
.if defined(NYETPCI_PLAT)
PLAT_REAL:= ${NYETPCI_PLAT:tu}
.else
PLAT_REAL:= ${.MAKE.OS:tu}
.endif

NYETPCI_PIC?= 1

#
# Toolchain flags
#
CFLAGS:= ${CFLAGS_ADD} -DNYETPCI_PLAT_${PLAT_REAL} -I${INCDIR} -Wall -Wextra -pedantic -std=c99 -Os
LDFLAGS:= ${LDFLAGS_ADD} -L${OBJDIR}/lib -s

.if ${NYETPCI_PIC} == 1
CFLAGS+= -fPIC
.else
CFLAGS+= -fno-pic
.endif
