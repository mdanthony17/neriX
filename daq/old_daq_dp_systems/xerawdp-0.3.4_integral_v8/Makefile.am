SUBDIRS = src scripts models
dist_doc_DATA = README
dist_pkgdata_DATA = xenon100-default.xml
install-data-hook:
	cd $(DESTDIR)$(datadir) && \
	mv -f $(PACKAGE) $(PACKAGE)-$(VERSION) && \
	$(LN_S) $(PACKAGE)-$(VERSION) $(PACKAGE)
