%define __provides_exclude_from ^%{_datadir}/%{name}/.*$
%define __requires_exclude ^lib.*\\.*$

Name: djvu-viewer
Version: 0.0.477
Release: 1
Summary: DjVu Viewer
License: GPLv2
Source0: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(Qt5Quick)
#BuildRequires: pkgconfig(Qt5Widgets)
#BuildRequires: pkgconfig(Qt5WebKit)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(Qt5Xml)
BuildRequires: pkgconfig(sailfishsilica) >= 1.1.8
BuildRequires: libqt5sparql-devel
#BuildRequires: poppler-qt5-devel poppler-qt5 poppler-devel poppler
BuildRequires: mapplauncherd-qt5-devel
BuildRequires: cmake
BuildRequires: qt5-qttools-linguist
BuildRequires: pkgconfig(icu-i18n)
BuildRequires: libtool
BuildRequires: gcc 
BuildRequires: gcc-c++ 
BuildRequires: automake 
BuildRequires: autoconf 
BuildRequires: libjpeg-turbo-devel 
BuildRequires: libtiff-devel
#BuildRequires: libdjvulibre-devel >= 3.5.28
#Requires: calligra-components
#Requires: calligra-filters >= 3.1.0+git18
Requires: sailfishsilica-qt5 >= 1.1.107
#Requires: sailfish-components-accounts-qt5
#Requires: sailfish-components-textlinking
#Requires: libqt5sparql-tracker-direct
#Requires: sailjail-launch-approval
#Requires: qt5-qtqml-import-webkitplugin
Requires: nemo-qml-plugin-configuration-qt5
Requires: nemo-qml-plugin-filemanager
#absent in emulator
#Requires: sailfish-content-graphics
#Requires: qt5-qtdeclarative-import-qtquick2plugin >= 5.4.0
#Requires: declarative-transferengine-qt5 >= 0.3.1
#Requires: libdjvulibre21 >= 3.5.28
#Requires: libjpeg-turbo
#Requires: libtiff



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


