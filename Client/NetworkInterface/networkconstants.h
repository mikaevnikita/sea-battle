#ifndef NETWORKCONSTANTS_H
#define NETWORKCONSTANTS_H

typedef quint64 BlockSize;
typedef quint8 NetworkCommand;
typedef quint8 AuthStatus;
typedef quint8 SessionStatus;
typedef quint8 InviteStatus;
typedef quint8 CloseGameStatus;
typedef quint8 GameStepAnswer;

enum class InviteStatuses: InviteStatus{
    Failed,
    Accepted,
    Rejected,
};

enum class GameStepAnswers: GameStepAnswer{
    Hit,
    Miss
};

enum class AuthStatuses: AuthStatus{
    Failed,
    Ok,
    NeedAuth
};

enum class CloseGameStatuses:CloseGameStatus{
    GamerIsLeave,
    Win,
    Lose
};

enum class NetworkCommands: NetworkCommand{
    AuthRequest,
    UsersListRequest,
    NewUserIsAdded,
    UserIsRemoved,
    InviteRequest,
    InviteStatus,
    CloseGame,
    CloseInvite,
    SendCoords,
    AnswerOnGameStep,
    ChatBroadcastMessage,
    ChatPrivateMessage,
    ChatSessionMessage
};

enum class SessionStatuses: SessionStatus{
    Invite,
    Active
};


#endif // NETWORKCONSTANTS_H
