PROJDIR:= ${.PARSEDIR}
OBJS_ALL:=
EXTRAS_ALL:=

.include "mk/env.mk"
.include "src/dirs.mk"

.if !make(clean)
.BEGIN:
	@mkdir -p ${OBJS_ALL:H:O:u} ${EXTRAS_ALL:H:O:u}
.endif

build: ${OBJS_ALL} ${EXTRAS_ALL}

clean:
.if exists(${OBJDIR})
	rm -rf ${OBJDIR}
.endif

.PHONY: build clean
.MAIN: build
