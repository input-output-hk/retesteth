#pragma once
#include "../../configs/FORK.h"
#include "../../configs/SealEngine.h"
#include "../Ethereum/State.h"
#include "../StateTests/Filled/Info.h"

#include "Filled/BlockchainTestBlock.h"
#include "Filled/BlockchainTestEnv.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

// using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
struct BlockchainTestInFilled : GCP_SPointerBase
{
    BlockchainTestInFilled(DataObject const&);
    string const& testName() const { return m_name; }
    Info const& testInfo() const { return m_info.getCContent(); }
    BlockchainTestEnv const& Env() const { return m_env.getCContent(); }
    BYTES const& genesisRLP() const { return m_genesisRLP.getCContent(); }

    SealEngine sealEngine() const { return m_sealEngine; }
    FORK const& network() const { return m_fork.getCContent(); }
    State const& Pre() const { return m_pre.getCContent(); }

    bool isFullState() const { return !m_post.isEmpty(); }
    State const& Post() const { return m_post.getCContent(); }
    FH32 const& PostHash() const { return m_postHash.getCContent(); }
    FH32 const& lastBlockHash() const { return m_lastBlockHash.getCContent(); }

    std::vector<BlockchainTestBlock> const& blocks() const { return m_blocks; }
    std::vector<string> const& unitTestExceptions() const { return m_exceptions; }

private:
    BlockchainTestInFilled() {}
    string m_name;
    spInfo m_info;
    SealEngine m_sealEngine;
    spFORK m_fork;
    spState m_pre;
    spBlockchainTestEnv m_env;
    // Genesis block header
    spBYTES m_genesisRLP;
    std::vector<BlockchainTestBlock> m_blocks;
    spState m_post;
    spFH32 m_postHash;
    spFH32 m_lastBlockHash;

    std::vector<string> m_exceptions;
};

struct BlockchainTest
{
    BlockchainTest(DataObject const&);
    std::vector<BlockchainTestInFilled> const& tests() const { return m_tests; }

private:
    BlockchainTest() {}
    std::vector<BlockchainTestInFilled> m_tests;
};


}  // namespace teststruct
}  // namespace test
