#ifndef __BBCP_C32_H__
#define __BBCP_C32_H__
/******************************************************************************/
/*                                                                            */
/*                            b b c p _ C 3 2 . h                             */
/*                                                                            */
/*(c) 2010-14 by the Board of Trustees of the Leland Stanford, Jr., University*//*      All Rights Reserved. See bbcp_Version.C for complete License Terms    *//*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
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

typedef unsigned int uint32;
  
class bbcp_C32 : public bbcp_ChkSum
{
public:

void  Init() {C32Result = CRC32_XINIT; TotLen = 0; has_final = false; }

void  Update(const char *Buff, int BLen);

int   csSize() {return sizeof(C32Result);}


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
                uint crc = C32Result;
                if (tLcs)
                   {while(tLcs) {buff[i++] = tLcs & 0xff ; tLcs >>= 8;}
                    crc = do_crc(crc, buff, i);
                   }
                crc = crc ^ CRC32_XOROT;
#ifndef BBCP_BIG_ENDIAN
                crc = htonl(crc);
#endif
                if (Text) *Text = x2a((char *)&crc);
                return crc;
               }

public:
const char *Type() {return "c32";}

            bbcp_C32() {Init();}
virtual    ~bbcp_C32() {}

private:
static const uint CRC32_XINIT = 0;
static const uint CRC32_XOROT = 0xffffffff;
static       uint crctable[256];
             uint C32Result;
       long  long TotLen;
         uint32_t tmpcrc;
             bool has_final;

             uint do_crc(uint crc, const char *p, int reclen);

};
#endif
