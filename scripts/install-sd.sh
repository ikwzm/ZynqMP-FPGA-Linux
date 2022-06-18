#!/bin/bash

CAT=cat
TAR=tar
COPY=cp
MKDIR=mkdir
GZIP=gzip

verbose=0
debug_level=0
target_architecture=${ARCH}
script_name=$0

usage_indent="        "
opt_usage_list=()
var_usage_list=()

opt_usage()
{
    opt_usage_list+=("${usage_indent}$1")
    shift
    while [ $# -gt 0 ]; do
        opt_usage_list+=("${usage_indent}    $1")
        shift
    done
    opt_usage_list+=("")
}

var_usage()
{
    local var_name=$1
    local var_value=$(eval echo "\$${var_name}")
    local default=
    var_usage_list+=("${usage_indent}$1")
    shift
    opt_usage_list+=("${usage_indent}$1")
    shift
    while [ $# -gt 0 ]; do
	var_usage_list+=("${usage_indent}    $1")
        opt_usage_list+=("${usage_indent}    $1")
        shift
    done
    if [[ ! -z $var_value ]]; then
        var_usage_list+=("${usage_indent}    Default: $var_value")
        opt_usage_list+=("${usage_indent}    Default: $var_value")
    fi
    var_usage_list+=("")
    opt_usage_list+=("")
}
    
    


usage()
{
    echo "NAME"
    echo "    ${script_name} - Install Linux Image and RootFS to SD-Card"
    echo ""
    echo "SYNOPSIS"
    echo "    ${script_name} - [<options>]"
    echo ""
    echo "DESCRIPTION"
    echo "    Install Linux Image and RootFS to SD-Card"
    echo ""
    opt_usage "-h, --help"                  "List this help"
    opt_usage "-v, --verbose"               "Turn on verbosity"
    opt_usage "-d LEVEL   , --debug LEVEL"  "Debug Level"
    var_usage ARCH \
              "-a ARCH    , --archtecture ARCH" \
              "Target Architecture Name" \
	      "Example: arm64 armhf"
    var_usage TARGET_BOARD \
              "-t TARGET  , --target TARGET" \
              "Target Board Name" \
	      "Example: Kv260 Ultra96 Ultra96-V2"
    var_usage TARGET_BOOT_DIRECTORY \
              "-b BOOT_DIR, --boot-dir BOOT_DIR" \
              "Target Boot Directory" 
    var_usage TARGET_ROOT_DIRECTORY \
              "-r BOOT_DIR, --root-dir ROOT_DIR"\
              "Target Root Directory" 
    var_usage LINUX_ROOT_ARCHIVE \
              "-f ARCHIVE , --root-archive ARCHIVE" \
              "Source Root Archive File Name" \
	      "Example: debian11-rootfs-vanilla.tgz"
    var_usage LINUX_KERNEL_VERSION \
              "-k VERSION , --linux-kernel-version VERSION" \
              "Linux Kernel Version" \
	      "Example: 5.4.0" 
    var_usage LINUX_LOCAL_VERSION \
              "-l VERSION , --linux-local-version VERSION"  \
              "Linux Local  Version" \
	      "Example: xlnx-v2020.2-zynqmp-fpga"
    var_usage LINUX_BUILD_VERSION \
              "-n VERSION , --linux-build-version VERSION" \
	      "Linux Build  Version" 
    var_usage MOUNT_BOOT \
              "-m DEVICE  , --mount-boot DEVICE"  \
              "Device mount to /mnt/boot" \
	      "Example: /dev/mmcblk0p1" 
    echo "OPTION"
    echo
    for line in "${opt_usage_list[@]}"
    do
	echo "$line"
    done
    echo "VARIABLES"
    echo
    for line in "${var_usage_list[@]}"
    do
	echo "$line"
    done
    exit 1
}

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            usage
            ;;
        -t|--target)
            shift
            TARGET_BOARD=$1
            shift
            ;;
        -a|--architecture)
            shift
            target_architecture=$1
            shift
            ;;
        -b|--boot-dir)
            shift
            TARGET_BOOT_DIRECTORY=$1
            shift
            ;;
        -r|--root-dir)
            shift
            TARGET_ROOT_DIRECTORY=$1
            shift
	    ;;
        -f|--root-archive)
            shift
            LINUX_ROOT_ARCHIVE=$1
            shift
	    ;;
        -n|--linux-build-version)
            shift
            LINUX_BUILD_VERSION=$1
            shift
            ;;
        -k|--linux-kernel-version)
            shift
            LINUX_KERNEL_VERSION=$1
            shift
            ;;
        -l|--linux-local-version)
            shift
            LINUX_LOCAL_VERSION=$1
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

if [[ -z $TARGET_BOARD ]]; then
    echo "Please specify the target board name"
    usage
fi

if [[ -z $LINUX_BUILD_VERSION ]]; then
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
    if [[ $TARGET_BOARD == "Kv260"   ]] ||
       [[ $TARGET_BOARD == "UltraZed-EG-IOCC" ]]; then
        mount_boot_device="/dev/mmcblk1p1"
    fi
    if [[ $TARGET_BOARD == "Ultra96" ]] ||
       [[ $TARGET_BOARD == "Ultra96-V2" ]]; then
        mount_boot_device="/dev/mmcblk0p1"
    fi
else
    mount_boot_device=$MOUNT_BOOT
fi

source_boot_directory="${BOOT_SOURCE_PATH}/${TARGET_BOARD}/boot"
kernel_release="${LINUX_KERNEL_VERSION}-${LINUX_LOCAL_VERSION}"
linux_image_source="${LINUX_IMAGE_SOURCE_PREFIX}-${kernel_release}-${LINUX_BUILD_VERSION}"
linux_image_target="${LINUX_IMAGE_TARGET_PREFIX}-${kernel_release}"
debian_package_tag="${kernel_release}_${kernel_release}-${LINUX_BUILD_VERSION}_${target_architecture}"
linux_image_debian_package="${PROJECT_PATH}/linux-image-${debian_package_tag}.deb"
linux_headers_debian_package="${PROJECT_PATH}/linux-headers-${debian_package_tag}.deb"

if [[ ! -z $TARGET_BOOT_DIRECTORY ]]; then
    error=0
    if [[ $verbose == 1 ]]; then
        echo ""
        echo "# Make Boot Partition"
    fi
    if [[ $debug_level > 0 ]]; then
	echo "#     Board Type                   : ${TARGET_BOARD}"
	echo "#     Linux Build Version          : ${LINUX_BUILD_VERSION}"
	echo "#     Source Boot Directory        : ${source_boot_directory}"
	echo "#     Target Boot Directory        : ${TARGET_BOOT_DIRECTORY}"
	echo "#     Source Linux Image           : ${linux_image_source}"
	echo "#     Target Linux Image           : ${linux_image_target}"
    fi
    if [[ $verbose == 1 ]]; then
        echo ""
    fi

    if [[ ! -d $TARGET_BOOT_DIRECTORY ]]; then
	echo "Error : Target Boot Directory($TARGET_BOOT_DIRECTORY) not found"
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

    command="${COPY} ${source_boot_directory}/* ${TARGET_BOOT_DIRECTORY}"
    if [[ $verbose == 1 ]]; then
        echo "$command"
    fi
    eval "$command"

    if [[ $LINUX_IMAGE_SOURCE_PREFIX == "vmlinuz" ]] && [[ $LINUX_IMAGE_TARGET_PREFIX == "image" ]]; then
        command="${GZIP} -d -c ${linux_image_source} > ${TARGET_BOOT_DIRECTORY}/${linux_image_target}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi

    if [[ $LINUX_IMAGE_SOURCE_PREFIX == $LINUX_IMAGE_TARGET_PREFIX ]]; then
        command="${COPY} ${linux_image_source} ${TARGET_BOOT_DIRECTORY}/${linux_image_target}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi
fi

if [[ ! -z $TARGET_ROOT_DIRECTORY ]]; then
    error=0
    if [[ $verbose == 1 ]]; then
        echo ""
        echo "# Make Root Partition"
    fi
    if [[ $debug_level > 0 ]]; then
        echo "#     Board Type                   : ${TARGET_BOARD}"
        echo "#     Linux Build Version          : ${LINUX_BUILD_VERSION}"
        echo "#     Target Root Directory        : ${TARGET_ROOT_DIRECTORY}"
        echo "#     Source Root Archive          : ${LINUX_ROOT_ARCHIVE}"
        echo "#     Linux Image Debian Package   : ${linux_image_debian_package}"
        echo "#     Linux Headers Debian Package : ${linux_headers_debian_package}"
        echo "#     Device Drivers to add        : ${DEVICE_DRIVER_LIST[@]}"
    fi
    if [[ $verbose == 1 ]]; then
        echo ""
    fi

    if [[ ! -d $TARGET_ROOT_DIRECTORY ]]; then
	echo "Error : Target Root Directory($TARGET_ROOT_DIRECTORY) not found"
	error=1
    fi
    if [[ ! -e $LINUX_ROOT_ARCHIVE ]]; then
	echo "Error : Source Root Archive($LINUX_ROOT_ARCHIVE) not found"
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

    if [[ -d $LINUX_ROOT_ARCHIVE ]]; then
	command="(${CAT} ${LINUX_ROOT_ARCHIVE}/*) | ${TAR} xfz - -C ${TARGET_ROOT_DIRECTORY}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi

    if [[ -f $LINUX_ROOT_ARCHIVE ]]; then
	command="${TAR} xfz ${LINUX_ROOT_ARCHIVE} -C ${TARGET_ROOT_DIRECTORY}"
        if [[ $verbose == 1 ]]; then
            echo "$command"
	fi
	eval "$command"
    fi

    target_debian_directory="${TARGET_ROOT_DIRECTORY}/home/fpga/debian"
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
                install_debian_package_list+=($found_package)
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

    if [[ ! -e ${TARGET_ROOT_DIRECTORY}/mnt/boot ]]; then
        command="${MKDIR} ${TARGET_ROOT_DIRECTORY}/mnt/boot"
        if [[ $verbose == 1 ]]; then
            echo "$command"
        fi
        eval "$command"
    fi
fi

if [[ ! -z $TARGET_ROOT_DIRECTORY ]] && [[ ! -z $mount_boot_device ]]; then

    etc_fstab="${TARGET_ROOT_DIRECTORY}/etc/fstab"
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
