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

Requires:       at-spi2-core
BuildRequires:  at-spi2-core-devel

BuildRequires:  pkgconfig(capi-system-info)
BuildRequires:  pkgconfig(capi-ui-efl-util)
BuildRequires:  pkgconfig(elementary)
BuildRequires:  gtest-devel

BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(capi-appfw-app-control)
BuildRequires: pkgconfig(capi-appfw-app-manager)
BuildRequires: pkgconfig(capi-appfw-service-application)
BuildRequires: pkgconfig(capi-system-device)
BuildRequires: pkgconfig(libtzplatform-config)

%if "%{?profile}" == "tv"
  %define __hash_signing 0
%else
  %define __hash_signing 1
%endif

%if 0%{?__hash_signing}
BuildRequires:  hash-signer
%if 0%{?sec_product_feature_profile_wearable}
Requires(post): signing-client
%endif
%endif

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
Requires: libgrpc

%description bootstrap
gRPC Server

%prep
%setup -q
cp %{SOURCE1001} .

export LDFLAGS+="-Wl,-z,noexecstack"

meson \
    --prefix /usr \
    --libdir %{_libdir} \
    -Dcpp_std=c++17 \
    -Dtizen=true \
    -Dtzapp_path=%{TZ_SYS_RO_APP} \
    -Dtzpackage_path=%{TZ_SYS_RO_PACKAGES} \
    gbsbuild 2>&1 | sed \
        -e 's%^.*: error: .*$%\x1b[37;41m&\x1b[m%' \
        -e 's%^.*: warning: .*$%\x1b[30;43m&\x1b[m%'


%build
ninja \
    -C gbsbuild \
    -j %(echo "`/usr/bin/getconf _NPROCESSORS_ONLN`") \
    -v \
    all 2>&1 | sed \
        -e 's%^.*: error: .*$%\x1b[37;41m&\x1b[m%' \
        -e 's%^.*: warning: .*$%\x1b[30;43m&\x1b[m%'


ninja \
    -C gbsbuild \
    -j %(echo "`/usr/bin/getconf _NPROCESSORS_ONLN`") \
    -v \
    test 2>&1 | sed \
        -e 's%^.*: error: .*$%\x1b[37;41m&\x1b[m%' \
        -e 's%^.*: warning: .*$%\x1b[30;43m&\x1b[m%'

%install

export DESTDIR=%{buildroot}
ninja -C gbsbuild install

%if 0%{?__hash_signing}
%define tizen_sign 1
%define tizen_sign_base /usr/apps/org.tizen.aurum-bootstrap
%define tizen_sign_level platform
%define tizen_author_sign 1
%define tizen_dist_sign 1
%endif

%post
sbin/ldconfig

%postun
sbin/ldconfig

%post bootstrap
%if 0%{?sec_product_feature_profile_wearable}
echo "signing %{TZ_SYS_RO_APP}/org.tizen.aurum-bootstrap"
/usr/bin/signing-client/hash-signer-client.sh -a -d -p platform %{TZ_SYS_RO_APP}/org.tizen.aurum-bootstrap
%endif
#chsmack -e "User" %{_bindir}/aurum_bootstrap

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
#%{_bindir}/aurum_bootstrap
#%{_unitdir_user}/aurum-bootstrap.service
%{TZ_SYS_RO_PACKAGES}/org.tizen.aurum-bootstrap.xml
%{TZ_SYS_RO_APP}/org.tizen.aurum-bootstrap/*

