Name:           aurum
Version:        0.1.0
Release:        0
License:        Apache-2.0
Summary:        Automation framework for Ui testing
Group:          UI Framework
Source:         %{name}-%{version}.tar.gz
Source1001:     %{name}.manifest

BuildRequires:  meson
BuildRequires:  pkgconfig(grpc)
BuildRequires:  pkgconfig(grpc++)
BuildRequires:  pkgconfig(atspi-2)
BuildRequires:  pkgconfig(capi-system-info)
BuildRequires:  pkgconfig(capi-ui-efl-util)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(elementary)
BuildRequires:  gtest-devel

BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(capi-appfw-app-control)
BuildRequires: pkgconfig(capi-appfw-app-manager)

Requires:  pkgconfig(atspi-2)
Requires:  pkgconfig(dlog)

%description
aurum is a project for testing ui.
it provides interfaces through gRPC protocol.

%package devel
Summary: devel package for libaurum

%description devel
devel package for libaurum

%package bootstrap
Summary: bootstrap
License: Apache-2.0
Requires: %{name} = %{version}-%{release}
Requires: pkgconfig(grpc)
Requires: pkgconfig(grpc++)

%description bootstrap
gRPC Server

%prep
%setup -q
cp %{SOURCE1001} .

meson \
    --prefix /usr \
    --libdir %{_libdir} \
    -Dcpp_std=c++17 \
    -Dtizen=true \
    gbsbuild 2>&1

%build

ninja \
    -C gbsbuild \
    -j %(echo "`/usr/bin/getconf _NPROCESSORS_ONLN`") \
    -v \
    all

%check

ninja \
    -C gbsbuild \
    -j %(echo "`/usr/bin/getconf _NPROCESSORS_ONLN`") \
    -v \
    test

%install

export DESTDIR=%{buildroot}
ninja -C gbsbuild install

%post
sbin/ldconfig

%postun 
sbin/ldconfig

%post bootstrap
#/sbin/ldconfig
chsmack -e "User" %{_bindir}/bootstrap_server

%postun bootstrap
/sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{_libdir}/libaurum.so.*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{_libdir}/libaurum.so

%files bootstrap
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{_bindir}/aurum_bootstrap
%{_unitdir_user}/aurum-bootstrap.service
