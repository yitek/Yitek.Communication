(function (factory) {
    if (typeof module === "object" && typeof module.exports === "object") {
        var v = factory(require, exports);
        if (v !== undefined) module.exports = v;
    }
    else if (typeof define === "function" && define.amd) {
        define(["require", "exports"], factory);
    }
})(function (require, exports) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    var Chat = /** @class */ (function () {
        function Chat(opts) {
            this.url = opts.url || "ws://127.0.0.1:5000/im";
            this.user = opts.user;
            this.id = opts.id;
            this.participants = {};
        }
        Chat.prototype.join = function (cmd_handler) {
            var _this = this;
            var wsImpl = window.WebSocket || window.MozWebSocket;
            // create a new websocket and connect
            var ws = this.ws = new wsImpl(this.url);
            ws.onopen = function () {
                console.info("chat[" + _this.id + "]已经连接上服务器，正在发送JOIN命令...");
                var joinCmdPar = {
                    Name: "JOIN",
                    Data: {
                        "UserId": _this.user.Id,
                        "ChatId": _this.id,
                        "Username": _this.user.Username,
                        "Alias": _this.user.Alias
                    }
                };
                var joinCmdPkg = makeCommandPackage(joinCmdPar);
                ws.send(joinCmdPkg);
            };
            ws.onclose = function () {
                console.info("chat" + _this.id + "\u5DF2\u7ECF\u4ECE\u670D\u52A1\u5668\u65AD\u5F00...");
            };
            // when data is comming from the server, this metod is called
            ws.onmessage = function (evt) {
                console.log("服务器端发送了消息", evt);
                var cmd = JSON.parse(evt.data);
                if (cmd_handler)
                    cmd_handler(cmd, _this);
            };
        };
        Chat.prototype.sendCommand = function (cmd) {
            var cmdPkg = makeCommandPackage(cmd);
            this.ws.send(cmdPkg);
        };
        return Chat;
    }());
    exports.Chat = Chat;
    var ChatContent = /** @class */ (function () {
        function ChatContent(chat, user, container) {
            this.chat = chat;
            this.user = user;
            var elem = this.element = document.createElement("div");
            if (container)
                container.appendChild(elem);
            elem.className = "chat-content";
            elem.style.overflow = "auto";
            this.messages = [];
        }
        ChatContent.prototype.appendMessage = function (msg) {
            this.messages.push(msg);
            if (this.messages.length / 20 == 0)
                this.element.innerHTML = "";
            /*let ellapse = msg.Time.valueOf() - this.time.valueOf();
            if (ellapse > 1000 * 60) {
                let tElem = document.createElement("div");
                tElem.className = "time";
                tElem.innerHTML = msg.Time.toString();
                this.element.appendChild(tElem);
            }*/
            var elem = document.createElement("div");
            elem.innerHTML = "<div class=\"sender " + (msg.Sender.Id == this.user.Id ? 'self' : '') + "\">\n<span class='uname'>" + msg.Sender.Alias + "(" + msg.Sender.Username + ")</span>\n<span>" + msg.Data.Content + "</span>\n";
            this.element.appendChild(elem);
        };
        return ChatContent;
    }());
    exports.ChatContent = ChatContent;
    var ChatSend = /** @class */ (function () {
        function ChatSend(chat, user, container) {
            var _this = this;
            this.chat = chat;
            this.user = user;
            var elem = this.element = document.createElement("div");
            if (container)
                container.appendChild(elem);
            elem.className = "chat-send";
            elem.innerHTML = "<textarea id='chat-send-content'></textarea><a class='submit-chat'>发送</a>";
            this.input = elem.firstChild;
            var btn = elem.lastChild;
            btn.onclick = function () {
                var ctn = _this.input.value;
                if (!ctn)
                    return;
                var cmdPar = {
                    Name: "MESSAGE",
                    Sender: user,
                    Time: new Date(),
                    Data: { Content: ctn }
                };
                chat.sendCommand(cmdPar);
                _this.input.value = "";
            };
        }
        return ChatSend;
    }());
    exports.ChatSend = ChatSend;
    var ChatUI = /** @class */ (function () {
        function ChatUI(chat, container) {
            this.chat = chat;
            var user = chat.user;
            this.element = document.createElement("div");
            if (container)
                container.appendChild(this.element);
            this.chatContent = new ChatContent(chat, user, this.element);
            this.chatSend = new ChatSend(chat, user, this.element);
        }
        ChatUI.prototype.appendMessage = function (msg) {
            this.chatContent.appendMessage(msg);
        };
        return ChatUI;
    }());
    exports.ChatUI = ChatUI;
    function makeCommandPackage(par) {
        var diagram = [];
        diagram.push(par.Name);
        for (var n in par.Data) {
            diagram.push(n);
            diagram.push(par.Data[n]);
        }
        return diagram.join('\0');
    }
    exports.COM = {
        Chat: Chat, ChatUI: ChatUI
    };
    if (typeof window !== 'undefined')
        window.COM = exports.COM;
});
//# sourceMappingURL=communication.js.map