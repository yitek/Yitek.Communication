using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace Yitek.Communication
{
    /// <summary>
    /// 连接通道
    /// </summary>
    public interface IConnection
    {
        Task SendTextAsync(string text);

        Task SendBytesAsync(byte[] buffer, int start,int length);

        Task<RecievedData> RecieveAsync();
    }
}
