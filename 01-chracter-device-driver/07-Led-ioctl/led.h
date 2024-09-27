#ifndef __LED_MODULE_H   // Include guard to prevent multiple inclusion of this header file
#define __LED_MODULE_H

// Base address for GPIO0 peripheral registers
#define GPIO0_ADDR_BASE             0x44E07000

// End address for the GPIO0 memory range (this is not directly used in your current code)
#define GPIO0_ADDR_END              0x44E07FFF

// Size of the GPIO0 address range, calculated as the difference between the base and end addresses
#define GPIO0_ADDR_SIZE             (GPIO0_ADDR_END - GPIO0_ADDR_BASE)  // Fix: size should be the difference

// Offset to the Output Enable register (OE) for GPIO, used to set pins as input or output
#define GPIO_OE_OFFSET              0x134

// Offset to the Clear Data Out register, used to clear GPIO pin output (turn the pin to 0)
#define GPIO_CLEARDATAOUT_OFFSET    0x190

// Offset to the Set Data Out register, used to set GPIO pin output (turn the pin to 1)
#define GPIO_SETDATAOUT_OFFSET      0x194

// Macro to represent the specific GPIO pin 28 (connected to P9_12 on BeagleBone Black)
#define GPIO0_30                    (1 << 30)       /* P9_11 */

// End of include guard
#endif
