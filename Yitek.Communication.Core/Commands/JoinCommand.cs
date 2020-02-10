using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Net.WebSockets;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;
using Yitek.Communication.Entities;

namespace Yitek.Communication.Commands
{
    public class JoinCommand : ICommand
    {
        public ChatManager ChatManager { get; private set; }
        public JoinCommand() {
            this.ChatManager = ChatManager.Default;
        }
        public async Task<object> ExecuteAsync(CommandParam param,object extras)
        {
            var webSocket = (WebSocket)extras;
            var chatId = new Guid(param["ChatId"]);
            var entity = new ParticipantEntity() {
                Id =new Guid( param["UserId"]),
                Username = param["Username"],
                Alias = param["Alias"]
            };
            if (entity.Alias == null) entity.Alias = entity.Username;
            var chat = this.ChatManager.TryGetChat(chatId,entity);
            var joiner = chat.AddParticipant(entity,webSocket);
            chat.BoardcastCommand(param);
            var participants = chat.GetParticipants();
            var cmdParam = new CommandParam(joiner, "PARTICIPANTS", new Dictionary<string, string>() { { "List",JsonConvert.SerializeObject(participants)} });
            await joiner.SendCommandAsync(cmdParam);
            return joiner;
        }
    }
}
