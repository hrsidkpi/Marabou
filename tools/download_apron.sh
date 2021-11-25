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

# A disgusting hack
cd installed/include
cp ../../taylor1plus/t1p_representation.h ../../taylor1plus/t1p_constructor.h ../../taylor1plus/t1p_meetjoin.h  ../../taylor1plus/t1p_assign.h ../../taylor1plus/t1p_resize.h ../../taylor1plus/t1p_otherops.h ../../taylor1plus/t1p_fun.h ../../taylor1plus/t1p_itv_utils.h  .

cd $curdir
