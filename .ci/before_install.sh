#!/bin/bash
set -xe
wget https://bitbucket.org/eigen/eigen/get/3.2.9.zip
unzip 3.2.9.zip
cd eigen-eigen-dc6cfdf9bcec
mkdir build
cd build
cmake ..
sudo make install
cd ../..