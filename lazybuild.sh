#!/bin/sh
# Copyright (c) 2024 The Earthcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

./autogen.sh
./configure --with_incompatible_bdb
make
