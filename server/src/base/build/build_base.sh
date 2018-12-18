#!/bin/sh

function main() {
    if [ -f build_base.log ]; then
        mv build_base.log build_base.log.bak
    fi

    make clean

    cmake --clean-first ..

    make 2>&1 | tee build_base.log

    local ret=$?

    if [ -f "libbase.a" ]; then
        echo "Build base success."
    else
        echo "Build base failed."
    fi

    return $ret
}

main $@
