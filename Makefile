##
# libcpp Makefile
##

SRCDIRS = .
DESTDIR = $(DSTROOT)

OBJROOT=.
SYMROOT=.

installsrc:: $(SRCROOT)

	ditto $(SRCDIRS)/include $(SRCROOT)/include
	ditto $(SRCDIRS)/lib/buildit $(SRCROOT)/lib/buildit
	ditto $(SRCDIRS)/src $(SRCROOT)/src
	ditto $(SRCDIRS)/Makefile $(SRCROOT)/Makefile

clean::

installhdrs::

	mkdir -p $(DSTROOT)/usr/include/c++/v1
	cp $(SRCDIRS)/include/* $(DSTROOT)/usr/include/c++/v1/
	chown -R root:wheel $(DSTROOT)/usr/include
	chmod 755 $(DSTROOT)/usr/include/c++/v1
	chmod 644 $(DSTROOT)/usr/include/c++/v1/*

install:: installhdrs $(DESTDIR)

	cd lib && ./buildit
	ditto lib/libc++.1.dylib $(SYMROOT)/usr/lib/libc++.1.dylib
	cd lib && dsymutil -o $(SYMROOT)/libc++.1.dylib.dSYM  $(SYMROOT)/usr/lib/libc++.1.dylib
	mkdir -p $(DSTROOT)/usr/lib
	strip -u -r -x -S -o $(DSTROOT)/usr/lib/libc++.1.dylib $(SYMROOT)/usr/lib/libc++.1.dylib
	cd $(DSTROOT)/usr/lib && ln -s libc++.1.dylib libc++.dylib
	
	
