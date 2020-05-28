#pragma once 
#include <winsock2.h>  
/**
 * Enumerates BSD socket state return values.
 */
enum class ESocketBSDReturn : UINT8
{
    Yes,
    No,
    EncounteredError,
};

/**
 * Enumerates BSD socket state parameters.
 */
enum class ESocketBSDParam : UINT8
{
    CanRead,
    CanWrite,
    HasError,
};

/** Indicates the connection state of the socket */
enum ESocketConnectionState
{
    SCS_NotConnected,
    SCS_Connected,
    /** Indicates that the end point refused the connection or couldn't be reached */
    SCS_ConnectionError
};

class SocketServer
{
public:
    SocketServer(unsigned short port);
    ~SocketServer();
    ESocketConnectionState GetConnectionState(SOCKET socket);
    ESocketConnectionState GetClientConnectionState()
    {
        return GetConnectionState(client);
    }
    bool WiteForConnection();
public:
    SOCKET GetNativeSocket() { return serverSocket; }
    SOCKET GetNativeClientSocket() { return client; }
    void ResetNativeClientSocket() { closesocket(client); client = NULL; }
    ESocketConnectionState MyGetConnectionState(SOCKET socket);
    bool CanRead(SOCKET socket)
    {
        timeval t = { 0,0 };

        int PendingDataSize;
        // make sure socket has no error state
        if (HasState(socket,ESocketBSDParam::CanRead,&t) == ESocketBSDReturn::Yes)
        {
            // See if there is any pending data on the read socket
            /*if (ioctlsocket(socket, FIONREAD, (u_long*)(&PendingDataSize)) == 0)
            {
                return (PendingDataSize > 0);
            }*/
            return true;
        }
        return false;
    }
    bool IsClientCanRead() { CanRead(client); }

    SOCKET serverSocket;
    SOCKET client;
private:

    ESocketBSDReturn HasState(SOCKET socket,ESocketBSDParam State, timeval* TimePointer);
    bool initServer(unsigned short port);
private:
    
};

