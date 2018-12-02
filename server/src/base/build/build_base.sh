#!/bin/sh

function main() {
    # Clean formal make files.
    rm -f build_base.log

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
