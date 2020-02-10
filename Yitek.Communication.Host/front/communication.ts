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
export class Chat {
    user: IParticipant;
    url: string;
    id: string;
    name: string;
    participants: { [id: string]: IParticipant }
    ws: WebSocket;
    constructor(opts: IChatOpts) {
        this.url = opts.url || "ws://127.0.0.1:5000/im";
        this.user = opts.user;
        this.id = opts.id;
        this.participants = {};
    }
    join(cmd_handler?:any) {
        let wsImpl = (window as any).WebSocket || (window as any).MozWebSocket;
        // create a new websocket and connect
        let ws = this.ws = new wsImpl(this.url);

        ws.onopen = () => {
            console.info("chat[" + this.id + "]已经连接上服务器，正在发送JOIN命令...");
            var joinCmdPar = {
                Name: "JOIN",
                Data: {
                    "UserId": this.user.Id,//"2E30CFCA-E015-4525-BCF4-7B3F7BB2AB83"
                    "ChatId": this.id,
                    "Username": this.user.Username,
                    "Alias": this.user.Alias
                }
            };
            var joinCmdPkg = makeCommandPackage(joinCmdPar);
            ws.send(joinCmdPkg);
        };

        ws.onclose = ()=> {
            console.info(`chat${this.id}已经从服务器断开...`);
        }

        // when data is comming from the server, this metod is called
        ws.onmessage = (evt) =>{
            console.log("服务器端发送了消息",evt);
            let cmd = JSON.parse(evt.data);
            if(cmd_handler)cmd_handler(cmd,this);
        };
    }

    sendCommand(cmd) {
        var cmdPkg = makeCommandPackage(cmd);
        this.ws.send(cmdPkg);
    }
}
export interface IMessage {
    Type: string;
    Data:any;
    Sender: IParticipant;
    Time: Date;
}
export class ChatContent {
    element: HTMLElement;
    messages: IMessage[];
    time: Date;

    constructor(public chat: Chat, public user: IParticipant, container?: HTMLElement) {
        let elem = this.element = document.createElement("div");
        if (container) container.appendChild(elem);
        elem.className = "chat-content";
        elem.style.overflow = "auto";
        this.messages=[];
    }
    appendMessage(msg: IMessage) {
        this.messages.push(msg);
        if (this.messages.length / 20 == 0) this.element.innerHTML = "";
        /*let ellapse = msg.Time.valueOf() - this.time.valueOf();
        if (ellapse > 1000 * 60) {
            let tElem = document.createElement("div");
            tElem.className = "time";
            tElem.innerHTML = msg.Time.toString();
            this.element.appendChild(tElem);
        }*/
        let elem = document.createElement("div");
        elem.innerHTML = `<div class="sender ${msg.Sender.Id == this.user.Id ? 'self' : ''}">
<span class='uname'>${msg.Sender.Alias}(${msg.Sender.Username})</span>
<span>${msg.Data.Content}</span>
`;
        this.element.appendChild(elem);
    }
}

export class ChatSend {
    element: HTMLElement;
    input: any;
    constructor(public chat: Chat, public user: IParticipant, container: HTMLElement) {
        let elem = this.element = document.createElement("div");
        if (container) container.appendChild(elem);
        elem.className = "chat-send";
        elem.innerHTML = "<textarea id='chat-send-content'></textarea><a class='submit-chat'>发送</a>";
        this.input = elem.firstChild;
        let btn = elem.lastChild as HTMLElement;
        btn.onclick = () => {
            let ctn = this.input.value;
            if (!ctn) return;
            let cmdPar = {
                Name: "MESSAGE",
                Sender:user,
                Time:new Date(),
                Data: {Content:ctn}
            };
            chat.sendCommand(cmdPar);
            this.input.value="";
        };
    }
    
}

export class ChatUI {
    element: HTMLElement;
    chatContent: ChatContent;
    chatSend: ChatSend;
    constructor(public chat: Chat, container: HTMLElement) {
        let user = chat.user;
        this.element = document.createElement("div");
        if (container) container.appendChild(this.element);
        this.chatContent = new ChatContent(chat, user, this.element);
        this.chatSend = new ChatSend(chat, user, this.element);
    }
    appendMessage(msg){
        this.chatContent.appendMessage(msg);
    }
}

function makeCommandPackage(par) {
    let diagram = [];
    diagram.push(par.Name);
    for (let n in par.Data) {
        diagram.push(n);
        diagram.push(par.Data[n]);
    }
    return diagram.join('\0');
}
export let COM: any = {
    Chat,ChatUI    
};
if(typeof window !=='undefined') (window as any).COM = COM;