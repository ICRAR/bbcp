#ifndef __BBCP_C32_ZIP_H__
#define __BBCP_C32_ZIP_H__
/******************************************************************************/
/*                                                                            */
/*                            b b c p _ C 3 2 _ Z i p. h                      */
/*                                                                            */
/*   ICRAR - International Centre for Radio Astronomy Research                */
/*   (c) UWA - The University of Western Australia, 2016                      */
/*   Copyright by UWA (in the framework of the ICRAR)                         */
/*   All rights reserved                                                      */
/*                                                                            */
/*   Produced by Rodrigo Tobar based on bbcp_C32.h                            */
/*                                                                            */
/* bbcp is free software: you can redistribute it and/or modify it under      */
/* the terms of the GNU Lesser General Public License as published by the     */
/* Free Software Foundation, either version 3 of the License, or (at your     */
/* option) any later version.                                                 */
/*                                                                            */
/* bbcp is distributed in the hope that it will be useful, but WITHOUT        */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public       */
/* License for more details.                                                  */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public License   */
/* along with bbcp in a file called COPYING.LESSER (LGPL license) and file    */
/* COPYING (GPL license).  If not, see <http://www.gnu.org/licenses/>.        */
/*                                                                            */
/* The copyright holder's institutional names and contributor's names may not */
/* be used to endorse or promote products derived from this software without  */
/* specific prior written permission of the institution or contributor.       */
/******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <netinet/in.h>

#include "bbcp_ChkSum.h"
#include "bbcp_Endian.h"

/**
 * A PKZip-compatible CRC32 implementation.
 *
 * The CRC32 implementation available in the bbcp_C32 class is based in the CRC
 * error checking described in the ISO/IEC 8802-3:1996 standard (Ethernet), and
 * implemented for example by the cksum utility. This class follows instead the
 * CRC32 algorithm used in the ISO 3309 standard, which is in turn used by the
 * ZIP format as specified in RFC 1952, and by many other CRC32 implementors.
 */
class bbcp_C32_Zip : public bbcp_ChkSum
{
public:

void  Init() { crc = CRC32_XINIT; has_final = false; }

void  Update(const char *Buff, int BLen);

int   csSize() {return sizeof(crc);}

char *csCurr(char **Text=0) {
    if( has_final ) {
        return (char *)&crc;
    }
    tmpcrc = finish(Text);
    return (char *)&tmpcrc;
}

char *Final(char **Text=0) {
    has_final = true;
    crc = finish(Text);
    return (char *)&crc;
}

private:
uint32_t finish(char **Text=0)
               {
                uint32_t finished = crc ^ CRC32_XOROT;
#ifndef BBCP_BIG_ENDIAN
                finished = htonl(finished);
#endif
                if (Text) *Text = x2a((char *)&finished);
                return finished;
               }

public:
const char *Type() {return "c32zip";}

            bbcp_C32_Zip() {Init();}
virtual    ~bbcp_C32_Zip() {}

private:
static const uint32_t CRC32_XINIT = 0xffffffff;
static const uint32_t CRC32_XOROT = 0xffffffff;
             uint32_t crc;
             uint32_t tmpcrc;
                 bool has_final;
};
#endif