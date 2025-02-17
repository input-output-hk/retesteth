#include "StateTestPostResult.h"
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
StateTestPostResult::StateTestPostResult(DataObject const& _data)
{
    m_dataInd = _data.atKey("indexes").atKey("data").asInt();
    m_gasInd = _data.atKey("indexes").atKey("gas").asInt();
    m_valInd = _data.atKey("indexes").atKey("value").asInt();
    m_hash = spFH32(new FH32(_data.atKey("hash").asString()));
    m_log = spFH32(new FH32(_data.atKey("logs").asString()));
    if (_data.count("txbytes"))
        m_txbytes = spBYTES(new BYTES(_data.atKey("txbytes").asString()));
    requireJsonFields(_data, "StateTestPostResult " + _data.getKey(),
        {{"indexes", {{DataType::Object}, jsonField::Required}},
         {"hash", {{DataType::String}, jsonField::Required}},
         {"txbytes", {{DataType::String}, jsonField::Optional}},
         {"logs", {{DataType::String}, jsonField::Required}}});
}

const DataObject StateTestPostResult::asDataObject() const
{
    DataObject res;
    res["hash"] = m_hash.getCContent().asString();
    res["logs"] = m_log.getCContent().asString();
    res["indexes"]["data"] = m_dataInd;
    res["indexes"]["gas"] = m_gasInd;
    res["indexes"]["value"] = m_valInd;
    return res;
}

}  // namespace teststruct
}  // namespace test
