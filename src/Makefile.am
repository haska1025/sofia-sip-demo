bin_PROGRAMS=sip_dmo

sip_dmo_SOURCES=sofia_sip_demo.c

sip_dmo_CFLAGS=$(SOFIASIP_CFLAGS)
#sip_dmo_LDFLAGS=$(SOFIASIP_LIBS)
sip_dmo_LDADD=$(SOFIASIP_LIBS)

clean-local:
	@rm -f *.gc* gmon.out
