SUBDIRS = \
	applications \
	test-l10n-data
CLEANFILES =
AM_CPPFLAGS = \
	$(QtCore_CFLAGS) \
	-I$(top_srcdir)/src
LIBS += \
	../src/libcontentaction.la

.cpp.moc:
	$(MOC) -o "$@" "$<"

BUILT_SOURCES = \
	env.sh \
	env.py

citestdir = $(datarootdir)/libcontentaction-tests
citest_SCRIPTS = \
	sandbox.sh testlib.sh \
	cltool.py \
	gallery.py \
	servicemapper.py \
	uberprogram.py \
	test-actions.sh \
	test-mimetypes.sh \
	test-invoking.py \
	test-defaults.py \
	test-mimes.py \
	test-servicefw-signals.py \
	test-desktop-launching.py \
	test-l10n.sh \
	test-fixed-params.py \
	test-schemes.sh \
	test-special-chars.py \
	test-invalid-defaults.py \
	test-regexps.py

noinst_SCRIPTS = \
	start-sandbox.sh \
	stop-sandbox.sh \
	test-highlight.sh

citest_DATA = \
	tests.xml \
	service.map \
	testdata.ttl \
	plaintext \
	empty.pdf \
	launchme.desktop

check_PROGRAMS = \
	linktest

noinst_PROGRAMS = hldemo
hldemo_SOURCES = hldemo.cpp
hldemo_CPPFLAGS = \
	$(MT_CFLAGS) \
	-I$(top_srcdir)/src
hldemo_LDADD = \
	$(MT_LIBS) \
	../src/libcontentaction.la
BUILT_SOURCES += hldemo.moc
CLEANFILES += hldemo.moc

testhelperdir = $(libdir)/libcontentaction-tests
testhelper_PROGRAMS = \
	servicetest

linktest_SOURCES = linktest.cpp
linktest_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	$(GIO_CFLAGS) \
	$(MT_CFLAGS)
linktest_LDADD = \
	$(MT_LIBS)

servicetest_SOURCES = servicelistener.cpp

BUILT_SOURCES += servicelistener.moc

../src/libcontentaction.la: FORCE
	$(MAKE) -C ../src libcontentaction.la

.PHONY: FORCE

EXTRA_DIST = \
	$(citest_DATA) \
	$(citest_SCRIPTS) \
	$(noinst_SCRIPTS) \
	hlinput.txt \
	plaintext \
	empty.pdf \
	launchme.desktop

# NOTE: we depend on automake executing $(TESTS) in order.  Should this
# change, we fail.
TESTS = \
	start-sandbox.sh \
	linktest \
	test-actions.sh \
	test-mimetypes.sh \
	test-invoking.py \
	test-defaults.py \
	test-servicefw-signals.py \
	test-highlight.sh \
	test-mimes.py \
	test-desktop-launching.py \
	test-l10n.sh \
	test-fixed-params.py \
	test-schemes.sh \
	test-special-chars.py \
	test-invalid-defaults.py \
	test-regexps.py \
	stop-sandbox.sh

clean-local:
	rm -f *.py?;

CLEANFILES += servicelistener.moc
