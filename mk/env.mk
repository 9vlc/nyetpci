#
# Toolchain
#
CC?= cc
AS?= as
LD?= ld
AR?= ar
NM?= nm
STRIP?= strip
RANLIB?= ranlib
READELF?= readelf
OBJDUMP?= objdump
OBJCOPY?= objcopy

#
# Directories
#
OBJDIR:= ${PROJDIR}/build
INCDIR:= ${PROJDIR}/include
MKDIR:= ${PROJDIR}/mk
SRCDIR:= ${PROJDIR}/src

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
.if defined(DEBUG) && ${DEBUG} == 1
CFLAGS:= -DDEBUG ${CFLAGS}
.endif
