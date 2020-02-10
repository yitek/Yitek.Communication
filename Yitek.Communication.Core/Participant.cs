using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Net.WebSockets;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Threading.Tasks.Dataflow;
using Yitek.Communication.Commands;
using Yitek.Communication.Entities;

namespace Yitek.Communication
{
    public class Participant
    {
        public Guid Id { get { return this.Entity.Id; } }
        public WebSocket WebSocket { get; internal set; }

        public Chat Chat { get; private set; }

        public ParticipantEntity Entity { get; private set; }

        public Participant(Chat chat, ParticipantEntity entity,WebSocket ws) {
            this.Chat = chat;
            this.Entity = entity;
            this.WebSocket = ws;
        }

        public async Task SendCommandAsync(CommandParam cmdParam) {
            var text = JsonConvert.SerializeObject(cmdParam);
            byte[] sendBuffer = Encoding.UTF8.GetBytes(text);
            await this.WebSocket.SendAsync(new ArraySegment<byte>(sendBuffer), WebSocketMessageType.Text, endOfMessage: true, CancellationToken.None);
                            
        }

        public async Task SendTextAsync(string text)
        {
            byte[] sendBuffer = Encoding.UTF8.GetBytes(text);
            await this.WebSocket.SendAsync(new ArraySegment<byte>(sendBuffer), WebSocketMessageType.Text, endOfMessage: true, CancellationToken.None);

        }

        public async static Task ProcessClientDataAsync(WebSocket webSocket,object context) {
            var buffer = new byte[4096];
            Participant participant = null;
            WebSocketReceiveResult result = await webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);
            while (!result.CloseStatus.HasValue)
            {
                if (result.MessageType == WebSocketMessageType.Text)
                {
                    var cmdPar = new CommandParam(participant, buffer,result.Count);
                    if (participant == null)
                    {
                        var joinCommand = new JoinCommand();
                        participant = await joinCommand.ExecuteAsync(cmdPar, webSocket) as Participant;
                    }
                    else if (cmdPar.Name == "EXIT")
                    {
                        await new ExitCommand().ExecuteAsync(cmdPar, context);

                    }
                    else if (cmdPar.Name == "MESSAGE")
                    {
                        participant.Chat.BoardcastCommand(cmdPar);

                    }
                    else {
                        var cmd = participant.Chat.FindCommand(cmdPar);
                        if (cmd != null) cmd.ExecuteAsync(cmdPar,context);
                    }
                }

                result = await webSocket.ReceiveAsync(buffer, CancellationToken.None);
            }
        }




    }
}
