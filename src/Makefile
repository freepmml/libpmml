# vim:ts=4:sts=4:sw=4:noet

CXXFLAGS=-std=c++0x -O3 -ggdb -I. -fPIC
LDFLAGS=-lxerces-c -Wl,-rpath -Wl,`pwd`
LIBNAME=libmpmml.so.1

.PHONY: clean all
all: pmml-test
clean:
	rm -f *.o *.d pmml-test $(LIBNAME) $(patsubst %.so.1,%.so,$(LIBNAME)) pmml-4-2.?pp || true
#

NODEPS:=clean tags
SOURCES:=$(shell find . -name "*.cpp" -printf "%f\n")
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	-include $(DEPFILES)
endif

%.d: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,%.o,$<)' $< -MF $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
pmml-4-2.cpp: pmml-4-2.xsd
	xsd cxx-tree --cxx-suffix .cpp --hxx-suffix .hpp --std c++11 --namespace-map http://www.dmg.org/PMML-4_2=PMML $<

pmml-test: $(LIBNAME)
pmml-test: test.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -L. -lmpmml $^ -o $@
$(LIBNAME): pmml-4-2.o Isotonic.o
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(LIBNAME) $^ -o $@
	ln -vfsT $(LIBNAME) $(patsubst %.so.1,%.so,$(LIBNAME))
