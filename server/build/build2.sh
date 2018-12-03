#!/bin/sh

# Auther: sunzcosmo@gmail.com
TIM_OS_TYPE=`ls -al ./os/ | grep "linux" | awk -F[_\.] '{ print $2 }'`

. ./os/linux_arch.sh

function init {
    
    ## Check & install cmake
    local cmake_info=`whereis cmake | awk -F: '{ print $2} '`
    if [ "X" = "X${cmake_info}" ]; then
        install cmake
    fi

    ## Check & install uuid
    local libuuid_cnt=`ldconfig -p | grep uuid | wc -l`
    if [ $libuuid_cnt -lt 1 ]; then
        install uuid
    fi

    ## Check & install openssl
    local libssl_cnt=`ldconfig -p | grep ssl | wc -l`
    local libcrypto_cnt=`ldconfig -p | grep crypto | wc -l`
    if [ $libssl_cnt -lt 1 -o $libcrypto_cnt -lt 1 ]; then
        install openssl
    fi

    ## Check & install curl
    local libcurl_cnt=`ldconfig -p | grep curl | wc -l`
    if [ $libcurl_cnt -lt 1 ]; then
        install curl
    fi

    ## Check & install libmysql
    local libmysql_cnt=`ldconfig -p | grep mysqlclient | wc -l`
    if [ $libmysql_cnt -lt 1 ]; then
        install mariadb
    fi


    ## Generating version.h, if version para not set, using default instead.
    ## Default version format "${date}-0-0-0"
	echo "#ifndef __VERSION_H__" > ../src/base/version.h
	echo "#define __VERSION_H__" >> ../base/version.h
	echo "#define VERSION \"$1\"" >> ../base/version.h
	echo "#endif" >> ../src/base/version.h
    
    ## Creating lib dir for cpp lib file.
    if [ -d lib ]; then
        rm -r lib_bak
        mv lib lib_bak
    fi
    mkdir lib
    chmod 760 lib

    ## Creatinong run dir
    if [ -d run ]; then
        rm -r run_bak
        mv run run_bak
    fi
    mkdir server
    chmod -R 760 server
}

function build_module {
    local module_name=$1

    cd ../src/${module_name}/build/

    ./build_${module_name}.sh

    $build_ret=$?

    if [ "X0" = "X${build_ret}" ]; then
        echo 'Make ${module_name} successed.'
        ./publish_${module}.sh
    else
        echo 'Make ${module_name} failed, build_ret is ${build_ret}.'
    fi

    cd -

    return $build_ret
}

function build_modules {
    
    build_module third_party/slog

    build_module base

    build_module login_server

    build_module route_server

    build_module msg_server
    
    build_module http_msg_server

    build_module file_server

    build_module push_server

    build_module tools

    build_module db_proxy_server

    build_module msfs
}

function pack_and_release {
    local ret="0"

    local build_version="$1"
    local release_name="${build_version}.tar.gz"
    if [ -e $build_name ]; then
        mv $build_name "${build_name}.bak"
    fi
    
    mkdir -p ../pack/$build_version

    cp ./run ../pack/$build_version

    cp ./lib ../pcak/$build_version
    
    cd ../pack
    tar -zcvf $build_name $build_version
    ret=$?
    rm -r build_version
    cd -

    return $ret
}

function main() {
    if [ $# -gt 1 ]; then
        echo 'Invalid cmd, try --help for more information.'
    fi

    local version_file='../src/base/bin/version.sh'

    local build_time=`date`

    sed -i -e 's/^TIM_BUILD_TIME=.*$/TIM_BUILD_TIME='${build_time}'/g' $version_file

    local_version=${TIM_VERSION}

    case "$1" in
        "--version")

            ;;
        "--help")
            ;;
        *)
        ;;
    esac

    local ret="0"
    init ${version}
    ret=$?
    if [ "X${ret}" != "X0" ]; then
        echo "Init env failed."
        return $ret
    fi
        

    build_modules
    ret=$?
    if [ "X${ret}" != "X0" ]; then
        echo "Build modules failed."
        return $ret
    fi

    pack_and_release $version
    ret=$?
    if [ "X${ret}" != "X0" ]; then
        echo "Pack & release server failed."
        return $ret
    fi

    return 0
}

main $@
