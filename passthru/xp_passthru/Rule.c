
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

extern struct Fw_Rules g_Rules;     //ȫ�ֹ������
extern struct Statistics  g_stat;   //ͳ�ư��Ĵ�С
extern struct ip 		*g_pIPHeader;
extern struct tcphdr	*g_pTCPHeader;
extern struct udphdr	*g_pUDPHeader;
extern struct icmp		*g_pICMPHeader;

extern struct Log g_Log, *g_pLog;   //��ʱ�ӵġ������޸�


USHORT  	alarm_num=0,log_num=0,acall_num=0,lcall_num=0;

#pragma warning(disable:4273)
extern LONG		KeReadStateEvent(IN PRKEVENT Event);
extern PVOID	gpEventObject;			// ��Ӧ��ͨ�ŵ��¼�����
extern PVOID	SystemVirtualAddress;	// �����ڴ��ַ


//-------------------------------CopyPacket2Buffer-------------------------------
//
// Purpose
// �ӽػ�İ�����������ȡ�����ݵ�������
// add by xiaoqiang
void CopyPacket2Buffer(IN PNDIS_PACKET pPacket,IN OUT PUCHAR pBuff,IN OUT PUINT pLength) 
{ 
	PNDIS_BUFFER BuffDT;  //����һ��bufferָʾ��
	PUCHAR BuffVA;  //ָ��һ��PNDIS_PACKET�ṹ�����Buffer�������ַ
	UINT BuffLen; //Buffer�ĳ���

	*pLength=0;  //���ݰ����ݵ��ܳ���,��ʼ������ 
	BuffLen=0;  

	NdisQueryPacket(pPacket,NULL,NULL,&BuffDT,NULL); //��ѯPacket����Ϣ����������Packet��Bufferָʾ������ĵ�һ��Buffer
	while(BuffDT!=(PNDIS_BUFFER)NULL) 
	{ 
		NdisQueryBufferSafe(BuffDT,&BuffVA,&BuffLen,32);  //�õ�BuffDTָ����Ǹ�Buffer�������BuffVA�ͳ���BuffLen 
		NdisMoveMemory(pBuff,BuffVA,BuffLen);   //��BuffVA���е����ݣ��ƶ���pBuffָ����ǿ�����
		pBuff=pBuff+BuffLen;   //pBuffָ����ƣ�ǰBuffLen���ֽ��Ѿ���������
		*pLength+=BuffLen;    //����pBuff���������ݵ��ܳ���pLength
		NdisGetNextBuffer(BuffDT,&BuffDT);  //���Bufferָʾ�������е���һ��Bufferָʾ��
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
   
    //�����ݰ����ݴ�Packet������pPacketContent
	
    NdisQueryPacket( pPacket,NULL,NULL,NULL,&PacketSize);
    NdisQueryBufferSafe(pPacket->Private.Head, &BuffVA, &BuffLen, 32 );
    NdisMoveMemory(pBuff, BuffVA, BuffLen);

    *pLength = BuffLen;
    pNext = pPacket->Private.Head;
    
    for(;;)
    {
        if(pNext == pPacket->Private.Tail)
            break;
        pNext = pNext->Next;   //ָ�����
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
// ������ݰ�����,����������UTIL_MatchRule��������
UINT Check_Packet(char *pPacket)
/*--
	return value:
	1 -> ƥ��
	0 -> ��ƥ��
--*/
/*===
{
	P_IP_PACKET p_ip;
	__try
	{
		__try
		{
			p_ip = (P_IP_PACKET)pPacket;
			// ����Ƿ�Ϊ TCP ���ݰ�
			if(p_ip->H_frame_type == 8 && p_ip->L_frame_type == 0 && p_ip->proto == 6)
			{
				DBGPRINT(("���ص�TCP���ݰ�"));   //add by xiaoqiang
				if(!KeReadStateEvent(gpEventObject))
				{
					// �������ݵ������ڴ�
					memset(SystemVirtualAddress, 0, 100);
					memcpy(SystemVirtualAddress, (char *)p_ip, sizeof(IP_PACKET));

					KeSetEvent(gpEventObject, 0, FALSE);
				}
				else{}
				return 1;
			}
		}
		except (EXCEPTION_EXECUTE_HANDLER){DbgPrint("�����쳣!  --In Check_Packet\n"); }
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
// �õ���ǰ����/ʱ�䡢���ں�ʱ��
//
// ������
//		Week:	�������ڣ�1��2 ... 7 �ֱ��ʾ �����ա�����һ ... ����6
//		pTime:	���ص�ǰʱ���ڵ��������
//
// ����ֵ��
//		ULONG��ֵ��ʾ������/ʱ�䣬������Ǵ�1970/1/1�����ڵ���������CTime��ʾ��ʽ��ͬ
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
//����UTIL_CountPacket
//
//����
//���򡢶���
//
//����ֵ
//��
//
//
//��ע
//ͳ����in��out������������ֹ�İ���in��0����out��1����allow��0��
//deny��1��


void UTIL_CountPacket (USHORT ptype,char direct,char action,int packsize)
{
	//
	//if ����Ϊin {
	//	if ����Ϊ����  ͳ�ƽṹ�ۼ�
	//	if ����Ϊ��ֹ  ͳ�ƽṹ�ۼ�
	//}
	//if ����Ϊout {
	//	if ����Ϊ����  ͳ�ƽṹ�ۼ�
	//	if ����Ϊ��ֹ  ͳ�ƽṹ�ۼ�
	//}
	//
	DbgPrint("==>UTIL_CountPacket()");

/*
	DBGPRINT(("tcpInBytes��%d",g_stat.ip.tcp.allow_in_bytes));
	DBGPRINT(("tcpInPacketNUM��%d",g_stat.ip.tcp.allow_in_pkts));
	DBGPRINT(("udpInBytes��%d",g_stat.ip.udp.allow_in_bytes));
	DBGPRINT(("udpInPacketNUM��%d",g_stat.ip.udp.allow_in_pkts));
	DBGPRINT(("tcpOutBytes��%d",g_stat.ip.tcp.allow_out_bytes));
	DBGPRINT(("tcpOutPacketNUM��%d",g_stat.ip.tcp.allow_out_pkts));
	DBGPRINT(("udpOutBytes��%d",g_stat.ip.udp.allow_out_bytes));
	DBGPRINT(("udpOutPacketNUM��%d",g_stat.ip.udp.allow_out_pkts));
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
//����UTIL_WriteLog
//
//����
//USHORT ID,struct ip cur_ip
//
//����ֵ
//��
//
//
//��ע
//��IOCTL�м���SETLOGADDRESS������ص�������ַ�����õ���ǰ�̺߳š�
//�ڴ˺����е��� _VWIN32_QueueUserApc() ����Ӧ�ó�������

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
	DBGPRINT(("==>UTIL_TcpWriteLog TCP������%d",Direct));
	g_Log.direct=Direct;
	g_Log.action=pTcpRule->action;
	for(i = 0; i < 100; i++)
	{
		g_Log.sMemo[i]=pTcpRule->sMemo[i];
	}

	#ifdef _DEBUG
	DBGPRINT(("g_Log.id��%d",g_Log.id));
	DBGPRINT(("g_Log.protocol��%d",g_Log.protocol));
	DBGPRINT(("g_Log.time��%d",g_Log.time));
	DBGPRINT(("g_Log.d_ip��%d",g_Log.d_ip));
	DBGPRINT(("g_Log.s_ip��%d",g_Log.s_ip));
	DBGPRINT(("g_Log.s_port��%d",g_Log.s_port));
	DBGPRINT(("g_Log.d_port��%d",g_Log.d_port));
	DBGPRINT(("g_Log.direct��%d",g_Log.direct));
	DBGPRINT(("g_Log.action��%d",g_Log.action));
	DBGPRINT(("g_Log.sMemo��%s",g_Log.sMemo));
	#endif

	if(!KeReadStateEvent(gpEventObject))
		{
		// �������ݵ������ڴ�
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
		// �������ݵ������ڴ�
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
		// �������ݵ������ڴ�
			memset(SystemVirtualAddress, 0, sizeof(g_Log));
			memcpy(SystemVirtualAddress, (char *)g_pLog, sizeof(g_Log));

			KeSetEvent(gpEventObject, 0, FALSE);
		}
}


/////////////////////////////////////////////////////////////////
//  
//����UTIL_SizeupPort
//
//����
//USHORT cur_port,char p_operator,USHORT port1,USHORT port2
//
//����ֵ
//TRUE,FALSE
//
//
//��ע
//cur_port�Ƿ���������жԶ˿ڵ�Ҫ��

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
//����UTIL_AddRuleSet
//
//����
//RuleSet
//
//����ֵ
//�ǣ���ɹ�
//
//
//��ע
//����ϵͳ����

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
//����UTIL_AddOneRule
//
//����
//rules
//
//����ֵ
//�ǣ���ɹ�
//
//
//��ע
//�������й���

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
				
				
				if (g_Rules.tcp.pin==NULL) //ͷ�ڵ�
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

				if (g_Rules.tcp.pout==NULL) //ͷ�ڵ�
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
				
				if (g_Rules.udp.pin==NULL) //ͷ�ڵ�
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

				if (g_Rules.udp.pout==NULL) //ͷ�ڵ�
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
				
				if (g_Rules.icmp.pin==NULL) //ͷ�ڵ�
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

				if (g_Rules.icmp.pout==NULL) //ͷ�ڵ�
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
//����UTIL_DelOneRule
//
//����
//prules ָ��Ҫɾ�Ĺ���
//
//����ֵ
//�ǣ���ɹ�
//
//
//��ע
//ɾ��һ������

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
					
				if(ptr_next==NULL)  //ע��== �� = ��MAY 7 ��Ϊ�����д���ˣ��˷�����СʱDebug
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
//����UTIL_DelAllrules
//
//����
//��
//
//����ֵ
//�ǣ���ɹ�
//
//
//��ע
//ɾ�����й���

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
	if(pTcpRule->disable) return DENY;	//������TCPЭ��ͨ�� 

	DBGPRINT(("==>UTIL_MatchTcpRule TCP������%d",Direct));
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
	DBGPRINT(("Rule direct��%d",ptr->direct));
	DBGPRINT(("Packet Server IP��%d",nSrcIP & ptr->s_mask));
	DBGPRINT(("Rule Server IP��%d",ptr->s_ip));
	DBGPRINT(("Packet Client IP��%d",nDstIP & ptr->d_mask));
	DBGPRINT(("Rule Client IP��%d",ptr->d_ip));
	DBGPRINT(("Packet Server Port��%d",nSrcPort));
	DBGPRINT(("Rule Server Port��%d",ptr->s_port));
	DBGPRINT(("Packet Client Port��%d",nDstPort));
	DBGPRINT(("Rule Client Port��%d",ptr->d_port));
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
	
	if(g_Rules.udp.disable) return DENY;	//������UDPЭ��ͨ�� 

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
	
	if(g_Rules.icmp.disable) return DENY;	//������ICMPЭ��ͨ�� 

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
	//�������ǽ״̬Ϊ�ж������򷵻�
	if(g_Rules.global.fw_status==FWS_DENYALL) return DENY;
	if(g_Rules.global.fw_status==FWS_ALLOWALL) return ALLOW;

	// Copy the data.
     
	if(1)		
	{
		// Copy the data.
//		DBGPRINT(("�ж��Ƿ���̫����"));
		NdisMoveMemory(			
			(PUCHAR)&nEtherType,
			(pPacketContent + MEtherType),//MEtherType=12
			sizeof(USHORT)
			);
	    
		nEtherType = UTIL_htons( nEtherType );	//��������ת��Ϊ����˳��
		if( nEtherType == ETHERTYPE_IP )		//if ��������IP��
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
			if((nIPOffset&IP_DF==0)&&(nIPOffset&0x1FFF!=0))	//��Ƭ�ǵ�һƬ����ͨ��
				return ALLOW;

			nIPhdrLen=g_pIPHeader->ip_hl*4;//����IPͷ����
			nProtocol=g_pIPHeader->ip_p;

			switch(nProtocol)
			{
				case IPPROTO_TCP:
					NdisMoveMemory(
						(PUCHAR)g_pTCPHeader,
						//����ָ��+֡ͷ����+IPͷ���ȣ�����TCPͷָ��
						pPacketContent + MHdrSize + nIPhdrLen,
						sizeof(*g_pTCPHeader)
						);
					DBGPRINT(("���ݰ����ͣ�TCP��,����%d",Direct));
					nAction=UTIL_MatchTcpRule(g_pIPHeader, g_pTCPHeader, Direct);
					return nAction;
					break;

				case IPPROTO_UDP:
					NdisMoveMemory(						
						(PUCHAR)g_pUDPHeader,
						pPacketContent + MHdrSize + nIPhdrLen,
						sizeof(*g_pUDPHeader)
						);
					DBGPRINT(("���ݰ����ͣ�UDP��"));
					nAction=UTIL_MatchUdpRule(g_pIPHeader, g_pUDPHeader, Direct);
					return nAction;
					break;

				case IPPROTO_ICMP:
					NdisMoveMemory(						
						(PUCHAR)g_pICMPHeader,
						pPacketContent + MHdrSize + nIPhdrLen,
						sizeof(*g_pICMPHeader)
						);
					DBGPRINT(("���ݰ����ͣ�ICMP��"));
					nAction=UTIL_MatchIcmpRule(g_pIPHeader, g_pICMPHeader, Direct);
					return nAction;
					break;
			}
		}//else����Ϳ��Խ���ARP����ǽ���
		DBGPRINT(("<==UTIL_MatchRule(...)"));
		if(g_Rules.global.fw_mode==RM_DENY)
			return DENY;
		else 
			return ALLOW;
	}
}
