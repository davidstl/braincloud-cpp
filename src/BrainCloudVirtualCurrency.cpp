// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudVirtualCurrency.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"

namespace BrainCloud
{
    BrainCloudVirtualCurrency::BrainCloudVirtualCurrency(BrainCloudClient* in_client) : m_client(in_client) { }

    void BrainCloudVirtualCurrency::getCurrency(const std::string& in_vcId, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = in_vcId;

        ServerCall * sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::VirtualCurrencyGetCurrency, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudVirtualCurrency::getParentCurrency(const std::string& in_vcId, const std::string& in_levelName, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = in_vcId;
        message[OperationParam::VirtualCurrencyServiceLevelName.getValue()] = in_levelName;

        ServerCall * sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::VirtualCurrencyGetParentCurrency, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudVirtualCurrency::getPeerCurrency(const std::string& in_vcId, const std::string& in_peerCode, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = in_vcId;
        message[OperationParam::VirtualCurrencyServicePeerCode.getValue()] = in_peerCode;

        ServerCall * sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::VirtualCurrencyGetPeerCurrency, message, in_callback);
        m_client->sendRequest(sc);
    }
}
