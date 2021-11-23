#!/bin/bash
curdir=$pwd

# Download apron
 git clone https://github.com/antoinemine/apron.git
 cd apron

# Prepare the makefile and installation dir
mkdir installed
cp Makefile.config.model Makefile.config
patch < ../Makefile.config.patch

# Build
make -C . APRON_PREFIX=./installed MLGMPIDL_PREFIX=./installed JAVA_PREFIX=./installed
make -C . install

cd $curdir