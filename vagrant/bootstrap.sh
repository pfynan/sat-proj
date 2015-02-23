#!/usr/bin/env bash

set -e

wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
yum -y install devtoolset-2-gcc devtoolset-2-binutils
yum -y install devtoolset-2-gcc-gfortran devtoolset-2-gcc-c++ zlib-devel

tar -xf /vagrant/vagrant/terminfo.tar --strip-components=1 -C /usr/share/terminfo


