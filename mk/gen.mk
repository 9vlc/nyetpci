_i:= ${.INCLUDEDFROMDIR}
OBJS:=

.for _srcfile in ${SRCS}
_o:= ${_srcfile:R}.o

.  if ${_srcfile:E} == c
${OBJDIR}/${_i:T}/${_o}: ${_i}/${_srcfile}
	${CC} ${CFLAGS} -c -o $@ $>
.  else
.    error Unknown source file extension: "${_srcfile:E}"
.  endif

OBJS:= ${OBJS} ${OBJDIR}/${_i:T}/${_o}
.endfor
SRCS:=
OBJS_ALL:= ${OBJS_ALL} ${OBJS}
OBJS.${_i:T}:= ${OBJS}
