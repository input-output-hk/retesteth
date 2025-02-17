#include "StateTestTransaction.h"
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
StateTestTransaction::StateTestTransaction(DataObject const& _data)
{
    try
    {
        if (_data.atKey("to").asString().empty())
            m_creation = true;
        else
        {
            m_creation = false;
            m_to = spFH20(new FH20(_data.atKey("to")));
        }
        m_secretKey = spFH32(new FH32(_data.atKey("secretKey")));
        m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));

        std::vector<spAccessList> accessLists;
        if (_data.count("accessLists"))
        {
            for (auto const& el : _data.atKey("accessLists").getSubObjects())
            {
                if (el.type() == DataType::Null)
                    accessLists.push_back(spAccessList(0));
                else
                    accessLists.push_back(spAccessList(new AccessList(el)));
            }
        }

        size_t index = 0;
        if (_data.count("accessLists"))
            if (accessLists.size() != _data.atKey("data").getSubObjects().size())
                ETH_ERROR_MESSAGE("`AccessLists` array length must match `data` array length!");

        for (auto const& el : _data.atKey("data").getSubObjects())
        {
            DataObject dataInKey = el;
            if (accessLists.size())
            {
                if (accessLists.at(index).isEmpty())
                    m_databox.push_back(Databox(dataInKey, dataInKey.asString(), string()));
                else
                    m_databox.push_back(Databox(dataInKey, dataInKey.asString(), string(), accessLists.at(index)));
            }
            else
                m_databox.push_back(Databox(dataInKey, dataInKey.asString(), string()));

            index++;
        }
        for (auto const& el : _data.atKey("gasLimit").getSubObjects())
            m_gasLimit.push_back(el);
        for (auto const& el : _data.atKey("value").getSubObjects())
            m_value.push_back(el);

        requireJsonFields(_data, "StateTestTransaction " + _data.getKey(),
            {{"data", {{DataType::Array}, jsonField::Required}}, {"accessLists", {{DataType::Array}, jsonField::Optional}},
                {"gasLimit", {{DataType::Array}, jsonField::Required}}, {"gasPrice", {{DataType::String}, jsonField::Required}},
                {"nonce", {{DataType::String}, jsonField::Required}}, {"value", {{DataType::Array}, jsonField::Required}},
                {"to", {{DataType::String}, jsonField::Required}}, {"secretKey", {{DataType::String}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestTransaction parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
