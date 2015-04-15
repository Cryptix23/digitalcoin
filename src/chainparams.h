// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAIN_PARAMS_H
#define BITCOIN_CHAIN_PARAMS_H

#include "core.h"
#include "protocol.h"
#include "uint256.h"
#include "util.h"

#include <vector>

typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

struct CDNSSeedData {
	std::string name, host;
    CDNSSeedData(const std::string &strName, const std::string &strHost) : name(strName), host(strHost) {}
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Network {
        MAIN,
        TESTNET,
        REGTEST,

        MAX_NETWORK_TYPES
    };

    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,
        EXT_PUBLIC_KEY,
        EXT_SECRET_KEY,

        MAX_BASE58_TYPES
    };

    const uint256& HashGenesisBlock() const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart() const { return pchMessageStart; }
    const std::vector<unsigned char>& AlertKey() const { return vAlertPubKey; }
    int GetDefaultPort() const { return nDefaultPort; }
    const uint256& ProofOfWorkLimit(int algo) const { return bnProofOfWorkLimit[algo]; }
    int SubsidyHalvingInterval() const { return nSubsidyHalvingInterval; }
	/* Used if GenerateBitcoins is called with a negative number of threads */
	int DefaultMinerThreads() const { return nMinerThreads; }
	int64_t TargetTimespan() const { return nTargetTimespan; }
	int64_t TargetSpacing() const { return nTargetSpacing; }
	int64_t Interval() const { return nTargetTimespan / nTargetSpacing; }
	const CBlock& GenesisBlock() const { return genesis; }
	bool RequireRPCPassword() const { return fRequireRPCPassword; }
	/* Make miner wait to have peers to avoid wasting work */
	bool MiningRequiresPeers() const { return fMiningRequiresPeers; }
	 /* Make miner stop after a block is found. In RPC, don't return
     * until nGenProcLimit blocks are generated */
	 /* Default value for -checkmempool argument */
	bool DefaultCheckMemPool() const { return fDefaultCheckMemPool; }
	bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }
    const std::string& DataDir() const { return strDataDir; }
    Network NetworkID() const { return networkID; }
	/* Return the BIP70 network string (main, test or regtest) */
	std::string NetworkIDString() const { return strNetworkID; }
	/* Make standard checks */
	bool RequireStandard() const { return fRequireStandard; }	
    const std::vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
	const std::vector<CAddress>& FixedSeeds() const { return vFixedSeeds; }
	
    int RPCPort() const { return nRPCPort; }
protected:
    CChainParams() {}

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    // Raw pub key bytes for the broadcast alert signing key.
    std::vector<unsigned char> vAlertPubKey;
    int nDefaultPort;
    int nRPCPort;
	int nMinerThreads;
    uint256 bnProofOfWorkLimit[NUM_ALGOS];
    int nSubsidyHalvingInterval;
	int64_t nTargetTimespan;
	int64_t nTargetSpacing;
    std::string strDataDir;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
	Network networkID;
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
    bool fRequireRPCPassword;
    bool fMiningRequiresPeers;
    bool fDefaultCheckMemPool;
    bool fAllowMinDifficultyBlocks;
    bool fRequireStandard;    
	bool fMineBlocksOnDemand;
};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams &Params();

/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

#endif
