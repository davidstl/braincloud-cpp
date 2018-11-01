#ifndef _WIN32WEBSOCKER_H_
#define _WIN32WEBSOCKER_H_

#include "braincloud/internal/IWebSocket.h"

#include <Windows.h>
#include <websocket.h>

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace BrainCloud
{
    class Win32WebSocket : public IWebSocket
    {
    public:
        virtual ~Win32WebSocket();

        virtual bool isValid();

        virtual void send(const std::string& message);
        virtual std::string recv();

        virtual void close();

    protected:
        friend class IWebSocket;

        Win32WebSocket(const std::string& address, int port, const std::map<std::string, std::string>& headers);

    private:
        // State
        bool _isValid;
        bool _isConnecting;

        // Update and send queue
        std::mutex _mutex;
        std::queue<std::string> _sendQueue;
        std::vector<uint8_t> _sendBuffer;
        std::thread _updateThread;

        // Connection
        std::condition_variable _connectionCondition;

        // Receiving queue
        std::mutex _recvMutex;
        std::queue<std::string> _recvQueue;
        std::condition_variable _recvCondition;

        // Context
        std::map<std::string, std::string> _authHeaders;
        WEB_SOCKET_HANDLE _webSocketHandle;
    };
};

#endif /* _DEFAULTTCPSOCKER_H_ */
