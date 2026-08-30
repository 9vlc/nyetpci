EXTRAS:= \
	nyetpci.so \
	nyetpci.a

.include "../../mk/extras.mk"

${SUBOBJDIR}/nyetpci.a: ${OBJS_ALL}
	${AR} rcs $@ $>
	${RANLIB} $@

${SUBOBJDIR}/nyetpci.so: ${OBJS_ALL}
	${LD} ${LDFLAGS} -shared -o $@ $>
