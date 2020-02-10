using System;
using System.Collections.Generic;
using System.Text;

namespace Yitek.Communication.Entities
{
    public class ChatEntity
    {
        /// <summary>
        /// 聊天的唯一Id
        /// </summary>
        public Guid Id { get; set; }

        /// <summary>
        /// 聊天名称
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// 参与者
        /// </summary>
        public IList<ParticipantEntity> Participants { get; set; }
    }
}
