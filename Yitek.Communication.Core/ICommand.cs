using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace Yitek.Communication
{
    public interface ICommand
    {
        Task<object> ExecuteAsync(CommandParam cmdPar,object extras);
    }
}
