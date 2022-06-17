#!/bin/bash

CAT=cat
TAR=tar
COPY=cp
MKDIR=mkdir
GZIP=gzip

verbose=0
debug_level=0
target_board=
target_boot_directory=
target_root_directory=
target_architecture=${ARCH}
script_name=$0

usage()
{
    echo "Usage: ${script_name} [options]"
    echo
    echo "[-h, --help]                          List this help"
    echo "[-v, --verbose]                       Turn on verbosity"
    echo "[-d LEVEL   , --debug LEVEL]          Debug Level"
    echo "[-t TARGET  , --target TARGET]        Target board name (ex. Kv260 Ultra96 Ultra96-V2)"
    echo "[-b BOOT_DIR, --boot-dir BOOT_DIR]    Target Boot Directory"
    echo "[-r BOOT_DIR, --root-dir ROOT_DIR]    Target Root Directory"
    if [[ ! -z $ARCH ]]; then
	default=" default=${ARCH}"
    else
	default=""
    fi
    echo "[-a ARCH    , --archtecture ARCH]     Target Architecture Name (ex. arm64 armhf)${default}"
    if [[ ! -z $SOURCE_ROOT_ARCHIVE ]]; then
	default=" default=${SOURCE_ROOT_ARCHIVE}"
    else
	default=""
    fi
    echo "[-f ARCHIVE , --root_archive ARCHIVE] Source Root Archive FIle Name (ex. debian11-rootfs-vanilla.tgz)${default}"
    if [[ ! -z $BUILD_VERSION ]]; then
	default=" default=${BUILD_VERSION}"
    else
	default=""
    fi
    echo "[-n VERSION , --build-version VERSION] Build Version Number${default}"
    echo "[-m DEVICE  , --mount-boot    DEVICE ] Device mount to /mnt/boot (ex. /dev/mmcblk0p1)"
    exit 1
}

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            usage
            ;;
        -t|--target)
            shift
            target_board=$1
            shift
            ;;
        -a|--architecture)
            shift
            target_architecture=$1
            shift
            ;;
        -b|--boot-dir)
            shift
            target_boot_directory=$1
            shift
            ;;
        -r|--root-dir)
            shift
            target_root_directory=$1
            shift
	    ;;
        -f|--root-archive)
            shift
            SOURCE_ROOT_ARCHIVE=$1
            shift
	    ;;
        -n|--build-version)
            shift
            BUILD_VERSION=$1
            shift
            ;;
        -m|--mount-boot)
            shift
            MOUNT_BOOT=$1
            shift
            ;;
        -v|--verbose)
            verbose=1
            shift
            ;;
        -d|--debug)
            shift
            debug_level=$1
            shift
            ;;
        *)
            echo "unknown option"
            usage
            ;;
    esac
done

if [[ -z $target_board ]]; then
    echo "Please specify the target board name"
    usage
fi

if [[ -z $BUILD_VERSION ]]; then
    echo "Please specify the build version number"
    usage
fi

if [[ -z $PROJECT_PATH ]]; then
    PROJECT_PATH='.'
fi

if [[ -z $BOOT_SOURCE_PATH ]]; then
    BOOT_SOURCE_PATH="${PROJECT_PATH}/target"
fi

if [[ -z $LINUX_IMAGE_SOURCE_PREFIX ]]; then
    LINUX_IMAGE_SOURCE_PREFIX="vmlinuz"
fi

if [[ -z $LINUX_IMAGE_TARGET_PREFIX ]]; then
    if [[ $target_architecture == "arm64" ]]; then
        LINUX_IMAGE_TARGET_PREFIX="image"
    else
        LINUX_IMAGE_TARGET_PREFIX="vmlinuz"
    fi
fi

if [[ -z $MOUNT_BOOT ]]; then
    mount_boot_device=
    if [[ $target_board == "Kv260"   ]] ||
       [[ $target_board == "UltraZed-EG-IOCC" ]]; then
        mount_boot_device="/dev/mmcblk1p1"
    fi
    if [[ $target_board == "Ultra96" ]] ||
       [[ $target_board == "Ultra96-V2" ]]; then
        mount_boot_device="/dev/mmcblk0p1"
    fi
else
    mount_boot_device=$MOUNT_BOOT
fi

source_boot_directory="${BOOT_SOURCE_PATH}/${target_board}/boot"
kernel_release="${KERNEL_VERSION}-${LOCAL_VERSION}"
linux_image_source="${LINUX_IMAGE_SOURCE_PREFIX}-${kernel_release}-${BUILD_VERSION}"
linux_image_target="${LINUX_IMAGE_TARGET_PREFIX}-${kernel_release}"
debian_package_tag="${kernel_release}_${kernel_release}-${BUILD_VERSION}_${target_architecture}"
linux_image_debian_package="${PROJECT_PATH}/linux-image-${debian_package_tag}.deb"
linux_headers_debian_package="${PROJECT_PATH}/linux-headers-${debian_package_tag}.deb"

if [[ ! -z $target_boot_directory ]]; then
    error=0
    if [[ $verbose == 1 ]]; then
        echo ""
        echo "# Make Boot Partition"
    fi
    if [[ $debug_level > 0 ]]; then
	echo "#     Board Type                   : ${target_board}"
	echo "#     Build Version                : ${BUILD_VERSION}"
	echo "#     Source Boot Directory        : ${source_boot_directory}"
	echo "#     Target Boot Directory        : ${target_boot_directory}"
	echo "#     Source Linux Image           : ${linux_image_source}"
	echo "#     Target Linux Image           : ${linux_image_target}"
    fi
    if [[ $verbose == 1 ]]; then
        echo ""
    fi

    if [[ ! -d $target_boot_directory ]]; then
	echo "Error : Target Boot Directory($target_boot_directory) not found"
	error=1
    fi
    if [[ ! -d $source_boot_directory ]]; then
	echo "Error : Source Boot Directory($source_boot_directory) not found"
	error=1
    fi
    if [[ ! -f $linux_image_source ]]; then
	echo "Error : Source Linux Image($linux_image_source) not found"
	error=1
    fi
    if [[ $error == 1 ]]; then
	exit 1
    fi

    command="${COPY} ${source_boot_directory}/* ${target_boot_directory}"
    if [[ $verbose == 1 ]]; then
        echo "$command"
    fi
    eval "$command"

    if [[ $LINUX_IMAGE_SOURCE_PREFIX == "vmlinuz" ]] && [[ $LINUX_IMAGE_TARGET_PREFIX == "image" ]]; then
        command="${GZIP} -d -c ${linux_image_source} > ${target_boot_directory}/${linux_image_target}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi

    if [[ $LINUX_IMAGE_SOURCE_PREFIX == $LINUX_IMAGE_TARGET_PREFIX ]]; then
        command="${COPY} ${linux_image_source} ${target_boot_directory}/${linux_image_target}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi
fi

if [[ ! -z $target_root_directory ]]; then
    error=0
    if [[ $verbose == 1 ]]; then
        echo ""
        echo "# Make Root Partition"
    fi
    if [[ $debug_level > 0 ]]; then
        echo "#     Board Type                   : ${target_board}"
        echo "#     Build Version                : ${BUILD_VERSION}"
        echo "#     Target Root Directory        : ${target_root_directory}"
        echo "#     Source Root Archive          : ${SOURCE_ROOT_ARCHIVE}"
        echo "#     Linux Image Debian Package   : ${linux_image_debian_package}"
        echo "#     Linux Headers Debian Package : ${linux_headers_debian_package}"
        echo "#     Device Drivers to add        : ${DEVICE_DRIVER_LIST[@]}"
    fi
    if [[ $verbose == 1 ]]; then
        echo ""
    fi

    if [[ ! -d $target_root_directory ]]; then
	echo "Error : Target Root Directory($target_root_directory) not found"
	error=1
    fi
    if [[ ! -e $SOURCE_ROOT_ARCHIVE ]]; then
	echo "Error : Source Root Archive($SOURCE_ROOT_ARCHIVE) not found"
	error=1
    fi
    if [[ ! -e $linux_image_debian_package ]]; then
	echo "Error : Linux Image Debian Package($linux_image_debian_package) not found"
	error=1
    fi
    if [[ ! -e $linux_headers_debian_package ]]; then
	echo "Error : Linux Header Debian Package($linux_headers_debian_package) not found"
	error=1
    fi
    if [[ $error == 1 ]]; then
	exit 1
    fi

    if [[ -d $SOURCE_ROOT_ARCHIVE ]]; then
	command="(${CAT} ${SOURCE_ROOT_ARCHIVE}/*) | ${TAR} xfz - -C ${target_root_directory}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi

    if [[ -f $SOURCE_ROOT_ARCHIVE ]]; then
	command="${TAR} xfz ${SOURCE_ROOT_ARCHIVE} -C ${target_root_directory}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi

    target_debian_directory="${target_root_directory}/home/fpga/debian"
    if [[ ! -e $target_debian_directory ]]; then
        command="${MKDIR} ${target_debian_directory}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
        fi
        eval "$command"
    fi
    if [[ ! -e $target_debian_directory ]]; then
	echo "Error : Target Debian Directory($target_debian_directory) not found"
	exit 1
    fi
    if [[ ! -d $target_debian_directory ]]; then
	echo "Error : Target Debian Directory($target_debian_directory) is not directory"
	exit 1
    fi
    
    install_debian_package_list[0]=$linux_image_debian_package
    install_debian_package_list[1]=$linux_headers_debian_package
    
    if [[ ! -z $DEVICE_DRIVER_LIST ]]; then
        for driver_name in "${DEVICE_DRIVER_LIST[@]}"
        do
            name="${driver_name}-${kernel_release}_*_${target_architecture}.deb"
            ## echo "NAME: ${name}"
            ## find_command="ls -1 ${PROJECT_PATH}/${name}"
            find_command="find ${PROJECT_PATH} -maxdepth 1 -name '${name}' -print"
            ## echo "FIND: ${find_command}"
            found_package_list=($(eval ${find_command} 2>/dev/null))
            ## echo "LIST: ${found_package_list[@]}"
            for found_package in "${found_package_list[@]}"
            do
                ## echo "FOUND: ${found_package}"
                install_debian_package_list=("${install_debian_package_list[@]}" $found_package)
            done
        done
    fi
    ## echo "${install_debian_package_list[@]}"
    for package in "${install_debian_package_list[@]}"
    do
        ## echo "SOURCE PACKAGE : ${package}"
	command="${COPY} ${package} ${target_debian_directory}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
        fi
        eval "$command"
    done

    if [[ ! -e ${target_root_directory}/mnt/boot ]]; then
        command="${MKDIR} ${target_root_directory}/mnt/boot"
        if [[ $verbose == 1 ]]; then
            echo "$command"
        fi
        eval "$command"
    fi
fi

if [[ ! -z $target_root_directory ]] && [[ ! -z $mount_boot_device ]]; then

    etc_fstab="${target_root_directory}/etc/fstab"
    add_entry=0
    if [[ ! -e $etc_fstab ]]; then
        add_entry=1
    else
        command="grep '^${mount_boot_device}' ${etc_fstab} > /dev/null 2>&1"
        eval "$command"
        add_entry=$?
    fi
    if [[ $add_entry == 1 ]]; then
        entry="${mount_boot_device}	/mnt/boot	auto		defaults	0	0"
        command="echo '""$entry""' >> ${etc_fstab}"
        if [[ $verbose == 1 ]]; then
            echo ""
            echo "# Add boot partition mount position to /etc/fstab"
            echo ""
            echo "$command"
        fi
        eval "$command"
    fi
fi

if [[ $verbose == 1 ]]; then
    echo ""
fi
