#include "TransactionAccessList.h"

#include <libdevcore/Address.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
TransactionAccessList::TransactionAccessList(DataObject const& _data, string const& _dataRawPreview, string const& _dataLabel)
  : Transaction(_data, _dataRawPreview, _dataLabel)
{
    m_accessList = AccessList(_data.atKey("accessList"));
    if (_data.count("secretKey"))
        buildVRS(VALUE(_data.atKey("secretKey")));  // Build without v + 27
}

TransactionAccessList::TransactionAccessList(dev::RLP const& _rlp)
{
    fromRLP(_rlp);
}

TransactionAccessList::TransactionAccessList(BYTES const& _rlp)
{
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    fromRLP(rlp);
}

void TransactionAccessList::fromRLP(dev::RLP const& _rlp)
{
    // 0 - chainID
    // 1 - nonce        4 - to      7 - v
    // 2 - gasPrice     5 - value   8 - r
    // 3 - gasLimit     6 - data    9 - s
    DataObject trData;
    size_t i = 0;
    rlpToString(_rlp[i++]);  // chainID
    trData["nonce"] = rlpToString(_rlp[i++]);
    trData["gasPrice"] = rlpToString(_rlp[i++]);
    trData["gasLimit"] = rlpToString(_rlp[i++]);
    string const to = rlpToString(_rlp[i++], 0);
    trData["to"] = to == "0x" ? "" : to;
    trData["value"] = rlpToString(_rlp[i++]);
    trData["data"] = rlpToString(_rlp[i++], 0);

    // read access list
    AccessList list(_rlp[i++]);
    trData["accessList"] = list.asDataObject();
    m_accessList = list;

    trData["v"] = rlpToString(_rlp[i++]);
    trData["r"] = rlpToString(_rlp[i++]);
    trData["s"] = rlpToString(_rlp[i++]);
    fromDataObject(trData);
}

void TransactionAccessList::buildVRS(VALUE const& _secret)
{
    dev::RLPStream stream;
    stream.appendList(9);
    stream << VALUE(1).asU256();  // txType
    TransactionAccessList::streamHeader(stream);
    dev::h256 hash(dev::sha3(stream.out()));
    dev::Signature sig = dev::sign(dev::Secret(_secret.asString()), hash);
    dev::SignatureStruct sigStruct = *(dev::SignatureStruct const*)&sig;
    ETH_FAIL_REQUIRE_MESSAGE(
        sigStruct.isValid(), TestOutputHelper::get().testName() + " Could not construct transaction signature!");

    DataObject v = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.v), 1));
    DataObject r = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.r)));
    DataObject s = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.s)));
    assignV(spVALUE(new VALUE(v)));
    assignR(spVALUE(new VALUE(r)));
    assignS(spVALUE(new VALUE(s)));
}

void TransactionAccessList::streamHeader(dev::RLPStream& _s) const
{
    // rlp([chainId, nonce, gasPrice, gasLimit, to, value, data, access_list, yParity, senderR, senderS])
    _s << VALUE(1).asU256();
    _s << nonce().asU256();
    _s << gasPrice().asU256();
    _s << gasLimit().asU256();
    if (Transaction::isCreation())
        _s << "";
    else
        _s << dev::Address(to().asString());
    _s << value().asU256();
    _s << test::sfromHex(data().asString());

    // Access List
    dev::RLPStream accessList(m_accessList.list().size());
    for (auto const& el : m_accessList.list())
        accessList.appendRaw(el.asRLPStream().out());

    _s.appendRaw(accessList.out());
}

BYTES const TransactionAccessList::getSignedRLP() const
{
    return BYTES("0x" + dev::toHex(asRLPStream().out()));
}

dev::RLPStream const TransactionAccessList::asRLPStream() const
{
    // RLP(01 + tr.rlp)
    dev::RLPStream wrapper;
    dev::RLPStream out;
    out.appendList(11);
    streamHeader(out);
    out << v().asU256().convert_to<dev::byte>();
    out << r().asU256();
    out << s().asU256();

    // Alter output with prefixed 01 byte + tr.rlp
    dev::bytes outa = out.out();
    outa.insert(outa.begin(), dev::byte(1));

    // Encode bytearray into rlp
    wrapper << outa;
    return wrapper;
}

DataObject const TransactionAccessList::asDataObject(ExportOrder _order) const
{
    DataObject out = Transaction::asDataObject(_order);

    out["chainId"] = "0x01";
    out["accessList"] = m_accessList.asDataObject();
    out["type"] = "0x01";
    if (_order == ExportOrder::ToolStyle)
    {
        out["chainId"] = "0x1";
        out["type"] = "0x1";
    }
    return out;
}

FH32 TransactionAccessList::hash() const
{
    // HASH (01 + tr.rlp)
    dev::RLPStream out;
    out.appendList(11);
    streamHeader(out);
    out << v().asU256().convert_to<dev::byte>();
    out << r().asU256();
    out << s().asU256();

    dev::bytes trRLP = out.out();
    trRLP.insert(trRLP.begin(), dev::byte(1));
    return FH32("0x" + dev::toString(dev::sha3(trRLP)));
}


}  // namespace teststruct
}  // namespace test
