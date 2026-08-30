#
# Include extra build instructions from sub files
#

_i:= ${.INCLUDEDFROMDIR}
_es:= ${EXTRAS}
EXTRAS:=

.for _extra in ${_es}
EXTRAS:= ${EXTRAS} ${OBJDIR}/${_i:T}/${_extra}
.endfor
EXTRAS_ALL:= ${EXTRAS_ALL} ${EXTRAS}
EXTRAS.${_i:T}:= ${EXTRAS}

EXTRAS:=
