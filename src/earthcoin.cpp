// Copyright (c) 2015-2017 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <arith_uint256.h>
#include <earthcoin.h>
#include <pow.h>
#include <util.h>

// As Earthcoin calculate block reward using trigonometric functions to follow the Earth rotation ...
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/* int static generateMTRandom(unsigned int s, int range)
{
    boost::mt19937 gen(s);
    boost::uniform_int<> dist(1, range);
    return dist(gen);
} */           // Sando: not used any more, so do not compile

// Earthcoin: Normally minimum difficulty blocks can only occur in between
// retarget blocks. However, once we introduce Digishield every block is
// a retarget, so we need to handle minimum difficulty on all blocks.
bool AllowDigishieldMinDifficultyForBlock(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // never allow minimal difficulty, EAC retarget works fine	
    return false;
}

unsigned int CalculateEarthcoinNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    // do not change retarget algo	
    return CalculateNextWorkRequired(pindexLast, nFirstBlockTime, params);
}

bool CheckAuxPowProofOfWork(const CBlockHeader& block, const Consensus::Params& params)
{
    // check legacy blocks and non-AUXPOW
    if ( block.IsLegacy() || !block.IsAuxpow() ) {
        if (block.auxpow)
            return error("%s : legacy block with auxpow properties",
                         __func__);
        return CheckProofOfWork(block.GetPoWHash(), block.nBits, params);
    }

    /* We have auxpow.  Check it.  */
    if (!block.auxpow)
            return error("%s : auxpow block without auxpow properties",
                         __func__);

    /* Except for legacy blocks with full version 1, ensure that
       the chain ID is correct.  Legacy blocks are not allowed since
       the merge-mining start, which is checked in AcceptBlockHeader
       where the height is known.  */
    if (params.fStrictChainId && (block.GetChainId() != params.nAuxpowChainId) )
        return error("%s : block does not have our chain ID"
                     " (got %d, expected %d, full nVersion %d)",
                     __func__, block.GetChainId(),
                     params.nAuxpowChainId, block.nVersion);

    if (!block.auxpow->check(block.GetHash(), block.GetChainId(), params))
        return error("%s : AUX POW is not valid", __func__);
    if (!CheckProofOfWork(block.auxpow->getParentBlockPoWHash(), block.nBits, params))
        return error("%s : AUX proof of work failed", __func__);

    return true;	
}

CAmount GetEarthcoinBlockSubsidy(int nHeight, const Consensus::Params& consensusParams)
{
    const CAmount nMinSubsidy = 1 * COIN;
    // base payout
    CAmount nSubsidy = 10000 * COIN;

    // first block receives 2% premine to support EarthCoin
    if (nHeight == 1) {
	    nSubsidy = 270000000LL * COIN;
	    return nSubsidy;
    }

    // relative height inside main period (approx. 1 year)
    int nHeightM = nHeight % 525600;
    double phase = ((double)nHeightM) / 525600.0 * 2.0 * M_PI;
    // modify base payout with seasonal effect
    nSubsidy += ((int)(2000.0 * sin(phase))) * COIN;

    // bonus zones

    // get number of days since the inception of EarthCoin
    int day = nHeight / 1440 + 1;

    // regular bonus zones

    // every 31 days, payout is increased by a factor of 5
    if (day % 31 == 0) {
	    nSubsidy *= 5;
    }
    // every 14 days, payout is increased by a factor of 2
    else if (day % 14 == 0) {
	    nSubsidy *= 2;
    }

    // special bonus zones

    // the first three days were special (approx. 12/21-21/24 in the year of 2013)
    switch (day) {
        // 5 times the normal payout on day 1
	case 1:
	    nSubsidy *= 5;
	    break;
	// 3 times the normal payout on day 2
	case 2:
	    nSubsidy *= 3;
	    break;
	// 2 times the normal payout on day 3
	case 3:
	    nSubsidy *= 2;
	    break;
    }

    // subsidy is cut in half every 525600 blocks,
    // which will occur approximately every 12 months
    nSubsidy >>= (nHeight / 525600);

    // nevertheless, there will a minimum payout of 1 EarthCoin
    if (nSubsidy < nMinSubsidy) {
	    nSubsidy = nMinSubsidy;
    }

    return nSubsidy;
}
