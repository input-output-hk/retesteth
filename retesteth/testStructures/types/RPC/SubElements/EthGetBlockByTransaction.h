#pragma once
#include "../../../basetypes.h"
#include "../../Ethereum/Transaction.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
// Transaction Structure inside RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockByTransaction
{
    EthGetBlockByTransaction(DataObject const&);
    FH32 const& hash() const { return m_hash.getCContent(); }
    Transaction const& transaction() const
    {
        assert(isFullTransaction());
        return m_transaction.getCContent();
    }
    bool isFullTransaction() const { return !m_transaction.isEmpty(); }
    FH32 const& blockHash() const
    {
        assert(isFullTransaction());
        return m_blockHash.getCContent();
    }
    VALUE const& blockNumber() const
    {
        assert(isFullTransaction());
        return m_blockNumber.getCContent();
    }

private:
    spTransaction m_transaction;

    spFH32 m_blockHash;
    spVALUE m_blockNumber;
    spFH20 m_from;
    spFH32 m_hash;
    spVALUE m_transactionIndex;
};

}  // namespace teststruct
}  // namespace test
