SUBDIRS:= \
	  nyetpci \
	  plat \
	  lib

.for _subdir in ${SUBDIRS}
SUBDIR:= ${PROJDIR}/src/${_subdir}
SUBOBJDIR:= ${OBJDIR}/${_subdir}
SUBDIR.${_subdir}:= ${SUBDIR}
SUBOBJDIR.${_subdir}:= ${SUBOBJDIR}
.  include "${_subdir}/sub.mk"
.endfor
