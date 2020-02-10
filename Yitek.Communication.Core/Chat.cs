using Newtonsoft.Json;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Net.WebSockets;
using System.Text;
using System.Threading.Tasks;
using Yitek.Communication.Commands;
using Yitek.Communication.Entities;

namespace Yitek.Communication
{
    public class Chat
    {
        public ChatEntity Entity{ get; private set; }
        ConcurrentDictionary<Guid, Participant> _Participants;

        Dictionary<string, ICommand> _Commands = new Dictionary<string, ICommand>() {
            { "EXIT",new ExitCommand()},
            { "JOIN",new JoinCommand()},
            { "MESSAGE",new MessageCommand() }
        };

        public Guid Id { get { return this.Entity.Id; } }
        public string Name { get { return this.Entity.Name; } }
        public Chat(ChatEntity entity) {
            this.Entity = entity;
            this._Participants = new ConcurrentDictionary<Guid, Participant>();
        }

        public virtual Participant AddParticipant(ParticipantEntity entity,WebSocket ws) {
            entity.ChatId = this.Id;
            var participant = new Participant(this, entity,ws);
            this._Participants.AddOrUpdate(entity.Id, participant, (id, old) => participant);
            return participant;
            //if (this._Participants.TryAdd(entity.Id, participant)) return participant;
            //else return null;
        }

        public ICommand FindCommand(CommandParam cmdPar) {
            ICommand cmd = null;
            this._Commands.TryGetValue(cmdPar.Name,out cmd);
            return cmd;
        }

        public Participant RemoveParticipant(Guid userId) {
            Participant participant = null;
            this._Participants.TryRemove(userId, out participant);
            return participant;
        }

        public void BoardcastCommand(CommandParam cmdPar) {
            var jsonText = JsonConvert.SerializeObject(cmdPar);
            foreach (var joiner in this._Participants.Values) {
                joiner.SendTextAsync(jsonText);  
            }
        }

        public IList<ParticipantEntity> GetParticipants() {
            var list = new List<ParticipantEntity>();
            foreach (var p in this._Participants.Values) {
                list.Add(p.Entity);
            }
            return list;
        }
    }
}
