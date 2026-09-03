EXTRAS:= nyetpci.a
.if ${NYETPCI_PIC} == 1
EXTRAS+= nyetpci.so
.endif

.include "${MBDIR}/extras.mk"

${SUBOBJDIR}/nyetpci.a: ${OBJS_ALL}
	${AR} rcs $@ $>
	${RANLIB} $@

.if ${NYETPCI_PIC} == 1
${SUBOBJDIR}/nyetpci.so: ${OBJS_ALL}
	${LD} ${LDFLAGS} -shared -o $@ $>
.endif
