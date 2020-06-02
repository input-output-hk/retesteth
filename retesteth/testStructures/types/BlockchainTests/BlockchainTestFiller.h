#pragma once
#include "../../configs/FORK.h"
#include "../../configs/SealEngine.h"
#include "../Ethereum/State.h"
#include "../StateTests/Filler/InfoIncomplete.h"
#include "Base/BlockchainTestFillerEnv.h"
#include "Filler/BlockchainTestFillerBlock.h"
#include "Filler/BlockchainTestFillerExpectSection.h"

#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

// using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
struct BlockchainTestInFiller : GCP_SPointerBase
{
    BlockchainTestInFiller(DataObject const&);
    string const& testName() const { return m_name; }
    bool hasInfo() const { return !m_info.isEmpty(); }
    InfoIncomplete const& Info() const { return m_info.getCContent(); }
    SealEngine sealEngine() const { return m_sealEngine; }
    BlockchainTestFillerEnv const& Env() const { return m_env.getCContent(); }
    State const& Pre() const { return m_pre.getCContent(); }
    std::vector<BlockchainTestFillerBlock> const& blocks() const { return m_blocks; }
    std::vector<BlockchainTestFillerExpectSection> const& expects() const { return m_expects; }

    bool hasExpectForNetwork(FORK const& _net) const;
    std::list<FORK> getAllForksFromExpectSections() const;
    std::vector<string> const& unitTestExceptions() const { return m_exceptions; }
    bool hasUnclesInTest() const { return m_hasAtLeastOneUncle; }

private:
    BlockchainTestInFiller() {}
    string m_name;
    spInfoIncomplete m_info;
    SealEngine m_sealEngine;
    spBlockchainTestFillerEnv m_env;
    spState m_pre;
    std::vector<BlockchainTestFillerBlock> m_blocks;
    std::vector<BlockchainTestFillerExpectSection> m_expects;
    std::vector<string> m_exceptions;
    bool m_hasAtLeastOneUncle;
};

struct BlockchainTestFiller
{
    BlockchainTestFiller(DataObject const&);
    std::vector<BlockchainTestInFiller> const& tests() const { return m_tests; }

private:
    BlockchainTestFiller() {}
    std::vector<BlockchainTestInFiller> m_tests;
};


}  // namespace teststruct
}  // namespace test
