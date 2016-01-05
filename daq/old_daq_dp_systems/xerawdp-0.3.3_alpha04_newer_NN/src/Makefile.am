bin_PROGRAMS = xerawdp
install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
	mv -f xerawdp$(EXEEXT) xerawdp-$(VERSION)$(EXEEXT) && \
	$(LN_S) xerawdp-$(VERSION)$(EXEEXT) xerawdp$(EXEEXT)
xerawdp_SOURCES = RawData.cpp RawData.h RawDataController.cpp RawDataController.h \
	Waveform.cpp Waveform.h Filter.cpp Filter.h PeakFinder.cpp PeakFinder.h \
	EventData.cpp EventData.h PositionReconstruction.cpp PositionReconstruction.h \
	SignalCorrections.cpp SignalCorrections.h SignalFitter.cpp SignalFitter.h \
	AnalysisManager.cpp AnalysisManager.h RawDataProcessor.cpp RawDataProcessor.h \
	XmlConfig.cpp XmlConfig.h \
	xerawdp.cpp xerawdp.h
nodist_xerawdp_SOURCES = VectorDict.cpp
xerawdp_LDADD = $(LIBXMLPP_LIBS) $(LIBCHI2_LIBS) $(LAPACK_LIBS) $(LIBFANN_LIBS) \
	-L@ROOTLIBDIR@ @ROOTLIBS@ @ROOTGLIBS@
xerawdp_CPPFLAGS = $(LIBXMLPP_CFLAGS) $(LIBCHI2_CFLAGS) $(LAPACK_CFLAGS) $(LIBFANN_CFLAGS) \
	-I@ROOTINCDIR@
BUILT_SOURCES = LinkDef.h Vector.h VectorDict.cpp VectorDict.h
CLEANFILES =  LinkDef.h Vector.h VectorDict.cpp VectorDict.h
LinkDef.h: Makefile
	echo '#pragma link C++ class vector<int>+;' > $@
	echo '#pragma link C++ class vector<vector<float> >+;' >> $@
Vector.h: Makefile
	echo '#include <vector>' > $@
VectorDict.cpp: Vector.h LinkDef.h
	rootcint -f $@ -c $^

