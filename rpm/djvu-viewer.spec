%define __provides_exclude_from ^%{_datadir}/%{name}/.*$
%define __requires_exclude ^lib.*\\.*$

Name: info.you-ra.djvuviewer
Version: 0.1.34100
Release: 1
Summary: DjVu Viewer
License: GPLv2
Source0: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(Qt5Xml)
BuildRequires: pkgconfig(sailfishsilica) >= 1.1.8
BuildRequires: mapplauncherd-qt5-devel
BuildRequires: cmake
BuildRequires: qt5-qttools-linguist
BuildRequires: pkgconfig(icu-i18n)
BuildRequires: libtool
BuildRequires: gcc 
BuildRequires: gcc-c++ 
BuildRequires: automake 
BuildRequires: autoconf 
#BuildRequires: libtiff-devel
Requires: sailfishsilica-qt5 >= 1.1.107
#Requires: sailjail-launch-approval
#Requires: nemo-qml-plugin-configuration-qt5


%description
%{summary}.


%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_datadir}/applications/*.desktop
%{_datadir}/%{name}/
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
%{_datadir}/icons/hicolor/108x108/apps/%{name}.png
%{_datadir}/icons/hicolor/128x128/apps/%{name}.png
%{_datadir}/icons/hicolor/172x172/apps/%{name}.png


%prep
%setup -q -n %{name}-%{version}


%build
cmake -DCMAKE_INSTALL_PREFIX=/usr .
make %{?_smp_mflags}


%install
make DESTDIR=%{buildroot} install


