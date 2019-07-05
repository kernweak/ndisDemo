
#include "precomp.h"
#include "DDKLBLInc.h"
#include "cons_def.h"
#include "fw_rule.h"
#include "fw_log.h"
#include "PCAENET.H"
#include "fw_interface.h"

#define ONE_DAY_SECONDS			86400
#define SECONDS_OF_1970_1601	3054539008	// 1970-01-01 - 1601-01-01 's seconds
#define WEEK_OF_1970_01_01		5			// 1970-01-01 is Saturday
#define HOUR_8_SECONDS			28800

extern struct Fw_Rules g_Rules;     //全局规则变量
extern struct Statistics  g_stat;   //统计包的大小
extern struct ip 		*g_pIPHeader;
extern struct tcphdr	*g_pTCPHeader;
extern struct udphdr	*g_pUDPHeader;
extern struct icmp		*g_pICMPHeader;

extern struct Log g_Log, *g_pLog;   //临时加的。。待修改


USHORT  	alarm_num=0,log_num=0,acall_num=0,lcall_num=0;

#pragma warning(disable:4273)
extern LONG		KeReadStateEvent(IN PRKEVENT Event);
extern PVOID	gpEventObject;			// 与应用通信的事件对象
extern PVOID	SystemVirtualAddress;	// 共享内存地址


//-------------------------------CopyPacket2Buffer-------------------------------
//
// Purpose
// 从截获的包描述符里面取出数据到缓冲区
// add by xiaoqiang
void CopyPacket2Buffer(IN PNDIS_PACKET pPacket,IN OUT PUCHAR pBuff,IN OUT PUINT pLength) 
{ 
	PNDIS_BUFFER BuffDT;  //定义一个buffer指示符
	PUCHAR BuffVA;  //指向一个PNDIS_PACKET结构里面的Buffer的虚拟地址
	UINT BuffLen; //Buffer的长度

	*pLength=0;  //数据包内容的总长度,开始先置零 
	BuffLen=0;  

	NdisQueryPacket(pPacket,NULL,NULL,&BuffDT,NULL); //查询Packet的信息，这里查的是Packet的Buffer指示符链表的第一个Buffer
	while(BuffDT!=(PNDIS_BUFFER)NULL) 
	{ 
		NdisQueryBufferSafe(BuffDT,&BuffVA,&BuffLen,32);  //得到BuffDT指向的那个Buffer的虚拟地BuffVA和长度BuffLen 
		NdisMoveMemory(pBuff,BuffVA,BuffLen);   //将BuffVA其中的内容，移动到pBuff指向的那块区域
		pBuff=pBuff+BuffLen;   //pBuff指针后移，前BuffLen个字节已经填入数据
		*pLength+=BuffLen;    //计算pBuff中填入数据的总长度pLength
		NdisGetNextBuffer(BuffDT,&BuffDT);  //获得Buffer指示符链表中的下一个Buffer指示符
	} 
	return; 
} 

/*----------The same as CopyPacket2Buffer-------------
//add by xiaoqiang  Mar 15 2006
void CopyPacketToBuffer(IN PNDIS_PACKET pPacket,IN OUT PUCHAR pBuff,IN OUT PUINT pLength)
{
	int        PacketSize;
    PUCHAR        BuffVA;
    UINT         BuffLen;
    MDL    *     pNext;
    *pLength = 0;
	BuffLen = 0;
   
    //把数据包内容从Packet拷贝到pPacketContent
	
    NdisQueryPacket( pPacket,NULL,NULL,NULL,&PacketSize);
    NdisQueryBufferSafe(pPacket->Private.Head, &BuffVA, &BuffLen, 32 );
    NdisMoveMemory(pBuff, BuffVA, BuffLen);

    *pLength = BuffLen;
    pNext = pPacket->Private.Head;
    
    for(;;)
    {
        if(pNext == pPacket->Private.Tail)
            break;
        pNext = pNext->Next;   //指针后移
        if(pNext == NULL) 
            break;

        NdisQueryBufferSafe(pNext,&BuffVA,&BuffLen,32);
        NdisMoveMemory(pBuff + *pLength,BuffVA,BuffLen);
        *pLength+=BuffLen;
	}
	return; 
}
--------------------------------------------------------------add end */

/*=========================================================================
// 检查数据包函数,后来采用了UTIL_MatchRule函数。。
UINT Check_Packet(char *pPacket)
/*--
	return value:
	1 -> 匹配
	0 -> 不匹配
--*/
/*===
{
	P_IP_PACKET p_ip;
	__try
	{
		__try
		{
			p_ip = (P_IP_PACKET)pPacket;
			// 检测是否为 TCP 数据包
			if(p_ip->H_frame_type == 8 && p_ip->L_frame_type == 0 && p_ip->proto == 6)
			{
				DBGPRINT(("拦截到TCP数据包"));   //add by xiaoqiang
				if(!KeReadStateEvent(gpEventObject))
				{
					// 复制数据到共享内存
					memset(SystemVirtualAddress, 0, 100);
					memcpy(SystemVirtualAddress, (char *)p_ip, sizeof(IP_PACKET));

					KeSetEvent(gpEventObject, 0, FALSE);
				}
				else{}
				return 1;
			}
		}
		except (EXCEPTION_EXECUTE_HANDLER){DbgPrint("发生异常!  --In Check_Packet\n"); }
	}
	finally{}

	return 0;
}
=====================================================   */

void*  UTIL_AllocateMemory(unsigned int size)
{
	PVOID ptr=NULL;
	ptr=ExAllocatePool(NonPagedPool , size);
	
	return ptr;
}

void  UTIL_FreeMemory(void* p)
{
	if (p)
		ExFreePool(p);
}


/////////////////////////////////////////////////////////////////////////////
//// UTIL_htonl
//
// Purpose
// Converts a ULONG from host to network byte order.
//
// Parameters
//
// Return Value
//
// Remarks
// ATTENTION!!! Must revise for non-Intel (Alpha) hosts.
//
// Why isn't this function named simply "htonl"? Possible confilct with
// system implementations available on some hosts, but possibly declared
// differently...
//

ULONG UTIL_htonl( ULONG hostlong )
{
	PUCHAR pBuffer;
	ULONG	nResult;
	UCHAR	c, *pResult;

	pBuffer = (PUCHAR )&hostlong;

	if( !pBuffer )
	{
		return( 0L );
	}

	pResult = (UCHAR * )&nResult;

	c = ((UCHAR * )pBuffer)[ 0 ];
	((UCHAR * )pResult)[ 0 ] = ((UCHAR * )pBuffer)[ 3 ];
	((UCHAR * )pResult)[ 3 ] = c;

	c = ((UCHAR * )pBuffer)[ 1 ];
	((UCHAR * )pResult)[ 1 ] = ((UCHAR * )pBuffer)[ 2 ];
	((UCHAR * )pResult)[ 2 ] = c;

	return( nResult );
}


/////////////////////////////////////////////////////////////////////////////
//// UTIL_htons
//
// Purpose
// Converts a USHORT from host to network byte order.
//
// Parameters
//
// Return Value
//
// Remarks
// ATTENTION!!! Must revise for non-Intel (Alpha) hosts.
//
// Why isn't this function named simply "htons"? Possible confilct with
// system implementations available on some hosts, but possibly declared
// differently...
//

USHORT UTIL_htons( USHORT hostshort )
{
	PUCHAR	pBuffer;
	USHORT	nResult;

	nResult = 0;
	pBuffer = (PUCHAR )&hostshort;

	nResult = ( (pBuffer[ 0 ] << 8) & 0xFF00 )
		| ( pBuffer[ 1 ] & 0x00FF );

	return( nResult );
}

/////////////////////////////////////////////////////////////////////////////
//// UTIL_ntohl
//
// Purpose
// Converts a ULONG from network to host byte order.
//
// Parameters
//
// Return Value
//
// Remarks
//
// Why isn't this function named simply "ntohl"? Possible confilct with
// system implementations available on some hosts, but possibly declared
// differently...
//

ULONG UTIL_ntohl( ULONG netlong )
{
	return( UTIL_htonl( netlong ) );
}

/////////////////////////////////////////////////////////////////////////////
//// UTIL_ntohs
//
// Purpose
// Converts a USHORT from network to host byte order.
//
// Parameters
//
// Return Value
//
// Remarks
//
// Why isn't this function named simply "ntohs"? Possible confilct with
// system implementations available on some hosts, but possibly declared
// differently...
//

USHORT UTIL_ntohs( USHORT netshort )
{
	return( UTIL_htons( netshort ) );
}


//
// 得到当前日期/时间、星期和时间
//
// 参数：
//		Week:	返回星期，1、2 ... 7 分别表示 星期日、星期一 ... 星期6
//		pTime:	返回当前时间在当天的秒数
//
// 返回值：
//		ULONG数值表示的日期/时间，保存的是从1970/1/1到现在的秒数，与CTime表示形式相同
//
//
//
ULONG GetCurrentTime(void)
{
	LARGE_INTEGER SystemTime;
	ULONG GmtDateTime, DateTime, Date, Time;
	KeQuerySystemTime(&SystemTime);
	GmtDateTime = (ULONG)(SystemTime.QuadPart / 10000000 - SECONDS_OF_1970_1601);
	ExSystemTimeToLocalTime(&SystemTime, &SystemTime);
	return GmtDateTime;
}

/////////////////////////////////////////////////////////////////
//  
//函数UTIL_CountPacket
//
//参数
//方向、动作
//
//返回值
//无
//
//
//备注
//统计在in、out方向上允许或禁止的包。in（0）、out（1）、allow（0）
//deny（1）


void UTIL_CountPacket (USHORT ptype,char direct,char action,int packsize)
{
	//
	//if 方向为in {
	//	if 动作为允许  统计结构累计
	//	if 动作为禁止  统计结构累计
	//}
	//if 方向为out {
	//	if 动作为允许  统计结构累计
	//	if 动作为禁止  统计结构累计
	//}
	//
	DbgPrint("==>UTIL_CountPacket()");

/*
	DBGPRINT(("tcpInBytes：%d",g_stat.ip.tcp.allow_in_bytes));
	DBGPRINT(("tcpInPacketNUM：%d",g_stat.ip.tcp.allow_in_pkts));
	DBGPRINT(("udpInBytes：%d",g_stat.ip.udp.allow_in_bytes));
	DBGPRINT(("udpInPacketNUM：%d",g_stat.ip.udp.allow_in_pkts));
	DBGPRINT(("tcpOutBytes：%d",g_stat.ip.tcp.allow_out_bytes));
	DBGPRINT(("tcpOutPacketNUM：%d",g_stat.ip.tcp.allow_out_pkts));
	DBGPRINT(("udpOutBytes：%d",g_stat.ip.udp.allow_out_bytes));
	DBGPRINT(("udpOutPacketNUM：%d",g_stat.ip.udp.allow_out_pkts));
*/

	if (direct==DT_IN)
	{
		if (action==ALLOW)
		{
			switch (ptype)
			{
			case PT_TCP:
				g_stat.ip.tcp.allow_in_bytes+=packsize;
				g_stat.ip.tcp.allow_in_pkts++;
				break;
			case PT_UDP:
				g_stat.ip.udp.allow_in_bytes+=packsize;
				g_stat.ip.udp.allow_in_pkts++;
				break;
			case PT_ICMP:
				g_stat.ip.icmp.allow_in_bytes+=packsize;
				g_stat.ip.icmp.allow_in_pkts++;
				break;
			default:
				g_stat.notip.other_stat.allow_in_bytes+=packsize;
				g_stat.notip.other_stat.allow_in_pkts++;
			}
		}
		if (action==DENY)
		{
			switch (ptype)
			{
			case PT_TCP:
				g_stat.ip.tcp.deny_in_bytes+=packsize;
				g_stat.ip.tcp.deny_in_pkts++;
				break;
			case PT_UDP:
				g_stat.ip.udp.deny_in_bytes+=packsize;
				g_stat.ip.udp.deny_in_pkts++;
				break;
			case PT_ICMP:
				g_stat.ip.icmp.deny_in_bytes+=packsize;
				g_stat.ip.icmp.deny_in_pkts++;
				break;
			default:
				g_stat.notip.other_stat.deny_in_bytes+=packsize;
				g_stat.notip.other_stat.deny_in_pkts++;
			}
		}
	}
	if (direct==DT_OUT)
	{
		if (action==ALLOW)
		{
			switch (ptype)
			{
			case PT_TCP:
				g_stat.ip.tcp.allow_out_bytes+=packsize;
				g_stat.ip.tcp.allow_out_pkts++;
				break;
			case PT_UDP:
				g_stat.ip.udp.allow_out_bytes+=packsize;
				g_stat.ip.udp.allow_out_pkts++;
				break;
			case PT_ICMP:
				g_stat.ip.icmp.allow_out_bytes+=packsize;
				g_stat.ip.icmp.allow_out_pkts++;
				break;
			default:
				g_stat.notip.other_stat.allow_out_bytes+=packsize;
				g_stat.notip.other_stat.allow_out_pkts++;
			}
		}
		if (action==DENY)
		{
			switch (ptype)
			{
			case PT_TCP:
				g_stat.ip.tcp.deny_out_bytes+=packsize;
				g_stat.ip.tcp.deny_out_pkts++;
				break;
			case PT_UDP:
				g_stat.ip.udp.deny_out_bytes+=packsize;
				g_stat.ip.udp.deny_out_pkts++;
				break;
			case PT_ICMP:
				g_stat.ip.icmp.deny_out_bytes+=packsize;
				g_stat.ip.icmp.deny_out_pkts++;
				break;
			default:
				g_stat.notip.other_stat.deny_out_bytes+=packsize;
				g_stat.notip.other_stat.deny_out_pkts++;
			}
		}
	}
	DbgPrint("<==UTIL_CountPacket()");
	
}


/////////////////////////////////////////////////////////////////
//  
//函数UTIL_WriteLog
//
//参数
//USHORT ID,struct ip cur_ip
//
//返回值
//无
//
//
//备注
//在IOCTL中加入SETLOGADDRESS，传入回调函数地址，并得到当前线程号。
//在此函数中调用 _VWIN32_QueueUserApc() 调用应用程序函数。

void UTIL_TcpWriteLog(
	struct Fw_Rule_Node *pTcpRule,
	struct ip *piphdr,
	struct tcphdr *ptcphdr,
	char Direct
	)
{
	int i;

	g_Log.id=pTcpRule->id;
	g_Log.protocol=PT_TCP;
	g_Log.time=GetCurrentTime();
	g_Log.d_ip=UTIL_ntohl(piphdr->ip_dst.S_un.S_addr);
	g_Log.s_ip=UTIL_ntohl(piphdr->ip_src.S_un.S_addr);
	g_Log.s_port=UTIL_ntohs(ptcphdr->th_sport);
	g_Log.d_port=UTIL_ntohs(ptcphdr->th_dport);
	DBGPRINT(("==>UTIL_TcpWriteLog TCP包方向：%d",Direct));
	g_Log.direct=Direct;
	g_Log.action=pTcpRule->action;
	for(i = 0; i < 100; i++)
	{
		g_Log.sMemo[i]=pTcpRule->sMemo[i];
	}

	#ifdef _DEBUG
	DBGPRINT(("g_Log.id：%d",g_Log.id));
	DBGPRINT(("g_Log.protocol：%d",g_Log.protocol));
	DBGPRINT(("g_Log.time：%d",g_Log.time));
	DBGPRINT(("g_Log.d_ip：%d",g_Log.d_ip));
	DBGPRINT(("g_Log.s_ip：%d",g_Log.s_ip));
	DBGPRINT(("g_Log.s_port：%d",g_Log.s_port));
	DBGPRINT(("g_Log.d_port：%d",g_Log.d_port));
	DBGPRINT(("g_Log.direct：%d",g_Log.direct));
	DBGPRINT(("g_Log.action：%d",g_Log.action));
	DBGPRINT(("g_Log.sMemo：%s",g_Log.sMemo));
	#endif

	if(!KeReadStateEvent(gpEventObject))
		{
		// 复制数据到共享内存
			memset(SystemVirtualAddress, 0, sizeof(g_Log));
			memcpy(SystemVirtualAddress, (char *)&g_Log, sizeof(g_Log));

			KeSetEvent(gpEventObject, 0, FALSE);
		}
}






void UTIL_UdpWriteLog(
	struct Fw_Rule_Node *pUdpRule,
	struct ip *piphdr,
	struct udphdr *pudphdr,
	char Direct
)
{
	int i;

	g_Log.id=pUdpRule->id;
	g_Log.protocol=PT_UDP;
	g_Log.time=GetCurrentTime();
	g_Log.d_ip=UTIL_ntohl(piphdr->ip_dst.S_un.S_addr);
	g_Log.s_ip=UTIL_ntohl(piphdr->ip_src.S_un.S_addr);
	g_Log.s_port=UTIL_ntohs(pudphdr->uh_sport);
	g_Log.d_port=UTIL_ntohs(pudphdr->uh_dport);
	g_Log.direct=Direct;
	g_Log.action=pUdpRule->action;
	for(i = 0; i < 100; i++)
	{
		g_Log.sMemo[i]=pUdpRule->sMemo[i];
	}

	if(!KeReadStateEvent(gpEventObject))
		{
		// 复制数据到共享内存
			memset(SystemVirtualAddress, 0, sizeof(g_Log));
			memcpy(SystemVirtualAddress, (char *)g_pLog, sizeof(g_Log));

			KeSetEvent(gpEventObject, 0, FALSE);
		}
}




void UTIL_IcmpWriteLog(
	struct Fw_Rule_Node *pIcmpRule,
	struct ip *piphdr,
	struct icmp *picmphdr,
	char Direct
)
{
	int i;

	g_Log.id=pIcmpRule->id;
	g_Log.protocol=PT_ICMP;
	g_Log.time=GetCurrentTime();
	g_Log.d_ip=UTIL_ntohl(piphdr->ip_dst.S_un.S_addr);
	g_Log.s_ip=UTIL_ntohl(piphdr->ip_src.S_un.S_addr);
	g_Log.s_port=0;
	g_Log.d_port=0;
	g_Log.direct=Direct;
	g_Log.action=pIcmpRule->action;
	for(i = 0; i < 100; i++)
	{
		g_Log.sMemo[i]=pIcmpRule->sMemo[i];
	}

	if(!KeReadStateEvent(gpEventObject))
		{
		// 复制数据到共享内存
			memset(SystemVirtualAddress, 0, sizeof(g_Log));
			memcpy(SystemVirtualAddress, (char *)g_pLog, sizeof(g_Log));

			KeSetEvent(gpEventObject, 0, FALSE);
		}
}


/////////////////////////////////////////////////////////////////
//  
//函数UTIL_SizeupPort
//
//参数
//USHORT cur_port,char p_operator,USHORT port1,USHORT port2
//
//返回值
//TRUE,FALSE
//
//
//备注
//cur_port是否满足规则中对端口的要求

BOOLEAN UTIL_SizeupPort(
			USHORT cur_port,
			char p_operator,
			USHORT port1,
			USHORT port2
			)
{
	if (port1==0 && port2==0)
		return TRUE;

	switch (p_operator)
	{
	case OP_GT:
		return (cur_port>port1);
	case OP_LT:
		return (cur_port<port1);
	case OP_EQ:
		return (cur_port==port1);
	case OP_NE:
		return (cur_port != port1);
	case OP_BW:
		return (cur_port>port1 && cur_port<port2);
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////
// debug function
//
//
void do_nothing()
{
;
;
}

/////////////////////////////////////////////////////////////////
//  
//函数UTIL_AddRuleSet
//
//参数
//RuleSet
//
//返回值
//是，否成功
//
//
//备注
//增加系统设置

BOOLEAN UTIL_AddConfig(struct RuleSet *RuleSets)
{
	g_Rules.global.fw_status=RuleSets->firewall_status;
	g_Rules.global.fw_mode=RuleSets->mode;
	g_Rules.global.log_disable=RuleSets->log_disable;
	g_Rules.tcp.disable=RuleSets->proto.tcp_disable;
	g_Rules.udp.disable=RuleSets->proto.udp_disable;
	g_Rules.icmp.disable=RuleSets->proto.icmp_disable;
	return TRUE;
}


void UTIL_PASSALL()
{
	g_Rules.global.fw_status=FWS_ALLOWALL;
}


void UTIL_DENYALL()
{
	g_Rules.global.fw_status=FWS_DENYALL;
}	


/////////////////////////////////////////////////////////////////
//  
//函数UTIL_AddOneRule
//
//参数
//rules
//
//返回值
//是，否成功
//
//
//备注
//增加所有规则

BOOLEAN UTIL_AddOneRule(struct Rules *prules)
{
	int i;
	struct Fw_Rule_Node		*ptcprule,*pt_in,*pt_out;
	struct Fw_Rule_Node	    *pudprule,*pu_in,*pu_out;
	struct Fw_Rule_Node		*picmprule,*pi_in,*pi_out;
	DbgPrint("==>UTIL_AddOnerule()");

		switch (prules->protocol)
		{
		case PT_TCP:
			if (prules->direct == DT_IN)
			{
				ptcprule=(struct Fw_Rule_Node *)UTIL_AllocateMemory(sizeof(struct Fw_Rule_Node));
				ptcprule->action=prules->action;
				ptcprule->s_port=prules->s_port;
				ptcprule->d_port=prules->d_port;
				ptcprule->direct=prules->direct;
				ptcprule->id=prules->id;
				ptcprule->s_ip=prules->s_ip;
				ptcprule->s_mask=prules->s_mask;
				ptcprule->d_ip=prules->d_ip;
				ptcprule->d_mask=prules->d_mask;
				ptcprule->next=NULL;
				
				
				if (g_Rules.tcp.pin==NULL) //头节点
				{
					g_Rules.tcp.pin=ptcprule;
					pt_in=ptcprule;
				}
				else
				{
					ptcprule->next=g_Rules.tcp.pin;
					g_Rules.tcp.pin=ptcprule;
				}

			}
			if(prules->direct == DT_OUT)
			{
				ptcprule=(struct Fw_Rule_Node *)UTIL_AllocateMemory(sizeof(struct Fw_Rule_Node));
				ptcprule->action=prules->action;
				ptcprule->s_port=prules->s_port;
				ptcprule->d_port=prules->d_port;
				ptcprule->direct=prules->direct;
				ptcprule->id=prules->id;
				ptcprule->s_ip=prules->s_ip;
				ptcprule->s_mask=prules->s_mask;
				ptcprule->d_ip=prules->d_ip;
				ptcprule->d_mask=prules->d_mask;
				ptcprule->next=NULL;

				if (g_Rules.tcp.pout==NULL) //头节点
				{
					g_Rules.tcp.pout=ptcprule;
					pt_out=ptcprule;
				}
				else
				{
					ptcprule->next=g_Rules.tcp.pout;
					g_Rules.tcp.pout=ptcprule;

				}

			
			}
			break;

		case PT_UDP:
			if (prules->direct == DT_IN)
			{
				pudprule=(struct Fw_Rule_Node *)UTIL_AllocateMemory(sizeof(struct Fw_Rule_Node));
				pudprule->action=prules->action;
				pudprule->s_port=prules->s_port;
				pudprule->d_port=prules->d_port;
				pudprule->direct=prules->direct;
				pudprule->id=prules->id;
				pudprule->s_ip=prules->s_ip;
				pudprule->s_mask=prules->s_mask;
				pudprule->d_ip=prules->d_ip;
				pudprule->d_mask=prules->d_mask;
				pudprule->next=NULL;
				
				if (g_Rules.udp.pin==NULL) //头节点
				{
					g_Rules.udp.pin=pudprule;
					pu_in=pudprule;
				}
				else
				{
					pudprule->next=g_Rules.udp.pin;
					g_Rules.udp.pin=pudprule;
				}
			}
			if (prules->direct==DT_OUT)
			{
				pudprule=(struct Fw_Rule_Node *)UTIL_AllocateMemory(sizeof(struct Fw_Rule_Node));
				pudprule->action=prules->action;
				pudprule->s_port=prules->s_port;
				pudprule->d_port=prules->d_port;
				pudprule->direct=prules->direct;
				pudprule->id=prules->id;
				pudprule->s_ip=prules->s_ip;
				pudprule->s_mask=prules->s_mask;
				pudprule->d_ip=prules->d_ip;
				pudprule->d_mask=prules->d_mask;
				pudprule->next=NULL;

				if (g_Rules.udp.pout==NULL) //头节点
				{
					g_Rules.udp.pout=pudprule;
					pu_out=pudprule;
				}
				else
				{
					pudprule->next=g_Rules.udp.pout;
					g_Rules.udp.pout=pudprule;
				}
			}
			break;

		case PT_ICMP:
			if (prules->direct==DT_IN)
			{
				picmprule=(struct Fw_Rule_Node *)UTIL_AllocateMemory(sizeof(struct Fw_Rule_Node));
				picmprule->action=prules->action;
				picmprule->s_port=prules->s_port;
				picmprule->d_port=prules->d_port;
				picmprule->direct=prules->direct;
				picmprule->id=prules->id;
				picmprule->s_ip=prules->s_ip;
				picmprule->s_mask=prules->s_mask;
				picmprule->d_ip=prules->d_ip;
				picmprule->d_mask=prules->d_mask;
				picmprule->next=NULL;
				
				if (g_Rules.icmp.pin==NULL) //头节点
				{
					g_Rules.icmp.pin=picmprule;
					pi_in=picmprule;
				}
				else
				{
					picmprule->next=g_Rules.icmp.pin;
					g_Rules.icmp.pin=picmprule;
				}
			}
			if (prules->direct==DT_OUT)
			{
				picmprule=(struct Fw_Rule_Node *)UTIL_AllocateMemory(sizeof(struct Fw_Rule_Node));
				picmprule->action=prules->action;
				picmprule->s_port=prules->s_port;
				picmprule->d_port=prules->d_port;
				picmprule->direct=prules->direct;
				picmprule->id=prules->id;
				picmprule->s_ip=prules->s_ip;
				picmprule->s_mask=prules->s_mask;
				picmprule->d_ip=prules->d_ip;
				picmprule->d_mask=prules->d_mask;
				picmprule->next=NULL;

				if (g_Rules.icmp.pout==NULL) //头节点
				{
					g_Rules.icmp.pout=picmprule;
					pi_out=picmprule;
				}
				else
				{
					picmprule->next=g_Rules.icmp.pout;
					g_Rules.icmp.pout=picmprule;
				}
			}
			break;

		default:
			//return FALSE;		//skip not define protocol
			break;

							
		}   // end of switch (protocol)
		
//	}   //end of for
	DbgPrint("<==UTIL_AddOnerule()");
	return TRUE;
}

/////////////////////////////////////////////////////////////////
//  
//函数UTIL_DelOneRule
//
//参数
//prules 指向要删的规则！
//
//返回值
//是，否成功
//
//
//备注
//删除一条规则

BOOLEAN UTIL_DelOneRule(struct Rules *prules)
{
	struct Fw_Rule_Node		*ptr_current=NULL;
	struct Fw_Rule_Node		*ptr_next=NULL;
	DbgPrint("==>UTIL_DelOnerule()");

	switch (prules->protocol)
	{
	case PT_TCP:
		if (prules->direct == DT_IN)
		{

			//tcp in chain
			if (g_Rules.tcp.pin!=NULL)
			{
				ptr_current=g_Rules.tcp.pin;
				ptr_next=ptr_current->next;
					
				if(ptr_next==NULL)  //注意== 和 = ，MAY 7 因为这里搞写错了，浪费两个小时Debug
				{
					if((ptr_current->d_ip==prules->d_ip)&&(ptr_current->s_ip==prules->s_ip)&&
							(ptr_current->d_port==prules->d_port)&&(ptr_current->s_port==prules->s_port)&&
							(ptr_current->action==prules->action))
					{
						g_Rules.tcp.pin=NULL;
						UTIL_FreeMemory((void *)ptr_current);
						
						return TRUE;
					}
				}
	
				while (ptr_next!=NULL)
				{
					if((ptr_next->d_ip==prules->d_ip)&&(ptr_next->s_ip==prules->s_ip)&&
						(ptr_next->d_port==prules->d_port)&&(ptr_next->s_port==prules->s_port)&&
						(ptr_next->action==prules->action))
					{
						ptr_current->next=ptr_next->next;
						UTIL_FreeMemory((void *)ptr_next);
						return TRUE;
					}
					ptr_current=ptr_next;
					if (ptr_current->next!=NULL)
						ptr_next=ptr_current->next;
				}
			}
		}

		if(prules->direct == DT_OUT)
		{
					
			//tcp out chain
			if (g_Rules.tcp.pout!=NULL)
			{
				ptr_current=g_Rules.tcp.pout;
				ptr_next=ptr_current->next;
		
				if(ptr_next==NULL)
				{
					if((ptr_current->d_ip==prules->d_ip)&&(ptr_current->s_ip==prules->s_ip)&&
							(ptr_current->d_port==prules->d_port)&&(ptr_current->s_port==prules->s_port)&&
							(ptr_current->action==prules->action))
					{
						g_Rules.tcp.pout=NULL;
						UTIL_FreeMemory((void *)ptr_current);


						return TRUE;
					}
				}

	
				while (ptr_next!=NULL)
				{
					if((ptr_next->d_ip==prules->d_ip)&&(ptr_next->s_ip==prules->s_ip)&&
						(ptr_next->d_port==prules->d_port)&&(ptr_next->s_port==prules->s_port)&&
						(ptr_next->action==prules->action))
					{
						ptr_current->next=ptr_next->next;
						UTIL_FreeMemory((void *)ptr_next);
						return TRUE;
					}
					ptr_current=ptr_next;
					if (ptr_current->next!=NULL)
						ptr_next=ptr_current->next;
				}
			}

		}
		break;

	case PT_UDP:
		if (prules->direct==DT_IN)
		{

			//udp in chain
			if (g_Rules.udp.pin!=NULL)
			{
				ptr_current=g_Rules.udp.pin;
				ptr_next=ptr_current->next;
		
				if(ptr_next==NULL)
				{
					if((ptr_current->d_ip==prules->d_ip)&&(ptr_current->s_ip==prules->s_ip)&&
							(ptr_current->d_port==prules->d_port)&&(ptr_current->s_port==prules->s_port)&&
							(ptr_current->action==prules->action))
					{
						g_Rules.udp.pin=NULL;
						UTIL_FreeMemory((void *)ptr_current);

						return TRUE;
					}
				}

	
				while (ptr_next!=NULL)
				{
					if((ptr_next->d_ip==prules->d_ip)&&(ptr_next->s_ip==prules->s_ip)&&
						(ptr_next->d_port==prules->d_port)&&(ptr_next->s_port==prules->s_port)&&
						(ptr_next->action==prules->action))
					{
						ptr_current->next=ptr_next->next;
						UTIL_FreeMemory((void *)ptr_next);
						return TRUE;
					}
					ptr_current=ptr_next;
					if (ptr_current->next!=NULL)
						ptr_next=ptr_current->next;
				}
			}
		}

		if (prules->direct==DT_OUT)
		{

			//udp out chain
			if (g_Rules.udp.pout!=NULL)
			{
				ptr_current=g_Rules.udp.pout;
				ptr_next=ptr_current->next;
		
				if(ptr_next==NULL)
				{
					if((ptr_current->d_ip==prules->d_ip)&&(ptr_current->s_ip==prules->s_ip)&&
							(ptr_current->d_port==prules->d_port)&&(ptr_current->s_port==prules->s_port)&&
							(ptr_current->action==prules->action))
					{
						g_Rules.udp.pout=NULL;
						UTIL_FreeMemory((void *)ptr_current);

						return TRUE;
					}
				}

	
				while (ptr_next!=NULL)
				{
					if((ptr_next->d_ip==prules->d_ip)&&(ptr_next->s_ip==prules->s_ip)&&
						(ptr_next->d_port==prules->d_port)&&(ptr_next->s_port==prules->s_port)&&
						(ptr_next->action==prules->action))
					{
						ptr_current->next=ptr_next->next;
						UTIL_FreeMemory((void *)ptr_next);
						return TRUE;
					}
					ptr_current=ptr_next;
					if (ptr_current->next!=NULL)
						ptr_next=ptr_current->next;
				}
			}
		}
		break;

	case PT_ICMP:
		if (prules->direct==DT_IN)
		{

			//imcp in chain
			if (g_Rules.icmp.pin!=NULL)
			{
				ptr_current=g_Rules.icmp.pin;
				ptr_next=ptr_current->next;

				if(ptr_next==NULL)
				{
					if((ptr_current->d_ip==prules->d_ip)&&(ptr_current->s_ip==prules->s_ip)&&
							(ptr_current->action==prules->action))
					{
						g_Rules.icmp.pin=NULL;
						UTIL_FreeMemory((void *)ptr_current);						

						return TRUE;
					}
				}

	
				while (ptr_next!=NULL)
				{
					if((ptr_next->d_ip==prules->d_ip)&&(ptr_next->s_ip==prules->s_ip)&&
						(ptr_next->d_port==prules->d_port)&&(ptr_next->s_port==prules->s_port)&&
						(ptr_next->action==prules->action))
					{
						ptr_current->next=ptr_next->next;
						UTIL_FreeMemory((void *)ptr_next);
						return TRUE;
					}
					ptr_current=ptr_next;
					if (ptr_current->next!=NULL)
						ptr_next=ptr_current->next;
				}
			}
		}

		if (prules->direct==DT_OUT)
		{

			//Icmp out chain
			if (g_Rules.icmp.pout!=NULL)
			{
				ptr_current=g_Rules.icmp.pout;
				ptr_next=ptr_current->next;
		
				if(ptr_next==NULL)
				{
					if((ptr_current->d_ip==prules->d_ip)&&(ptr_current->s_ip==prules->s_ip)&&
							(ptr_current->action==prules->action))
					{
						g_Rules.icmp.pout=NULL;
						UTIL_FreeMemory((void *)ptr_current);						

						return TRUE;
					}
				}

	
				while (ptr_next!=NULL)
				{
					if((ptr_next->d_ip==prules->d_ip)&&(ptr_next->s_ip==prules->s_ip)&&
						(ptr_next->d_port==prules->d_port)&&(ptr_next->s_port==prules->s_port)&&
						(ptr_next->action==prules->action))
					{
						ptr_current->next=ptr_next->next;
						UTIL_FreeMemory((void *)ptr_next);
						return TRUE;
					}
					ptr_current=ptr_next;
					if (ptr_current->next!=NULL)
						ptr_next=ptr_current->next;
				}
			}

		}
		break;

		default:
			//return FALSE;		//skip not define protocol
			break;

							
		}   // end of switch (protocol)
		
//	}   //end of for
	DbgPrint("<==UTIL_DelOnerule()");
	return TRUE;
}

/////////////////////////////////////////////////////////////////
//  
//函数UTIL_DelAllrules
//
//参数
//无
//
//返回值
//是，否成功
//
//
//备注
//删除所有规则

BOOLEAN UTIL_DelAllrules(void)
{

	struct Fw_Rule_Node		*ptr_current=NULL;
	struct Fw_Rule_Node		*ptr_next=NULL;

	DbgPrint("==>UTIL_DelAllrules()");

	//tcp in chain
	if (g_Rules.tcp.pin!=NULL)
	{
		ptr_current=g_Rules.tcp.pin;
		ptr_next=ptr_current->next;
		while (ptr_current!=NULL)
		{
			UTIL_FreeMemory((void *)ptr_current);
			ptr_current=ptr_next;
			if (ptr_current!=NULL)
				ptr_next=ptr_current->next;
		}
		g_Rules.tcp.pin=NULL;
	}
	//tcp out chain
	if (g_Rules.tcp.pout!=NULL)
	{
		ptr_current=g_Rules.tcp.pout;
		ptr_next=ptr_current->next;
		while (ptr_current!=NULL)
		{
			UTIL_FreeMemory((void *)ptr_current);
			ptr_current=ptr_next;
			if (ptr_current!=NULL)
				ptr_next=ptr_current->next;
		}
		g_Rules.tcp.pout=NULL;
	}

	//udp in chain
	if (g_Rules.udp.pin!=NULL)
	{
		ptr_current=g_Rules.udp.pin;
		ptr_next=ptr_current->next;
		while (ptr_current!=NULL)
		{
			UTIL_FreeMemory((void *)ptr_current);
			ptr_current=ptr_next;
			if (ptr_current!=NULL)
				ptr_next=ptr_current->next;
		}
		g_Rules.udp.pin=NULL;
	}
	//udp out chain
	if (g_Rules.udp.pout!=NULL)
	{
		ptr_current=g_Rules.udp.pout;
		ptr_next=ptr_current->next;
		while (ptr_current!=NULL)
		{
			UTIL_FreeMemory((void *)ptr_current);
			ptr_current=ptr_next;
			if (ptr_current!=NULL)
				ptr_next=ptr_current->next;
		}
		g_Rules.udp.pout=NULL;
	}

	//icmp in chain
	if (g_Rules.icmp.pin!=NULL)
	{
		ptr_current=g_Rules.icmp.pin;
		ptr_next=ptr_current->next;
		while (ptr_current!=NULL)
		{
			UTIL_FreeMemory((void *)ptr_current);
			ptr_current=ptr_next;
			if (ptr_current!=NULL)
				ptr_next=ptr_current->next;
		}
		g_Rules.icmp.pin=NULL;
	}
	//icmp out chain
	if (g_Rules.icmp.pout!=NULL)
	{
		ptr_current=g_Rules.icmp.pout;
		ptr_next=ptr_current->next;
		while (ptr_current!=NULL)
		{
			UTIL_FreeMemory((void *)ptr_current);
			ptr_current=ptr_next;
			if (ptr_current!=NULL)
				ptr_next=ptr_current->next;
		}
		g_Rules.icmp.pout=NULL;
	}
	DbgPrint("<==UTIL_DelAllrules()");
	return TRUE;

}



UCHAR	UTIL_MatchTcpRule(struct ip *pIpHeader, struct tcphdr *pTcpHeader, char Direct)
{
	UCHAR		nAction=NOT_MATCH;
	USHORT		nSrcPort, nDstPort;
	ULONG		nSrcIP, nDstIP;
	struct Fw_Rule	*pTcpRule=NULL;
	struct Fw_Rule_Node	*ptr=NULL;
	
	pTcpRule=&g_Rules.tcp;
	if(pTcpRule->disable) return DENY;	//不允许TCP协议通过 

	DBGPRINT(("==>UTIL_MatchTcpRule TCP包方向：%d",Direct));
	if(Direct==DT_OUT)
	{
		nSrcIP=UTIL_ntohl(pIpHeader->ip_src.s_addr);
		nDstIP=UTIL_ntohl(pIpHeader->ip_dst.s_addr);
		nSrcPort=UTIL_ntohs(pTcpHeader->th_sport);
		nDstPort=UTIL_ntohs(pTcpHeader->th_dport);
		ptr=pTcpRule->pout;
	}
	else
	{
		nSrcIP=UTIL_ntohl(pIpHeader->ip_src.s_addr);
		nDstIP=UTIL_ntohl(pIpHeader->ip_dst.s_addr);
		nSrcPort=UTIL_ntohs(pTcpHeader->th_sport);
		nDstPort=UTIL_ntohs(pTcpHeader->th_dport);
		ptr=pTcpRule->pin;
	}
/*
	DBGPRINT(("Rule direct：%d",ptr->direct));
	DBGPRINT(("Packet Server IP：%d",nSrcIP & ptr->s_mask));
	DBGPRINT(("Rule Server IP：%d",ptr->s_ip));
	DBGPRINT(("Packet Client IP：%d",nDstIP & ptr->d_mask));
	DBGPRINT(("Rule Client IP：%d",ptr->d_ip));
	DBGPRINT(("Packet Server Port：%d",nSrcPort));
	DBGPRINT(("Rule Server Port：%d",ptr->s_port));
	DBGPRINT(("Packet Client Port：%d",nDstPort));
	DBGPRINT(("Rule Client Port：%d",ptr->d_port));
*/
	for(; ptr!=NULL; ptr=ptr->next)
	{
		if(((nSrcPort==ptr->s_port)||(ptr->s_port==0)) && 
			((nDstPort==ptr->d_port)||(ptr->d_port==0)) &&
			((ptr->s_ip==(nSrcIP & ptr->s_mask))||(ptr->s_ip==0))&&
			((ptr->d_ip==(nDstIP & ptr->d_mask))||(ptr->d_ip==0))&&
			(Direct==ptr->direct))
		{
				nAction=ptr->action;
				break;
		}
	}

	UTIL_CountPacket(PT_TCP, Direct, nAction, (int)UTIL_ntohs(pIpHeader->ip_len));
	if (nAction!=NOT_MATCH)
		{
		if (!g_Rules.global.log_disable)
			UTIL_TcpWriteLog(ptr, pIpHeader,pTcpHeader,Direct);
		}
	return nAction;
}


UCHAR	UTIL_MatchUdpRule(struct ip *pIpHeader, struct udphdr *pUdpHeader, char Direct)
{
	UCHAR		nAction=NOT_MATCH;
	USHORT		nSrcPort, nDstPort;
	ULONG		nSrcIP, nDstIP;
	struct Fw_Rule_Node	*ptr=NULL;
	
	if(g_Rules.udp.disable) return DENY;	//不允许UDP协议通过 

	if(Direct==DT_OUT)
	{
		nSrcIP=UTIL_ntohl(pIpHeader->ip_src.s_addr);
		nDstIP=UTIL_ntohl(pIpHeader->ip_dst.s_addr);
		nSrcPort=UTIL_ntohs(pUdpHeader->uh_sport);
		nDstPort=UTIL_ntohs(pUdpHeader->uh_dport);
		ptr=g_Rules.udp.pout;
	}
	else
	{
		nSrcIP=UTIL_ntohl(pIpHeader->ip_src.s_addr);
		nDstIP=UTIL_ntohl(pIpHeader->ip_dst.s_addr);
		nSrcPort=UTIL_ntohs(pUdpHeader->uh_sport);
		nDstPort=UTIL_ntohs(pUdpHeader->uh_dport);
		ptr=g_Rules.udp.pin;
	}

	for(; ptr!=NULL; ptr=ptr->next)
	{
		if(((nSrcPort==ptr->s_port)||(ptr->s_port==0)) && 
			((nDstPort==ptr->d_port)||(ptr->d_port==0)) &&
			((ptr->s_ip==(nSrcIP & ptr->s_mask))||(ptr->s_ip==0))&&
			((ptr->d_ip==(nDstIP & ptr->d_mask))||(ptr->d_ip==0))&&
			(Direct==ptr->direct))
		{
				nAction=ptr->action;
				break;
		}
	}

	UTIL_CountPacket(PT_UDP, Direct, nAction, (int)UTIL_ntohs(pIpHeader->ip_len));
	if (nAction!=NOT_MATCH)
		{
		if (!g_Rules.global.log_disable)
			UTIL_UdpWriteLog(ptr, pIpHeader, pUdpHeader,Direct);
		}
	return nAction;
}


UCHAR	UTIL_MatchIcmpRule(struct ip *pIpHeader, struct icmp *pIcmpHeader, char Direct)
{
	UCHAR		nAction=NOT_MATCH;
	ULONG		nSrcIP, nDstIP;
	struct Fw_Rule_Node	*ptr=NULL;
	
	if(g_Rules.icmp.disable) return DENY;	//不允许ICMP协议通过 

	if(Direct==DT_OUT)
	{
		nSrcIP=UTIL_ntohl(pIpHeader->ip_src.s_addr);
		nDstIP=UTIL_ntohl(pIpHeader->ip_dst.s_addr);
		ptr=g_Rules.icmp.pout;
	}
	else
	{
		nSrcIP=UTIL_ntohl(pIpHeader->ip_src.s_addr);
		nDstIP=UTIL_ntohl(pIpHeader->ip_dst.s_addr);
		ptr=g_Rules.icmp.pin;
	}

	for(; ptr!=NULL; ptr=ptr->next)
	{
		if(((ptr->s_ip==(nSrcIP & ptr->s_mask))||(ptr->s_ip==0))&&
			((ptr->d_ip==(nDstIP & ptr->d_mask))||(ptr->d_ip==0))&&
			(Direct==ptr->direct))
		{
				nAction=ptr->action;
				break;
		}
	}

	UTIL_CountPacket(PT_ICMP, Direct, nAction, (int)UTIL_ntohs(pIpHeader->ip_len));
	if (nAction!=NOT_MATCH)
		{
		if (!g_Rules.global.log_disable)
			UTIL_IcmpWriteLog(ptr, pIpHeader, pIcmpHeader,Direct);
		}
	return nAction;
}



///////////////////////////////////////////////////////////////////////////////
// add by xiaoqiang mar 18 2006

UCHAR UTIL_MatchRule(char *pPacketContent, char Direct)
{
	UCHAR		nProtocol;
	USHORT		nEtherType = 0, nIPhdrLen, nIPOffset;
	UCHAR		nAction;
	ULONG 		datetime=0;
	nAction = ALLOW;

	DBGPRINT(("==>UTIL_MatchRule(...)"));
	//如果防火墙状态为切断所有则返回
	if(g_Rules.global.fw_status==FWS_DENYALL) return DENY;
	if(g_Rules.global.fw_status==FWS_ALLOWALL) return ALLOW;

	// Copy the data.
     
	if(1)		
	{
		// Copy the data.
//		DBGPRINT(("判断是否以太网包"));
		NdisMoveMemory(			
			(PUCHAR)&nEtherType,
			(pPacketContent + MEtherType),//MEtherType=12
			sizeof(USHORT)
			);
	    
		nEtherType = UTIL_htons( nEtherType );	//将包类型转换为网络顺序
		if( nEtherType == ETHERTYPE_IP )		//if 包类型是IP包
		{
			//read ip header
			nAction = ALLOW;
			NdisMoveMemory(				
				(PUCHAR )g_pIPHeader,
				pPacketContent + MHdrSize,//MHdrSize=14
				sizeof(*g_pIPHeader)
				);	

			nIPOffset=UTIL_ntohs(g_pIPHeader->ip_off);
			#pragma warning(disable:4554)
			if((nIPOffset&IP_DF==0)&&(nIPOffset&0x1FFF!=0))	//分片非第一片允许通过
				return ALLOW;

			nIPhdrLen=g_pIPHeader->ip_hl*4;//计算IP头长度
			nProtocol=g_pIPHeader->ip_p;

			switch(nProtocol)
			{
				case IPPROTO_TCP:
					NdisMoveMemory(
						(PUCHAR)g_pTCPHeader,
						//包首指针+帧头长度+IP头长度，就是TCP头指针
						pPacketContent + MHdrSize + nIPhdrLen,
						sizeof(*g_pTCPHeader)
						);
					DBGPRINT(("数据包类型：TCP包,方向：%d",Direct));
					nAction=UTIL_MatchTcpRule(g_pIPHeader, g_pTCPHeader, Direct);
					return nAction;
					break;

				case IPPROTO_UDP:
					NdisMoveMemory(						
						(PUCHAR)g_pUDPHeader,
						pPacketContent + MHdrSize + nIPhdrLen,
						sizeof(*g_pUDPHeader)
						);
					DBGPRINT(("数据包类型：UDP包"));
					nAction=UTIL_MatchUdpRule(g_pIPHeader, g_pUDPHeader, Direct);
					return nAction;
					break;

				case IPPROTO_ICMP:
					NdisMoveMemory(						
						(PUCHAR)g_pICMPHeader,
						pPacketContent + MHdrSize + nIPhdrLen,
						sizeof(*g_pICMPHeader)
						);
					DBGPRINT(("数据包类型：ICMP包"));
					nAction=UTIL_MatchIcmpRule(g_pIPHeader, g_pICMPHeader, Direct);
					return nAction;
					break;
			}
		}//else在这就可以进行ARP防火墙设计
		DBGPRINT(("<==UTIL_MatchRule(...)"));
		if(g_Rules.global.fw_mode==RM_DENY)
			return DENY;
		else 
			return ALLOW;
	}
}
