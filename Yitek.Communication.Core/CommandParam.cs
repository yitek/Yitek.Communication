
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using Yitek.Communication.Entities;

namespace Yitek.Communication
{
    public class CommandParam
    {
        
        public string Name { get; private set; }
        public ParticipantEntity Sender { get { return this.Creator?.Entity; } }
        [JsonIgnore]
        public Participant Creator { get; private set; }
        Dictionary<string, string> _Data;

        public IReadOnlyDictionary<string, string> Data {
            get { return this._Data; }
        }
        

        public string this[string index]
        {
            get
            {
                string value = null;
                this._Data.TryGetValue(index, out value);
                return value;
            }
        }
        public CommandParam(Participant participant, string name, Dictionary<string, string> data)
        {
            this.Name = name;
            this._Data = data;
            this.Creator = participant;
        }
        public CommandParam(Participant participant, byte[] buffer, int byteCount)
        {
            this._Data = new Dictionary<string, string>();
            this.Creator = participant;
            var at = 0;
            var startAt = 0;
            string key = null;
            while (at < byteCount)
            {
                var ch = buffer[at++];
                if (ch == 0)
                {
                   
                    var data = UTF8Encoding.UTF8.GetString(buffer, startAt, at - startAt-1);
                    startAt = at;
                    if (this.Name == null) this.Name = data;
                    else if (key == null) key = data;
                    else
                    {
                        this._Data.Add(key, data);
                        key = null;
                    }
                }
            }
            if (key != null)
            {
                var data = UTF8Encoding.UTF8.GetString(buffer, startAt, at - startAt);
                this._Data.Add(key, data);
            }
        }

        
    }
}
 