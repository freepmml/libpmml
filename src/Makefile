# vim:ts=4:sts=4:sw=4:noet

CXXFLAGS=-std=c++0x -O3 -ggdb -I. -fPIC
LDFLAGS=-lxerces-c -Wl,-rpath -Wl,`pwd`
LIBVERSION=1
LIBNAME=libpmml.so.$(LIBVERSION)
DEVLIBNAME=$(patsubst %.so.$(LIBVERSION),%.so,$(LIBNAME))

.PHONY: clean all install
all: $(LIBNAME)
clean:
	rm -f *.o *.d pmml-test $(LIBNAME) $(DEVLIBNAME) pmml-4-2.?pp || true

NODEPS:=clean tags
SOURCES:=$(shell find . -name "*.cpp" -printf "%f\n")
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	-include $(DEPFILES)
endif

Isotonic.d: pmml-4-2.cpp
%.d: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,%.o,$<)' $< -MF $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
pmml-4-2.cpp: pmml-4-2.xsd
	xsd cxx-tree --cxx-suffix .cpp --hxx-suffix .hpp --std c++11 --namespace-map http://www.dmg.org/PMML-4_2=PMML $<

test: $(LIBNAME)
test: test.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -L. -lpmml $^ -o $@
$(LIBNAME): pmml-4-2.o Isotonic.o
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(LIBNAME) $^ -o $@
	ln -vfsT $(LIBNAME) $(DEVLIBNAME)


install: $(DESTDIR)/usr/lib64/$(LIBNAME) $(DESTDIR)/usr/lib64/$(DEVLIBNAME) $(DESTDIR)/usr/include/pmml/Isotonic.hpp

$(DESTDIR)/usr/lib64/$(LIBNAME): $(LIBNAME)
	install -m 755 -D $(LIBNAME) $@

$(DESTDIR)/usr/include/pmml/Isotonic.hpp: Isotonic.hpp
	install -m 644 -D Isotonic.hpp $@

$(DESTDIR)/usr/lib64/$(DEVLIBNAME):
	ln -fsT $(LIBNAME) $@

