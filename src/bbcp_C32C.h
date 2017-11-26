#ifndef __BBCP_C32C_H__
#define __BBCP_C32C_H__
/******************************************************************************/
/*                                                                            */
/*                            b b c p _ C 3 2 C . h                           */
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
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>

#include "bbcp_ChkSum.h"
#include "bbcp_Endian.h"


/* Try to guess if we have an Intel CPU */
#if defined __X86__ || defined __i386__ || defined i386 || defined _M_IX86 || defined __386__ || defined __x86_64__ || defined _M_X64
#define HAS_INTEL_CPU 1
#else
#define HAS_INTEL_CPU 0
#endif


typedef unsigned int uint32;
  
class bbcp_C32C : public bbcp_ChkSum
{
public:

void  Init();

void  Update(const char *Buff, int BLen);

int csSize() {
	return sizeof(C32Result);
}


char *csCurr(char **Text=0) {
    if( has_final ) {
        return (char *)&C32Result;
    }
    tmpcrc = finish(Text);
    return (char *)&tmpcrc;

}

char *Final(char **Text=0) {
    has_final = true;
    C32Result = finish(Text);
    return (char *)&C32Result;
}

private:
uint finish(char **Text)
               {char buff[sizeof(long long)];
                long long tLcs = TotLen;
                int i = 0;
                has_final = true;
                uint crc = ~C32Result;
#ifndef BBCP_BIG_ENDIAN
                crc = htonl(crc);
#endif
                if (Text) *Text = x2a((char *)&crc);
                return crc;
               }

public:
const char *Type() {return "c32c";}

            bbcp_C32C() {Init();}
virtual    ~bbcp_C32C() {}

private:
static const uint CRC32_XINIT = 0xffffffff;
static       uint crctable[256];
             uint C32Result;
       long  long TotLen;
         uint32_t tmpcrc;
             bool has_final;
#if HAS_INTEL_CPU
             bool has_hw_crc32c;
#endif

             uint do_crc(uint crc, const char *p, int reclen);

};
#endif
