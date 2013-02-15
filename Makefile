# type either "make linux", "make win32", or "make os2" to compile

help:
	@echo You can use this Makefile in the following ways:
	@echo make linux ............ Make Linux binaries
	@echo make win32 ............ Make Win32 binaries
	@echo make os2 .............. Make OS/2 binaries
	@echo make cleanlinux ....... Remove object files under Linux
	@echo make cleanwin32 ....... Remove object files under Win32
	@echo make cleanos2 ......... Remove object files under OS/2
	@echo make tests ............ Run Tests (requires /bin/sh)

linux :
	mkdir -p bin
	make -C src -f Makefile linux

win32 :
	make -C src -f Makefile win32

os2 :
	make -C src -f Makefile os2

cleanlinux :
	rm -rf bin
	make -C src -f Makefile cleanlinux

cleanwin32 : 
	make -C src -f Makefile cleanwin32

cleanos2 : 
	make -C src -f Makefile cleanos2

.PHONY: tests
tests: linux
	make -C tests

