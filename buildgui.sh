#!/bin/sh
# Copyright (c) 2024 The Earthcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

./autogen.sh
./configure --with-incompatible-bdb --with-gui
make
cd src
strip earthcoind
strip earthcoin-cli
strip earthcoin-tx
cd qt
# strip earthcoin.gui