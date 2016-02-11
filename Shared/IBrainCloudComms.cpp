
#include "IBrainCloudComms.h"
#include "BrainCloudClient.h"

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <ctype.h>
#include <string>
#include <list>
#include "HTTP/IFileUploader.h"


namespace BrainCloud {

	////////////////////////////////////////////////////////
    // Public methods
    ////////////////////////////////////////////////////////
	
    IBrainCloudComms::IBrainCloudComms()
        : _packetId(0)
        , _expectedPacketId(NO_PACKET_EXPECTED)
        , _retryCount(0)
        , _packetSendTimeMillis(0)
		, _isInitialized(false)
        , _isAuthenticated(false)
        , _heartbeatInterval(0)
        , _immediateRetryOnError(false)
        , _loggingEnabled(false)
        , _authenticationTimeoutMillis(DEFAULT_AUTHENTICATION_TIMEOUT_MILLIS)
        , _oldStyleStatusMessageErrorCallback(false)
        , _eventCallback(NULL)
        , _fileUploadCallback(NULL)
        , _rewardCallback(NULL)
        , _uploadLowTransferRateTimeoutSecs(120)
        , _uploadLowTransferRateThresholdBytesPerSec(50)
        , _uploadOverallTimeoutSecs(0)
        , _statusCodeCache(0)
        , _reasonCodeCache(0)
    {
        setPacketTimeoutsToDefault();
    }
    
    IBrainCloudComms::~IBrainCloudComms()
    {
    }

    void IBrainCloudComms::enableLogging(bool shouldEnable)
    {
        _loggingEnabled = shouldEnable;
    }
    
	bool IBrainCloudComms::isInitialized()
	{
		return _isInitialized;
	}

    bool IBrainCloudComms::isAuthenticated()
    {
        return _isAuthenticated;
    }
    
    void IBrainCloudComms::setHeartbeatInterval(int milliseconds)
    {
        _heartbeatInterval = milliseconds;
    }
    
    void IBrainCloudComms::setImmediateRetryOnError(bool value)
    {
        _immediateRetryOnError = value;
    }
    
    const std::string& IBrainCloudComms::getGameId() const
    {
        return _gameId;
    }
    
    void IBrainCloudComms::clearSessionId()
    {
        _sessionId.clear();
    }
    
    const std::string& IBrainCloudComms::getSessionId() const
    {
        return _sessionId;
    }
    
    void IBrainCloudComms::setSessionId( const char * sessionId )
    {
        _sessionId.clear();
        _sessionId.append( sessionId );
    }
    
   	void IBrainCloudComms::setServerUrl( const char * serverUrl )
    {
        _serverUrl.clear();
        _serverUrl.append( serverUrl );
    }
    
    const std::vector<int> & IBrainCloudComms::getPacketTimeouts()
    {
        return _packetTimeouts;
    }
    
    void IBrainCloudComms::setPacketTimeouts(const std::vector<int> & in_packetTimeouts)
    {
        _packetTimeouts = in_packetTimeouts;
    }
    
    void IBrainCloudComms::setPacketTimeoutsToDefault()
    {
        _packetTimeouts.clear();
        _packetTimeouts.push_back(10);
        _packetTimeouts.push_back(10);
        _packetTimeouts.push_back(10);
    }
    
    void IBrainCloudComms::setAuthenticationPacketTimeout(int in_timeoutSecs)
    {
        if (in_timeoutSecs > 0)
        {
            _authenticationTimeoutMillis = in_timeoutSecs * 1000;
        }
    }
    
    int IBrainCloudComms::getAuthenticationPacketTimeout()
    {
        return _authenticationTimeoutMillis / 1000;
    }
    
    void IBrainCloudComms::setOldStyleStatusMessageErrorCallback(bool in_enabled)
    {
        _oldStyleStatusMessageErrorCallback = in_enabled;
    }
    
    int IBrainCloudComms::getUploadLowTransferRateTimeout()
    {
        return _uploadLowTransferRateTimeoutSecs;
    }

    void IBrainCloudComms::setUploadLowTransferRateTimeout(int in_timeoutSecs)
    {
        _uploadLowTransferRateTimeoutSecs = in_timeoutSecs;
    }
    
    int IBrainCloudComms::getUploadLowTransferRateThreshold()
    {
        return _uploadLowTransferRateThresholdBytesPerSec;
    }
    
    void IBrainCloudComms::setUploadLowTransferRateThreshold(int in_bytesPerSec)
    {
        _uploadLowTransferRateThresholdBytesPerSec = in_bytesPerSec;
    }
    
    //////////////////////////////////////////////////////
    // Protected methods
    //////////////////////////////////////////////////////
    
    void IBrainCloudComms::setCredentials( const Json::Value& in_jsonAuthenticationResponse )
    {
        clearSessionId();
        if (in_jsonAuthenticationResponse["data"] != Json::nullValue && in_jsonAuthenticationResponse["data"]["sessionId"] != Json::nullValue)
        {
            std::string sessionId = in_jsonAuthenticationResponse["data"].get("sessionId", "").asString();
            std::string profileId = in_jsonAuthenticationResponse["data"].get("profileId", "").asString();
            setSessionId(sessionId.c_str());
            BrainCloudClient::getInstance()->getAuthenticationService()->setProfileId(profileId.c_str());
        }
    }
    
    void IBrainCloudComms::filterIncomingMessages(const ServerCall* servercall, const Json::Value& response)
    {
        //This is a hook to perform processing on any messages that come in, before they are sent to the calling application.

		if (servercall->getService() == ServiceName::AuthenticateV2 && servercall->getOperation() == ServiceOperation::Authenticate)
		{
            _isAuthenticated = true;
			BrainCloudClient::getInstance()->setCredentials(response);
            
            // pick up the timeout interval from the auth call
            if (_heartbeatInterval == 0)
            {
                if (response["data"] != Json::nullValue && response["data"]["playerSessionExpiry"] != Json::nullValue)
                {
                    int sessionTimeout = response["data"]["playerSessionExpiry"].asInt();
                    sessionTimeout = (int) sessionTimeout * 0.85;
                    if (sessionTimeout > 30) // minimum 30 secs
                    {
                        _heartbeatInterval = sessionTimeout * 1000;
                    }
                }
            }
		}
		else if (servercall->getService() == ServiceName::PlayerState
                 && (servercall->getOperation() == ServiceOperation::FullReset
                     || servercall->getOperation() == ServiceOperation::Logout))
        {
            _isAuthenticated = false;
            _sessionId.clear();
            BrainCloudClient::getInstance()->getAuthenticationService()->clearSavedProfileId();
        }
        else if (servercall->getService() == ServiceName::File
                 && (servercall->getOperation() == ServiceOperation::PrepareUserUpload))
        {
            startFileUpload(response);
        }
    }
    
    void IBrainCloudComms::runCallbacksFileUpload()
    {
        // go through map and check for completions/failures
        // and remove uploads that are done

        // if we are now transferring fewer than the max,
        // check if any uploads are pending and start them

        // we assume that all mutex locking has been done from the calling method!

        tFileUploadsIterator it;
        for (it = _fileUploads.begin(); it != _fileUploads.end();)
        {
            IFileUploader::eFileUploaderStatus status = it->second->getStatus();
            bool cleanup = false;
            switch (status)
            {
            case IFileUploader::UPLOAD_STATUS_COMPLETE_SUCCESS:
            {
                if (!it->second->isThreadRunning())
                {
                    if (_fileUploadCallback != NULL)
                    {
                        _fileUploadCallback->fileUploadCompleted(it->first.c_str(), it->second->getHttpResponse());
                    }
                    cleanup = true;
                }
                break;
            }
            case IFileUploader::UPLOAD_STATUS_COMPLETE_FAILED:
            {
                if (!it->second->isThreadRunning())
                {
                    if (_fileUploadCallback != NULL)
                    {
                        _fileUploadCallback->fileUploadFailed(it->first.c_str(), it->second->getHttpStatus(), it->second->getErrorReasonCode(), it->second->getHttpResponse());
                    }
                    cleanup = true;
                }
                break;
            }
            default:
                break;
            }
            if (cleanup)
            {
                delete it->second;
                _fileUploads.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }
    
    void IBrainCloudComms::createJsonErrorResponse(int in_statusCode,
                                                   int in_reasonCode,
                                                   const std::string & in_statusMessage,
                                                   std::string & out_jsonErrorResponse)
    {
        Json::Value root;
        Json::FastWriter writer;
        
        root["status"] = in_statusCode;
        root["reason_code"] = in_reasonCode;
        root["status_message"] = in_statusMessage;
        root["severity"] = "ERROR";
        
        out_jsonErrorResponse = writer.write(root);
    }

}  // end namespace