using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Text;
using Yitek.Communication.Entities;

namespace Yitek.Communication
{
    public class ChatManager
    {
        public readonly static ChatManager Default = new ChatManager();
        ConcurrentDictionary<Guid, Chat> _Chats;
        public ChatManager() {
            this._Chats = new ConcurrentDictionary<Guid, Chat>();
        }

        public virtual Chat TryGetChat(Guid id,ParticipantEntity participent) {
            Chat chat;
            this._Chats.TryGetValue(id,out chat);
            if (chat == null) {
                var chatEntity = new ChatEntity() { Id=id,Name = participent.Alias +"创建的对话"};
                chat = new Chat(chatEntity) { };
                this._Chats.TryAdd(chat.Id,chat);
            } 
            return chat;
        }
    }
}
