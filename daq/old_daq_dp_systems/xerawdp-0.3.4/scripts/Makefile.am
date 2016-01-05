pkglib_PYTHON = process.py merge.py config.py pmt_database.py run_database.py
install-exec-hook:
	cd $(DESTDIR)$(libdir) && \
	mv -f $(PACKAGE) $(PACKAGE)-$(VERSION) && \
	$(LN_S) $(PACKAGE)-$(VERSION) $(PACKAGE) && \
	for f in process.py merge.py; do \
		chmod a+x $(PACKAGE)/$$f; \
	done
