/**
 * @file        main.h
 * @path        ~/CODES_local/ccs7_workspace3/F28335_ram_led/include/main.h
 *
 * @date        
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license     GNU GPL v3
 *
 *
   @verbatim

    ------------------------------------------------------------------------

    Copyright (C) 2017  Manuel Del Basso

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    ------------------------------------------------------------------------

   @endverbatim
 *
 *
 */
#ifndef MAIN_H_
#define MAIN_H_


/**
 * Hex Encoder The position of the 4-bit hex encoder sets the values of GPIOs 12-15
 */
struct ENC_GPIOS {
    uint16_t mask;
    uint16_t GPIO15:1;            //
    uint16_t GPIO14:1;            //
    uint16_t GPIO13:1;            // ENC1
    uint16_t GPIO12:1;            // ENC0
};

extern volatile struct ENC_GPIOS MD_ENC;



#endif /* MAIN_H_ */
