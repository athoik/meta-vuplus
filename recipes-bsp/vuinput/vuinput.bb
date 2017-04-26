SUMMARY = "Add missing stb/tsmux/inputX_choices proc helper"
MAINTAINER = "OpenPLi"
SECTION = "kernel/modules"
LICENSE = "GPLv2"
DEPENDS = "virtual/kernel"

PACKAGE_ARCH = "${MACHINE_ARCH}"

require conf/license/license-gplv2.inc

SRC_URI = "file://vuinput.c file://Makefile"

S = "${WORKDIR}"
PV = "1.0"
PR = "r0"

inherit module

do_compile () {  
    unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
    oe_runmake 'MODPATH="${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/vuinput" ' \
    -C "${STAGING_KERNEL_BUILDDIR}" SUBDIRS="${S}" modules
}

do_install () {
    install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/vuinput
    install -m 0644 ${S}/vuinput.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/vuinput
    install -d ${D}/${sysconfdir}/modules-load.d
    echo vuinput >> ${D}/${sysconfdir}/modules-load.d/zzvvuinput.conf
}

FILES_${PN} += "${sysconfdir}/modules-load.d/zzvuinput.conf"
