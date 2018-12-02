#!/bin/sh

function install_mariadb() {
    sudo apt-get install mariadb
}


## For debug.
$@
