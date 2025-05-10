using Antmicro.Renode.Peripherals.Bus;
using System.Collections.Generic;
using Antmicro.Renode.Core;
using Antmicro.Renode.Core.Structure.Registers;
using Antmicro.Renode.Logging;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Time;
using Antmicro.Renode.Utilities;
using Antmicro.Renode.Peripherals.Timers;

namespace Antmicro.Renode.Peripherals.Timers
{
    public class STM32F103_RTC : IDoubleWordPeripheral, IKnownSize
    {
        public STM32F103_RTC(IMachine machine, long wakeupTimerFrequency = 32768)
        {
            
        }
        public long Size => 0x400;

        public uint ReadDoubleWord(long offset)
        {
            return 0;
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            return;
        }

        public void Reset()
        {
            return;
        }
        
    }
}