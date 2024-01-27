#!/bin/sh
# Copyright (c) 2024 The Earthcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

./autogen.sh
./configure --with-incompatible-bdb
make -j$(nproc)
cd src
strip earthcoind
strip earthcoin-cli
strip earthcoin-tx
cp earthcoind ../earthcoind
cp earthcoin-cli ../earthcoin-cli
cp earthcoin-tx ../earthcoin-tx
