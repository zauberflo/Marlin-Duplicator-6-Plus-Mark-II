/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _MATH_AVR_H_
#define _MATH_AVR_H_

#define a(CODE) " " CODE "\n\t"

/**
 * Optimized math functions for AVR
 */

// intRes = longIn1 * longIn2 >> 24
// uses:
// r26 to store 0
// r27 to store bits 16-23 of the 48bit result. The top bit is used to round the two byte result.
// note that the lower two bytes and the upper byte of the 48bit result are not calculated.
// this can cause the result to be out by one as the lower bytes may cause carries into the upper ones.
// B0 A0 are bits 24-39 and are the returned value
// C1 B1 A1 is longIn1
// D2 C2 B2 A2 is longIn2
//
#define MultiU24X32toH16(intRes, longIn1, longIn2) \
  asm volatile ( \
                 A("clr r26")      \
                 A("mul %A1, %B2") \
                 A("mov r27, r1")  \
                 A("mul %B1, %C2") \
                 A("movw %A0, r0") \
                 A("mul %C1, %C2") \
                 A("add %B0, r0")  \
                 A("mul %C1, %B2") \
                 A("add %A0, r0")  \
                 A("adc %B0, r1")  \
                 A("mul %A1, %C2") \
                 A("add r27, r0")  \
                 A("adc %A0, r1")  \
                 A("adc %B0, r26") \
                 A("mul %B1, %B2") \
                 A("add r27, r0")  \
                 A("adc %A0, r1")  \
                 A("adc %B0, r26") \
                 A("mul %C1, %A2") \
                 A("add r27, r0")  \
                 A("adc %A0, r1")  \
                 A("adc %B0, r26") \
                 A("mul %B1, %A2") \
                 A("add r27, r1")  \
                 A("adc %A0, r26") \
                 A("adc %B0, r26") \
                 A("lsr r27")      \
                 A("adc %A0, r26") \
                 A("adc %B0, r26") \
                 A("mul %D2, %A1") \
                 A("add %A0, r0")  \
                 A("adc %B0, r1")  \
                 A("mul %D2, %B1") \
                 A("add %B0, r0")  \
                 A("clr r1")       \
                 : \
                 "=&r" (intRes) \
                 : \
                 "d" (longIn1), \
                 "d" (longIn2) \
                 : \
                 "r26" , "r27" \
               )

// intRes = intIn1 * intIn2 >> 16
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 24 bit result
#define MultiU16X8toH16(intRes, charIn1, intIn2) \
  asm volatile ( \
                 A("clr r26")      \
                 A("mul %A1, %B2") \
                 A("movw %A0, r0") \
                 A("mul %A1, %A2") \
                 A("add %A0, r1")  \
                 A("adc %B0, r26") \
                 A("lsr r0")       \
                 A("adc %A0, r26") \
                 A("adc %B0, r26") \
                 A("clr r1")       \
                 : \
                 "=&r" (intRes) \
                 : \
                 "d" (charIn1), \
                 "d" (intIn2) \
                 : \
                 "r26" \
               )


#endif // _MATH_AVR_H_
