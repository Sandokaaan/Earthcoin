// Copyright (c) 2012-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_VERSION_H
#define EARTHCOIN_VERSION_H

/**
 * network protocol versioning
 */

// XXX: Decide if this is appropriate - if we reintroduce alerts we may need
//      to  reduce to 70012
static const int PROTOCOL_VERSION = 70018;

//! initial proto version, to be increased after version/verack negotiation
static const int INIT_PROTO_VERSION = 209;

//! In this version, 'getheaders' was introduced.
static const int GETHEADERS_VERSION = 70002;

//! disconnect from peers older than this proto version
static const int MIN_PEER_PROTO_VERSION = 70018;

//! nTime field added to CAddress, starting with this version;
//! if possible, avoid requesting addresses nodes older than this
static const int CADDR_TIME_VERSION = 31402;

//! BIP 0031, pong message, is enabled for all versions AFTER this one
static const int BIP0031_VERSION = 60000;

//! "filter*" commands are disabled without NODE_BLOOM after and including this version
static const int NO_BLOOM_VERSION = 70018;

//! "sendheaders" command and announcing blocks with headers starts with this version
static const int SENDHEADERS_VERSION = 70018;

//! "feefilter" tells peers to filter invs to you by fee starts with this version
static const int FEEFILTER_VERSION = 70018;

//! short-id-based block download starts with this version
static const int SHORT_IDS_BLOCKS_VERSION = 70018;

//! not banning for invalid compact blocks starts with this version
static const int INVALID_CB_NO_BAN_VERSION = 70018;

#endif // EARTHCOIN_VERSION_H
