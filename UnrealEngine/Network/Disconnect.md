ListenServer 关闭当前Map.

Client:
UControlChannel::ReceivedBunch( FInBunch& Bunch )
    UWorld::NotifyControlMessage(UNetConnection* Connection, uint8 MessageType, class FInBunch& Bunch)
        UEngine::BroadcastNetworkFailure(UWorld * World, UNetDriver *NetDriver, FailureReceived, Host closed the connection)
            NetworkFailureEvent.Broadcast(World, NetDriver, FailureType, ErrorString);
            UGameViewportClient::PeekNetworkFailureMessages
            UEditorEngine::HandleNetworkFailure
                Call Super --> UEngine::HandleNetworkFailure
                    UEngine::HandleNetworkFailure_NotifyGameInstance
                    CallHandleDisconnectForFailure(World, NetDriver)
                        FGameDelegates::Get().GetHandleDisconnectDelegate().Broadcast(InWorld, NetDriver)
                        InWorld->GetGameInstance()->GetOnlineSession()->HandleDisconnect(InWorld, NetDriver)
                        UOnlineSession::HandleDisconnect
                            UEngine::HandleDisconnect
                                UEngine::SetClientTravel // 设置要在下一个Tick切关卡 ?close
        UNetConnection::Close()

UWorld::TickNetClient( float DeltaSeconds )
    UEngine::BroadcastNetworkFailure(ConnectionLost,Your connection to the host has been lost)
        UGameViewportClient::PeekNetworkFailureMessages
        UGameViewportClient::PeekNetworkFailureMessages
        UEditorEngine::HandleNetworkFailure
        UEngine::HandleNetworkFailure
            UEngine::HandleNetworkFailure_NotifyGameInstance
            CallHandleDisconnectForFailure
                FGameDelegates::Get().GetHandleDisconnectDelegate().Broadcast(InWorld, NetDriver);
                UOnlineSession::HandleDisconnect
                    UEngine::SetClientTravel






