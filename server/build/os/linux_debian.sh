#!/bin/sh

function install_mariadb() {
    sudo apt-get install mariadb
}

function install_hiredis() {

}


## For debug.
$@
