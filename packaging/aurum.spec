Name:           aurum
Version:        1.0.0
Release:        1
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
BuildRequires:  pkgconfig(gmock)

BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(capi-appfw-app-control)
BuildRequires: pkgconfig(capi-appfw-app-manager)
BuildRequires: pkgconfig(capi-appfw-service-application)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(capi-system-device)
BuildRequires: pkgconfig(libtzplatform-config)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(capi-base-utils-i18n)
BuildRequires: pkgconfig(vconf)

%if "%{mqtt}" == "1"
BuildRequires: pkgconfig(libmosquitto)
BuildRequires: pkgconfig(jsoncpp)
%endif

%if 0%{?gendoc:1}
BuildRequires:  doxygen
%endif

%if 0%{?gcov:1}
BuildRequires:  lcov
%endif

BuildRequires:  hash-signer
%if 0%{?sec_product_feature_profile_wearable}
Requires(post): signing-client
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

%if 0%{?gendoc:1}
%package docs
Summary: documentation
License: Apache-2.0
Requires: %{name} = %{version}-%{release}
Requires: libgrpc

%description docs
documentations for aurum
%endif

%if 0%{?gcov:1}
%package gcov
Summary:    Aurum - Ui Automation (gcov)
Group:      Graphics & UI Framework/Testing
%description gcov
Ui Automation Library Aurum gcov objects
%endif

%prep
%setup -q
cp %{SOURCE1001} .

%if "%{asan}" == "1"
%restore_fcommon
%else
export LDFLAGS+="-Wl,-z,noexecstack"
%endif

%if 0%{?gcov:1}
export CFLAGS+=" -fprofile-arcs -ftest-coverage "
export CXXFLAGS+=" -fprofile-arcs -ftest-coverage "
export FFLAGS+=" -fprofile-arcs -ftest-coverage"
export LDFLAGS+=" -lgcov"
%define TIZEN_GCOV true
%else
%define TIZEN_GCOV false
%endif

%if 0%{?gendoc:1}
%define TIZEN_GEN_DOC true
%else
%define TIZEN_GEN_DOC false
%endif

%if "%{mqtt}" == "1"
%define MQTT_ENABLED true
%else
%define MQTT_ENABLED false
%endif

meson \
    --prefix /usr \
    --libdir %{_libdir} \
    -Dcpp_std=c++17 \
    -Dtizen=true \
    -Denable_documentation=%{TIZEN_GEN_DOC} \
    -Dmqtt_enabled=%{MQTT_ENABLED} \
    -Dtizen_gcov=%{TIZEN_GCOV} \
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

%if 0%{?gcov:1}
  mkdir -p gcov-obj
  find . -name '*.gcno' -exec cp '{}' gcov-obj ';'
%endif

#Enable below test if needed
#meson test \
#    -C gbsbuild \
#    -v

%install

%define tizen_sign 1
%define tizen_sign_base /usr/apps/org.tizen.aurum-bootstrap
%define tizen_sign_level platform
%define tizen_author_sign 1
%define tizen_dist_sign 1

export DESTDIR=%{buildroot}
ninja -C gbsbuild install

%if 0%{?gcov:1}
mkdir -p %{buildroot}%{_datadir}/gcov/obj
install -m 0644 gcov-obj/* %{buildroot}%{_datadir}/gcov/obj
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

echo -e 'Make it preloaded app to get appdebugging privilege'
tpk-backend -y org.tizen.aurum-bootstrap --preload

%postun bootstrap
/sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{_libdir}/libaurum.so.*
%{_libdir}/libpugixml.so.*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{_includedir}/*
%{_libdir}/libaurum.so
%{_libdir}/libpugixml.so
%{_libdir}/pkgconfig/*

%files bootstrap
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{TZ_SYS_RO_PACKAGES}/org.tizen.aurum-bootstrap.xml
%{TZ_SYS_RO_APP}/org.tizen.aurum-bootstrap/*

%if 0%{?gendoc:1}
%files docs
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{_datadir}/doc/aurum/
%endif

%if 0%{?gcov:1}
%files gcov
%{_datadir}/gcov/obj/*
%{_bindir}/test_*
%endif
