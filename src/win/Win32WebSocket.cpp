#include "braincloud/internal/win/Win32WebSocket.h"

namespace BrainCloud
{
    IWebSocket* IWebSocket::create(const std::string& address, int port, const std::map<std::string, std::string>& headers)
    {
        return new Win32WebSocket(address, port, headers);
    }

    Win32WebSocket::~Win32WebSocket()
    {
        close();
    }

    Win32WebSocket::Win32WebSocket(const std::string& uri, int port, const std::map<std::string, std::string>& headers)
        : _isValid(false)
        , _isConnecting(true)
        , _authHeaders(headers)
        , _webSocketHandle(0)
    {
        HRESULT result;

        // Create handle
        WEB_SOCKET_PROPERTY properties[] = { {} };
        result = WebSocketCreateClientHandle(properties, 0, &_webSocketHandle);
        if (result != S_OK)
        {
            _isConnecting = false;
            return;
        }

        // Begin handshake
        result = WebSocketBeginClientHandshake(
            _webSocketHandle,
            PCSTR                         *pszSubprotocols,
            ULONG                         ulSubprotocolCount,
            PCSTR                         *pszExtensions,
            ULONG                         ulExtensionCount,
            const PWEB_SOCKET_HTTP_HEADER pInitialHeaders,
            ULONG                         ulInitialHeaderCount,
            PWEB_SOCKET_HTTP_HEADER       *pAdditionalHeaders,
            ULONG                         *pulAdditionalHeaderCount
        );

        int tmp;
        tmp = 5;
    }

    bool Win32WebSocket::isValid()
    {
        return false;
        // std::unique_lock<std::mutex> lock(_mutex);

        // if (_isConnecting)
        // {
        //     _connectionCondition.wait(lock, [this]() { return !_isConnecting; });
        // }

        // return _isValid;
    }

    void Win32WebSocket::send(const std::string& message)
    {
        // std::unique_lock<std::mutex> lock(_mutex);
        // _sendQueue.push(message);
    }

    std::string Win32WebSocket::recv()
    {
        // std::unique_lock<std::mutex> lock(_recvMutex);

        // if (!_recvQueue.empty())
        // {
        //     std::string message = _recvQueue.front();
        //     _recvQueue.pop();
        //     return message;
        // }

        // _recvCondition.wait(lock, [this]()
        // {
        //     return !_isValid || !_recvQueue.empty();
        // });

        // if (_isValid && !_recvQueue.empty())
        // {
        //     std::string message = _recvQueue.front();
        //     _recvQueue.pop();
        //     return message;
        // }

        return "";
    }

    void Win32WebSocket::close()
    {
        // Stop and clean send queue
        // {
        //     std::unique_lock<std::mutex> lock(_mutex);
        //     while (!_sendQueue.empty()) _sendQueue.pop();
        //     _isValid = false;
        //     _isConnecting = false;
        //     _connectionCondition.notify_all();
        // }

        // // Stop and clean recving
        // {
        //     std::unique_lock<std::mutex> lock(_recvMutex);
        //     while (!_recvQueue.empty()) _recvQueue.pop();
        //     _recvCondition.notify_all();
        // }

        // // Join update thread
        // if (_updateThread.joinable())
        // {
        //     _updateThread.join();
        // }
    }
};
