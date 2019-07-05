#ifndef __DDKLBLINC_H
#define __DDKLBLINC_H

//
// Master Header For Windows NT DDK Use Of LBL Network Include Files
//

/////////////////////////////////////////////////////////////////////////////
//// Windows System Definitions

#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#define LITTLE_ENDIAN 1234
#endif

#define BYTE_ORDER LITTLE_ENDIAN 

/*
 * Basic system type definitions, taken from the BSD file sys/types.h.
 */
typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

//
// Specify Structure Packing
//
#pragma pack(push,1)

#include "in_systm.h"      // n_long


/////////////////////////////////////////////////////////////////////////////
//// Data Link

#include "ethertype.h"


/////////////////////////////////////////////////////////////////////////////
//// Internet Protocol (IP)

/*
 * Internet address (old style... should be updated)
 */
#ifndef _WINSOCKAPI_

struct in_addr {
        union {
                struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { u_short s_w1,s_w2; } S_un_w;
                u_long S_addr;
        } S_un;
#define s_addr  S_un.S_addr
                                /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2
                                /* host on imp */
#define s_net   S_un.S_un_b.s_b1
                                /* network */
#define s_imp   S_un.S_un_w.s_w2
                                /* imp */
#define s_impno S_un.S_un_b.s_b4
                                /* imp # */
#define s_lh    S_un.S_un_b.s_b3
                                /* logical host */
};

#endif

#include "in.h"
#include "ip.h"
#include "udp.h"
#include "tcp.h"
#include "igmp.h"
#include "ip_icmp.h"
#include "ip_mrout.h"

//
// Restore Default Structure Packing
//
#pragma pack(pop)

#endif // __DDKLBLINC_H

