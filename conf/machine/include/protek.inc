# We know about rtc
require conf/machine/include/build-extras.inc

BOX_BRAND = "protek"

MTD_KERNEL = "mtd1"
MTD_ROOTFS = "mtd2"
KERNEL_FILE = "kernel.bin"
ROOTFS_FILE = "rootfs.bin"

IMAGEDIR = "${MACHINE}"

DVBPROVIDER = "kernel"
DVBSKYPROVIDER = "kernel"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS += "\
    protek-dvb-modules-${MACHINE} \
    mt7601u \
    firmware-mt7601u \
    "

MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS += "\
    kernel-module-cdfs \
    "

MACHINE_EXTRA_RRECOMMENDS += "\
    gstreamer1.0-plugin-dvbmediasink \
    "

require conf/machine/include/autoload-filesystems.inc

DVBMEDIASINK_CONFIG = "--with-wma --with-wmv --with-pcm --with-dtsdownmix --with-vb6 --with-vb8 --with-spark"

TARGET_ARCH = "mipsel"
DEFAULTTUNE = "mips32el"

PREFERRED_PROVIDER_virtual/kernel = "linux-protek"
PREFERRED_VERSION_linux-protek = "4.1.21"
PREFERRED_PROVIDER_virtual/blindscan-dvbc = "protek-blindscan-utils-dvbc"
PREFERRED_PROVIDER_virtual/blindscan-dvbs = "protek-blindscan-utils-dvbs"

KERNEL_IMAGETYPE = "vmlinux"
KERNEL_OUTPUT = "${KERNEL_IMAGETYPE}"
KERNEL_CONSOLE = "ttyS0,115200n8"

IMAGE_FSTYPES ?= "ubi"

UBI_VOLNAME = "rootfs"
MKUBIFS_ARGS = "-m 2048 -e 126976 -c 4096"
UBINIZE_ARGS = "-m 2048 -p 128KiB"

IMAGE_CMD_ubi_prepend = "\
    rm -rf ${IMAGE_ROOTFS}/boot/*; \
    rm -rf ${IMAGE_ROOTFS}/tmp/*; \
    "

IMAGE_CMD_ubi_append = "\
    mkdir -p ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}; \
    ${@bb.utils.contains("MACHINE_FEATURES", "bootsplash", "cp ${DEPLOY_DIR_IMAGE}/${BOOTLOGO_FILENAME} ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/${BOOTLOGO_FILENAME};" , "", d)} \
    mv ${IMGDEPLOYDIR}/${IMAGE_NAME}${IMAGE_NAME_SUFFIX}.ubi ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/${ROOTFS_FILE}; \
    gzip -9c ${DEPLOY_DIR_IMAGE}/vmlinux-${MACHINE}.bin > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/${KERNEL_FILE}; \
    echo ${IMAGE_NAME} > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/imageversion; \
    echo "rename this file to 'force' to force an update without confirmation" > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/noforce; \
    rm -f ${DEPLOY_DIR_IMAGE}/*.zip; \
    cd ${DEPLOY_DIR_IMAGE}; \
    zip ${IMAGE_NAME}_usb.zip ${IMAGEDIR}/*; \
    rm -f ${DEPLOY_DIR_IMAGE}/*.ubi; \
    rm -f ${DEPLOY_DIR_IMAGE}/*.ubifs; \
    rm -Rf ${IMAGEDIR}; \
    "

MACHINE_FEATURES += "3dtv alsa hdmicec switchoff videoenhancement ci blindscan-tbs 7segment textlcd middleflash"

# Broken bootloader can only flash around 100 MB
FLASHSIZE = "96"

BOOTLOGO_FILENAME = "splash.bin"

require conf/machine/include/tune-mips32.inc
require conf/machine/include/soc-family.inc
