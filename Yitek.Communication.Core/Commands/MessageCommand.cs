using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace Yitek.Communication.Commands
{
    public class MessageCommand:ICommand
    {
        public async Task<object> ExecuteAsync(CommandParam param, object extras)
        {
            
            param.Creator.Chat.BoardcastCommand(param);
            return null;
        }
    }
}
