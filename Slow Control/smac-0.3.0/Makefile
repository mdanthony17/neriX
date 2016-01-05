VERSION := $(shell grep -P "^version\s*=\s*'\d+\.\d+\.\d+'" smacd | grep -P -o '\d+\.\d+\.\d+')
DISTFILES = Makefile README smacd smacd.conf smac
DISTDIR = smac-$(VERSION)

dist:
	@if test -d $(DISTDIR); then rm -r $(DISTDIR); fi
	@mkdir $(DISTDIR)
	@cp $(DISTFILES) $(DISTDIR)
	@tar zcf $(DISTDIR).tar.gz $(DISTDIR)
	@rm -r $(DISTDIR)

install:
	@install -m 644 smacd.conf /etc/smacd.conf
	@install -m 755 smacd /etc/init.d/smacd

