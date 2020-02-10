export interface IParticipant {
    Id: string;
    Username: string;
    Alias: string;
}
export interface IChatOpts {
    url: string;
    id: string;
    user: IParticipant;
}
export declare class Chat {
    user: IParticipant;
    url: string;
    id: string;
    name: string;
    participants: {
        [id: string]: IParticipant;
    };
    ws: WebSocket;
    constructor(opts: IChatOpts);
    join(cmd_handler?: any): void;
    sendCommand(cmd: any): void;
}
export interface IMessage {
    Type: string;
    Data: any;
    Sender: IParticipant;
    Time: Date;
}
export declare class ChatContent {
    chat: Chat;
    user: IParticipant;
    element: HTMLElement;
    messages: IMessage[];
    time: Date;
    constructor(chat: Chat, user: IParticipant, container?: HTMLElement);
    appendMessage(msg: IMessage): void;
}
export declare class ChatSend {
    chat: Chat;
    user: IParticipant;
    element: HTMLElement;
    input: any;
    constructor(chat: Chat, user: IParticipant, container: HTMLElement);
}
export declare class ChatUI {
    chat: Chat;
    element: HTMLElement;
    chatContent: ChatContent;
    chatSend: ChatSend;
    constructor(chat: Chat, container: HTMLElement);
    appendMessage(msg: any): void;
}
export declare let COM: any;
