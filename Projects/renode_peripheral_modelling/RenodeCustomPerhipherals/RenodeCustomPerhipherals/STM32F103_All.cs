using Antmicro.Renode.Peripherals.Bus;
using System.Collections.Generic;
using Antmicro.Renode.Core;
using Antmicro.Renode.Core.Structure.Registers;
using Antmicro.Renode.Logging;
using Antmicro.Renode.Time;
using Antmicro.Renode.Utilities;
using Antmicro.Renode.Peripherals.Timers;
using System;





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






namespace Antmicro.Renode.Peripherals.Miscellaneous
{
    [AllowedTranslations(AllowedTranslation.ByteToDoubleWord | AllowedTranslation.WordToDoubleWord)]
    public sealed class STM32F103_RCC : IDoubleWordPeripheral, IKnownSize,
        IProvidesRegisterCollection<DoubleWordRegisterCollection>
    {
        public STM32F103_RCC(IMachine machine , Antmicro.Renode.Peripherals.Timers.STM32F103_RTC rtcPeripheral)
        {

            var registersMap = new Dictionary<long, DoubleWordRegister>
            {
                {
                    (long)Registers.ClockControl, new DoubleWordRegister(this, 0x483)
                        .WithFlag(0, out var hsion, name: "HSION")
                        .WithFlag(1, FieldMode.Read, valueProviderCallback: _ => hsion.Value, name: "HSIRDY")
                        .WithReservedBits(2, 1)
                        .WithValueField(3, 5, name: "HSITRIM")
                        .WithTag("HSICAL", 8, 8)
                        .WithFlag(16, out var hseon, name: "HSEON")
                        .WithFlag(17, FieldMode.Read, valueProviderCallback: _ => hseon.Value, name: "HSERDY")
                        .WithTag("HSEBYP", 18, 1)
                        .WithTag("CSSON", 19, 1)
                        .WithReservedBits(20, 4)
                        .WithFlag(24, out var pllon, name: "PLLON")
                        .WithFlag(25, FieldMode.Read, valueProviderCallback: _ => pllon.Value, name: "PLLRDY")
                        .WithReservedBits(26, 6)
                },
                {
                    (long)Registers.ClockConfiguration, new DoubleWordRegister(this)
                        .WithValueField(0, 2, out var systemClockSwitch, name: "SW")
                        .WithValueField(2, 2, FieldMode.Read, name: "SWS",
                            valueProviderCallback: _ => systemClockSwitch.Value)
                        .WithValueField(4, 4, name: "HPRE")
                        .WithValueField(8, 3, name: "PPRE1")
                        .WithValueField(11, 3, name: "PPRE2")
                        .WithValueField(14, 2, name: "ADCPRE")
                        .WithValueField(16, 1, name: "PLLSRC")
                        .WithValueField(17, 1, name: "PLLXTPRE")
                        .WithValueField(18, 4, name: "PLLMUL")
                        .WithValueField(22, 1, name: "USBPRE")
                        .WithReservedBits(23, 1)
                        .WithValueField(24, 3, name: "MCO")
                        .WithReservedBits(27, 5)
                }, //Clock interrupt was not implemented in F4. assuming its safe to do same here
                {
                    (long)Registers.APB2PeripheralReset, new DoubleWordRegister(this)
                        .WithValueField(0, 1, name: "AFIORST")
                        .WithReservedBits(1, 1)
                        .WithValueField(2, 1, name: "IOPARST")
                        .WithValueField(3, 1, name: "IOPBRST")
                        .WithValueField(4, 1, name: "IOPCRST")
                        .WithValueField(5, 1, name: "IOPDRST")
                        .WithValueField(6, 1, name: "IOPERST")
                        .WithValueField(7, 1, name: "IOPFRST")
                        .WithValueField(8, 1, name: "IOPGRST")
                        .WithValueField(9, 1, name: "ADC1RST")
                        .WithValueField(10, 1, name: "ADC2RST")
                        .WithValueField(11, 1, name: "TIM1RST")
                        .WithValueField(12, 1, name: "SPI1RST")
                        .WithValueField(13, 1, name: "TIM8RST")
                        .WithValueField(14, 1, name: "USART1RST")
                        .WithValueField(15, 1, name: "ADC3RST")
                        .WithReservedBits(16,3)
                        .WithValueField(19, 1, name: "TIM9RST")
                        .WithValueField(20, 1, name: "TIM10RST")
                        .WithValueField(21, 1, name: "TIM811ST")
                        .WithReservedBits(22,10)
                },
                {
                    (long)Registers.APB1PeripheralReset, new DoubleWordRegister(this)
                        .WithValueField(0, 9, name: "TIMxRST")
                        .WithReservedBits(9, 2)
                        .WithValueField(11, 1, name: "WWDGRST")
                        .WithReservedBits(12, 2)
                        .WithValueField(14, 2, name: "SPIxRST")
                        .WithReservedBits(16, 1)
                        .WithValueField(17, 2, name: "USARTxRST")
                        .WithValueField(19, 2, name: "UARTxRST")
                        .WithValueField(21, 2, name: "I2CxRST")
                        .WithValueField(23, 1, name: "USBRST")
                        .WithReservedBits(24, 1)
                        .WithValueField(25, 1, name: "CANRST")
                        .WithReservedBits(26, 1)
                        .WithValueField(27, 1, name: "BKPRST")
                        .WithValueField(28, 1, name: "PWRRST")
                        .WithValueField(29, 1, name: "DACRST")
                        .WithReservedBits(30, 2)
                    
                },
                {
                    (long)Registers.APB2PeripheralClockEnable, new DoubleWordRegister(this)
                        .WithValueField(0, 1, name: "AFIOEN")
                        .WithReservedBits(1, 1)
                        .WithValueField(2,7, name: "IOPAxEN")
                        .WithValueField(9, 2, name: "ADCxEN")
                        .WithValueField(11, 1, name: "TIM1EN")
                        .WithValueField(12, 1, name: "SPI1EN")
                        .WithValueField(13, 1, name: "TIM8EN")
                        .WithValueField(14, 1, name: "USART1EN")
                        .WithValueField(15, 1, name: "ADC3EN")
                        .WithReservedBits(16, 3)
                        .WithValueField(19, 1, name: "TIM9EN")
                        .WithValueField(20, 1, name: "TIM10EN")
                        .WithValueField(21, 1, name: "TIM11EN")
                        .WithReservedBits(22, 10)
                },
                {
                    (long)Registers.APB1PeripheralClockEnable, new DoubleWordRegister(this)
                        .WithValueField(0, 9, name: "TIMxEN")
                        .WithReservedBits(9, 2)
                        .WithFlag(11, name: "WWDGEN")
                        .WithReservedBits(12, 2)
                        .WithValueField(14, 2, name: "SPIxEN")
                        .WithReservedBits(16, 1)
                        .WithValueField(17, 2, name: "USARTxEN")
                        .WithValueField(19, 2, name: "UARTxEN")
                        .WithValueField(21, 2, name: "I2CxEN")
                        .WithValueField(23, 1, name: "USBEN")
                        .WithReservedBits(24, 1)
                        .WithValueField(25, 1, name: "CANxEN")
                        .WithReservedBits(26, 1)
                        .WithFlag(27, name: "BKPEN")
                        .WithFlag(28, name: "PWREN")
                        .WithFlag(29, name: "DACEN")
                        .WithReservedBits(30, 2)
                },
                {
                    (long)Registers.BackupDomainControl, new DoubleWordRegister(this)
                        .WithFlag(0, out var lseon, name: "LSEON")
                        .WithFlag(1, FieldMode.Read, valueProviderCallback: _ => lseon.Value, name: "LSERDY")
                        .WithValueField(2, 1, name: "LSEBYP")
                        .WithReservedBits(3, 5)
                        .WithValueField(8, 2, name: "RTCSEL")
                        .WithReservedBits(10, 5)
                        .WithFlag(15, name: "RTCEN",
                            writeCallback: (_, value) =>
                            {
                                if (value)
                                {
                                    machine.SystemBus.EnablePeripheral(rtcPeripheral);
                                }
                                else
                                {
                                    machine.SystemBus.DisablePeripheral(rtcPeripheral);
                                }
                            })
                        .WithValueField(16, 1, name: "BDRST")
                        .WithReservedBits(17, 15)
                },
                {
                    (long)Registers.ClockControlAndStatus, new DoubleWordRegister(this, 0x0C000000)
                        .WithFlag(0, out var lsion, name: "LSION")
                        .WithFlag(1, FieldMode.Read, valueProviderCallback: _ => lsion.Value, name: "LSIRDY")
                        .WithReservedBits(2, 21)
                        .WithTag("RMVF", 24, 1)
                        .WithReservedBits(25, 1)
                        .WithTag("PINRSTF", 26, 1)
                        .WithTag("PORRSTF", 27, 1)
                        .WithTag("SFTRSTF", 28, 1)
                        .WithTag("IWDGRSTF", 29, 1)
                        .WithTag("WWDGRSTF", 30, 1)
                        .WithTag("LPWRRSTF", 31, 1)
                },
                {
                    (long)Registers.AHBPeripheralClockEnable, new DoubleWordRegister(this, 0x14)
                        .WithValueField(0, 2, name: "DMAxEN")
                        .WithValueField(2, 1, name: "SRAMEN")
                        .WithReservedBits(3, 1)
                        .WithValueField(4, 1, name: "FLITFEN")
                        .WithReservedBits(5, 1)
                        .WithValueField(6, 1, name: "CRCEN")
                        .WithReservedBits(7, 1)
                        .WithValueField(8, 1, name: "FSMCEN")
                        .WithReservedBits(9,1)
                        .WithValueField(10, 1, name: "SDIOEN")
                        .WithReservedBits(11, 21)
                    
                },
              
            };
            RegistersCollection = new DoubleWordRegisterCollection(this, registersMap);

        }

        public void Reset()
        {
            RegistersCollection.Reset();
        }
        public DoubleWordRegisterCollection RegistersCollection { get; }
        
        public void WriteDoubleWord(long offset, uint value)
        {
           // this.NoisyLog(string.Format("WRITING TO RCC reg FIle MAPPER + value: {0:X}", value));
            RegistersCollection.Write(offset, value);
        }
        
        public uint ReadDoubleWord(long offset)
        {
            return RegistersCollection.Read(offset);
        }

        public long Size => 0x400;

        private enum Registers
        {
            ClockControl = 0x0, // RCC_CR
            ClockConfiguration = 0x4, // RCC_CFGR
            ClockInterrupt = 0x8, // RCC_CIR
            APB2PeripheralReset = 0x0C, // RCC_APB2RSTR
            APB1PeripheralReset = 0x10, // RCC_APB1RSTR
            AHBPeripheralClockEnable = 0x14, // RCC_AHBENR,
            APB2PeripheralClockEnable = 0x18, // RCC_APB2ENR
            APB1PeripheralClockEnable = 0x1C, // RCC_APB1ENR
            BackupDomainControl = 0x20, // RCC_BDCR
            ClockControlAndStatus = 0x24, // RCC_CSR
          
            
        }
    }

    public class RCC_PLL_MAPPER :IDoubleWordPeripheral, IKnownSize
    {
        private STM32F103_RCC peripheral;
        public RCC_PLL_MAPPER(STM32F103_RCC rccPeripheral)
        {
            peripheral = rccPeripheral;
        }
        public long Size => 0x400;
        public void Reset()
        {
            
        }

        public uint ReadDoubleWord(long offset)
        {
            this.NoisyLog("READING FROM RCC MAPPER");
            return peripheral.ReadDoubleWord(0x0);
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            if (value == 0)
            {
                uint new_value = peripheral.ReadDoubleWord(0x0);
                new_value &= ~(1u << 24);
                peripheral.WriteDoubleWord(0x0, new_value);
            }
            else if (value == 1)
            {
                uint new_value = peripheral.ReadDoubleWord(0x0);
                new_value  |= (1 << 24); 
                peripheral.WriteDoubleWord(0x0, new_value);
            }
           
            
        }
    }
    public class FLASH_ACR :IDoubleWordPeripheral, IKnownSize
    {
        private uint regValue = 0x2;
        public FLASH_ACR()
        {
          
        }
        public long Size => 0x4;
        public void Reset()
        {
            
        }

        public uint ReadDoubleWord(long offset)
        {
            return regValue;
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            regValue = value;
        }
           
            
        }
    


} 
    
