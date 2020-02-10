using System;
using System.Collections.Generic;
using System.Text;

namespace Yitek.Communication.Entities
{
    /// <summary>
    /// 聊天通信的参与者
    /// </summary>
    public class ParticipantEntity
    {
        /// <summary>
        /// 用户Id,一般跟账号的Id保持一致
        /// </summary>
        public Guid Id { get; set; }

        /// <summary>
        /// 用户名，用户唯一名，一般是email或手机号
        /// </summary>
        public string Username { get; set; }
        /// <summary>
        /// 在该聊天中的别名
        /// </summary>
        public string Alias { get; set; }

        public Guid ChatId { get; set; }
    }
}
