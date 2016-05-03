// Copyright 2016 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudIdentity.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "braincloud/AuthenticationType.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"

namespace BrainCloud
{
    BrainCloudIdentity::BrainCloudIdentity(BrainCloudClient* in_client) : m_client(in_client) { }

    void BrainCloudIdentity::attachFacebookIdentity(const char * in_facebookId, const char * in_authenticationToken, IServerCallback * in_callback)
    {
        attachIdentity(in_facebookId, in_authenticationToken, AuthenticationType::Facebook, in_callback);
    }

    void BrainCloudIdentity::mergeFacebookIdentity(const char * in_facebookId, const char * in_authenticationToken, IServerCallback * in_callback)
    {
        mergeIdentity(in_facebookId, in_authenticationToken, AuthenticationType::Facebook, in_callback);
    }

    void BrainCloudIdentity::detachFacebookIdentity(const char * in_facebookId, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_facebookId, AuthenticationType::Facebook, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::attachEmailIdentity(const char * in_email, const char * in_password, IServerCallback * in_callback)
    {
        attachIdentity(in_email, in_password, AuthenticationType::Email, in_callback);
    }

    void BrainCloudIdentity::mergeEmailIdentity(const char * in_email, const char * in_password, IServerCallback * in_callback)
    {
        mergeIdentity(in_email, in_password, AuthenticationType::Email, in_callback);
    }

    void BrainCloudIdentity::detachEmailIdentity(const char * in_email, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_email, AuthenticationType::Email, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::attachUniversalIdentity(const char * in_userId, const char * in_password, IServerCallback * in_callback)
    {
        attachIdentity(in_userId, in_password, AuthenticationType::Universal, in_callback);
    }

    void BrainCloudIdentity::mergeUniversalIdentity(const char * in_userId, const char * in_password, IServerCallback * in_callback)
    {
        mergeIdentity(in_userId, in_password, AuthenticationType::Universal, in_callback);
    }

    void BrainCloudIdentity::detachUniversalIdentity(const char * in_userId, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_userId, AuthenticationType::Universal, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::attachGameCenterIdentity(const char * in_gameCenterId, IServerCallback * in_callback)
    {
        attachIdentity(in_gameCenterId, "", AuthenticationType::GameCenter, in_callback);
    }

    void BrainCloudIdentity::mergeGameCenterIdentity(const char * in_gameCenterId, IServerCallback * in_callback)
    {
        mergeIdentity(in_gameCenterId, "", AuthenticationType::GameCenter, in_callback);
    }

    void BrainCloudIdentity::detachGameCenterIdentity(const char * in_gameCenterId, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_gameCenterId, AuthenticationType::GameCenter, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::attachSteamIdentity(const char * in_steamId, const char * in_sessionTicket, IServerCallback * in_callback)
    {
        attachIdentity(in_steamId, in_sessionTicket, AuthenticationType::Steam, in_callback);
    }

    void BrainCloudIdentity::mergeSteamIdentity(const char * in_steamId, const char * in_sessionTicket, IServerCallback * in_callback)
    {
        mergeIdentity(in_steamId, in_sessionTicket, AuthenticationType::Steam, in_callback);
    }

    void BrainCloudIdentity::detachSteamIdentity(const char * in_steamId, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_steamId, AuthenticationType::Steam, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::attachGoogleIdentity(const char * in_googleId, const char * in_authenticationToken, IServerCallback * in_callback)
    {
        attachIdentity(in_googleId, in_authenticationToken, AuthenticationType::Google, in_callback);
    }

    void BrainCloudIdentity::mergeGoogleIdentity(const char * in_googleId, const char * in_authenticationToken, IServerCallback * in_callback)
    {
        mergeIdentity(in_googleId, in_authenticationToken, AuthenticationType::Google, in_callback);
    }

    void BrainCloudIdentity::detachGoogleIdentity(const char * in_googleId, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_googleId, AuthenticationType::Google, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::attachTwitterIdentity(const char * in_twitterId, const char * in_authenticationToken, const char * in_secret, IServerCallback * in_callback)
    {
        std::string buffer(in_authenticationToken);
        buffer += ":";
        buffer += in_secret;
        attachIdentity(in_twitterId, buffer.c_str(), AuthenticationType::Twitter, in_callback);
    }

    void BrainCloudIdentity::mergeTwitterIdentity(const char * in_twitterId, const char * in_authenticationToken, const char * in_secret, IServerCallback * in_callback)
    {
        std::string buffer(in_authenticationToken);
        buffer += ":";
        buffer += in_secret;
        mergeIdentity(in_twitterId, buffer.c_str(), AuthenticationType::Twitter, in_callback);
    }

    void BrainCloudIdentity::detachTwitterIdentity(const char * in_twitterId, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_twitterId, AuthenticationType::Twitter, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::attachParseIdentity(const char * in_parseId, const char * in_authenticationToken, IServerCallback * in_callback)
    {
        attachIdentity(in_parseId, in_authenticationToken, AuthenticationType::Parse, in_callback);
    }

    void BrainCloudIdentity::mergeParseIdentity(const char * in_parseId, const char * in_authenticationToken, IServerCallback * in_callback)
    {
        mergeIdentity(in_parseId, in_authenticationToken, AuthenticationType::Parse, in_callback);
    }

    void BrainCloudIdentity::detachParseIdentity(const char * in_parseId, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_parseId, AuthenticationType::Parse, in_continueAnon, in_callback);
    }

    void BrainCloudIdentity::switchToChildProfile(const char * in_childProfileId, const char * in_childGameId, bool in_forceCreate, IServerCallback * in_callback)
    {
        switchToChildProfile(in_childProfileId, in_childGameId, in_forceCreate, false, in_callback);
    }

    void BrainCloudIdentity::switchToSingletonChildProfile(const char * in_childGameId, bool in_forceCreate, IServerCallback * in_callback)
    {
        switchToChildProfile(NULL, in_childGameId, in_forceCreate, true, in_callback);
    }

    void BrainCloudIdentity::switchToParentProfile(const char * in_parentLevelName, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticationServiceAuthenticationLevelName.getValue()] = in_parentLevelName;

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::SwitchToParentProfile, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudIdentity::getChildProfiles(bool in_includeSummaryData, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStateServiceIncludeSummaryData.getValue()] = in_includeSummaryData;

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetChildProfiles, message, in_callback);
        m_client->sendRequest(sc);
    }

    // Private methods

    void BrainCloudIdentity::switchToChildProfile(const char * in_childProfileId, const char * in_childGameId, bool in_forceCreate, bool in_forceSingleton, IServerCallback * in_callback)
    {
        BrainCloudClient * brainCloudClientRef = m_client;
        Json::Value message;

        if (StringUtil::IsOptionalParameterValid(in_childProfileId)) {
            message[OperationParam::AuthenticateServiceAuthenticateProfileId.getValue()] = in_childProfileId;
        }

        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = in_childGameId;
        message[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = in_forceCreate;
        message[OperationParam::IdentityServiceForceSingleton.getValue()] = in_forceSingleton;

        message[OperationParam::AuthenticateServiceAuthenticateReleasePlatform.getValue()] = brainCloudClientRef->getReleasePlatform().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateCountryCode.getValue()] = brainCloudClientRef->getCountryCode().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateLanguageCode.getValue()] = brainCloudClientRef->getLanguageCode().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateTimeZoneOffset.getValue()] = brainCloudClientRef->getTimezoneOffset();

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::SwitchToChildProfile, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudIdentity::attachIdentity(const char * in_facebookId, const char * in_authenticationToken, AuthenticationType in_authenticationType, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::IdentityServiceExternalId.getValue()] = in_facebookId;
        message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
        message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_authenticationToken;

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Attach, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudIdentity::mergeIdentity(const char * in_facebookId, const char * in_authenticationToken, AuthenticationType in_authenticationType, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::IdentityServiceExternalId.getValue()] = in_facebookId;
        message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
        message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_authenticationToken;

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Merge, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudIdentity::detachIdentity(const char * in_facebookId, AuthenticationType in_authenticationType, bool in_continueAnon, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::IdentityServiceExternalId.getValue()] = in_facebookId;
        message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
        message[OperationParam::IdentityServiceConfirmAnonymous.getValue()] = in_continueAnon;

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Detach, message, in_callback);
        m_client->sendRequest(sc);
    }
}
