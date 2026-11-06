# Makefile Para Holotz's Castle. (C) Juan Carlos Seijo Pérez - 2004.
# Makefile for Holotz's Castle.

HC_VERSION=1.3.14

all: 
	$(MAKE) -C src

distro-src:
	find . -iname "*~" -exec 'rm' '{}' ';';
	$(MAKE) clean;
	rm -rf _DISTRO/SRC;
	mkdir -p _DISTRO/SRC/holotz-castle-$(HC_VERSION)-src;\
	cp -RL [^_h]* _DISTRO/SRC/holotz-castle-$(HC_VERSION)-src &&\
	cd _DISTRO/SRC &&\
	tar -czvf holotz-castle-$(HC_VERSION)-src.tar.gz holotz-castle-$(HC_VERSION)-src &&\
	cd ../.. &&\
	rm -rf _DISTRO/SRC/holotz-castle-$(HC_VERSION)-src\

distro-i486:
	find . -iname "*~" -exec 'rm' '{}' ';';
	$(MAKE) I486_BUILD=1 CPU_OPTS="-march=i486" &&\
	rm -rf _DISTRO/I486;
	mkdir -p _DISTRO/I486/holotz-castle-$(HC_VERSION)-linux-i486;
	cp -RL holotz-castle holotz-castle-editor res HCedHome doc LICENSE.txt man _DISTRO/I486/holotz-castle-$(HC_VERSION)-linux-i486 &&\
	cd _DISTRO/I486 &&\
	tar -czvf holotz-castle-$(HC_VERSION)-linux-i486.tar.gz holotz-castle-$(HC_VERSION)-linux-i486;
	rm -rf _DISTRO/I486/holotz-castle-$(HC_VERSION)-linux-i486;

distro-win32:
	find . -iname "*~" -exec 'rm' '{}' ';';
	rm -rf _DISTRO/WIN32;
	mkdir -p _DISTRO/WIN32/HC;
	cp -RL _WIN32_DLLS/* holotz-castle.exe holotz-castle-editor.exe res HCedHome LICENSE.txt doc _DISTRO/WIN32/HC &&\
	cd _DISTRO/WIN32 &&\
	zip -r HC HC;
	mv _DISTRO/WIN32/HC.zip _DISTRO/WIN32/holotz-castle-$(HC_VERSION)-win32.zip;
	rm -rf _DISTRO/WIN32/HC;

install:
	$(MAKE) -C src install

uninstall:
	$(MAKE) -C src uninstall;

.PHONY: clean
clean:
	$(MAKE) -C src clean
