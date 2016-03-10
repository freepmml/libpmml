Name:    libpmml
Version: 1.0.0
Release: 1%{?dist}
Summary: library to evaluate PMML format in c++.

Group: System Environment/Libraries
License: GPL
URL: https://github.com/freepmml/libpmml.git
Source0: https://github.com/freepmml/libpmml.git/libpmml-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: xsd >= 2.0
BuildRequires: xerces-c-devel >= 2.7.0

%description
Very small library to evaluate PMML format in c++.
Only linearRegression implemented for now.

%package devel
Summary: Header files, libraries and development documentation for %{name}
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
This package contains header and .so file.


%prep
%if %(test -f %{SOURCE0} && echo 1 || echo 0)
%setup -q -n %{name}-%{version}
%else
rm -rf %_builddir/%{name}-%{version}
mkdir -p %_builddir
cd %_builddir
git clone --depth 1 https://github.com/freepmml/libpmml.git %{name}-%{version}
cd %{name}-%{version}
git archive --format tar --prefix %{name}-%{version}/ master | gzip > %{SOURCE0}
%setup -q -n %{name}-%{version} -T -D
%endif

%build
make -C src %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make -C src install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/libpmml*.so.*

%files devel
%defattr(-,root,root)
%{_libdir}/libpmml.so
%{_includedir}/pmml/*

%changelog
