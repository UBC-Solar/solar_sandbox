
using System;
using System.Collections.Generic;
using System.Linq;
using Antmicro.Renode.Core;
using Antmicro.Renode.Exceptions;
using Antmicro.Renode.Logging;
using Antmicro.Renode.Peripherals.I2C;
using Antmicro.Renode.Peripherals.Sensor;
using Antmicro.Renode.Utilities;

namespace Antmicro.Renode.Peripherals.Miscellaneous
{
    public class SolarGPS : II2CPeripheral
    {
        private String message = "GPS!";
        
        public SolarGPS()
        {
            Reset();
        }
        public byte[] Read(int count = 1)
        {
            return System.Text.Encoding.ASCII.GetBytes(message);
        }

        public void Write(byte[] data)
        {
            
        }
        public void FinishTransmission()
        {
            
        }

        public void Reset()
        {
            return;
        }
    }
}