#!/bin/bash
sudo apt install automake autoconf
sudo apt install libncurses5-dev
./autogen.sh
./configure
make
make install