

using System;
using System.Collections.Generic;
using Antmicro.Renode.Core;
using Antmicro.Renode.Core.Structure.Registers;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Peripherals.Timers;
using Antmicro.Renode.Peripherals.Miscellaneous;

namespace Antmicro.Renode.Peripherals.Miscellaneous
{
    public class RCC_PLL_MAPPER :IDoubleWordPeripheral
    {
        public RCC_PLL_MAPPER(STM32F103_RCC rccPeripheral)
        {
            
        }

        public void Reset()
        {
            
        }

        public uint ReadDoubleWord(long offset)
        {
            return rccPeripheral.ReadDoubleWord(offset);
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            rccPeripheral.WriteDoubleword(offset, value);
        }
    }
}