using System;
using System.Collections.Generic;
using System.Net.WebSockets;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Yitek.Communication.Commands
{
    public class ExitCommand:ICommand
    {
        public ExitCommand()
        {
            
        }

        public async Task<object> ExecuteAsync(CommandParam param, object extras) {
            //从聊天中移除
            param.Creator.Chat.RemoveParticipant(param.Creator.Id);
            // 关闭Socket
            await param.Creator.WebSocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Bye for now", CancellationToken.None);
            //通知下线
            param.Creator.Chat.BoardcastCommand(param);
            return null;
        }
    }
}
