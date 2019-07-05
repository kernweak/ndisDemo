#ifndef _DRIVERDEF_H
#define _DRIVERDEF_H

#define APP_VERSION				100

#define	FLAG_SHARE_MEMORY_FILE	0x48464D53		// SMFH
#define FLAG_PACKET_BUFFER		0x4250			// PB
#define VERSION_SHARE_MEMORY	0x101

#define LOCK_DELAY				50

#define CHECK_PASS				0
#define CHECK_DENY				1

#define FILE_DEVICE_DRIVER		(32767 + 78)	// 0x7FFF + 78
#define DRIVER_API_BASE			(2047 + 78)		// 0x07FF + 78

#define CTL_CODE_EX(Function)	CTL_CODE(FILE_DEVICE_DRIVER, \
			(DRIVER_API_BASE + Function), METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MALLOC_SHARE_BUFFER		CTL_CODE_EX(0)
#define IOCTL_MALLOC_SHARE_BUFFER_IP	CTL_CODE_EX(1)
#define IOCTL_MALLOC_SHARE_BUFFER_URL	CTL_CODE_EX(2)
#define IOCTL_FREE_SHARE_BUFFER 		CTL_CODE_EX(3)
#define IOCTL_GET_SHARE_BUFFER 			CTL_CODE_EX(4)
#define IOCTL_GET_SHARE_BUFFER_IP		CTL_CODE_EX(5)
#define IOCTL_GET_SHARE_BUFFER_URL		CTL_CODE_EX(6)
#define IOCTL_GET_USER_PACKET_BUFFER	CTL_CODE_EX(7)
#define IOCTL_SET_FILTER_MODE	 		CTL_CODE_EX(8)
#define IOCTL_GET_VERSION	 			CTL_CODE_EX(9)

#define NET_SERVICE_TYPE_ALL			0
#define NET_SERVICE_TYPE_TCP			1
#define NET_SERVICE_TYPE_UDP			2
#define NET_SERVICE_TYPE_FTP			3
#define NET_SERVICE_TYPE_TELNET			4
#define NET_SERVICE_TYPE_HTTP			5
#define NET_SERVICE_TYPE_NNTP			6
#define NET_SERVICE_TYPE_POP3			7
#define NET_SERVICE_TYPE_SMTP			8
#define NET_SERVICE_TYPE_ICMP			9

#define NET_DIRECTION_IN				0
#define NET_DIRECTION_OUT				1

#define MAX_PACKET_BUFFER				512
#define MAX_PACKET_ONLINE				1024

#define MAX_URL_LENGTH					128
#define MAX_CUST_URL_LENGTH				50
#define MAX_CUST_KEY_LENGTH				8
#define MAX_CUST_UTF8KEY_LENGTH			MAX_CUST_KEY_LENGTH/2*3*3

typedef struct tagURL			URL,*PURL;
struct tagURL
{
	char szUrl[MAX_CUST_URL_LENGTH+1];
};

typedef struct tagKEY			KEY,*PKEY;
struct tagKEY
{
	char szKey[MAX_CUST_KEY_LENGTH+1];
};

typedef struct tagUTF8KEY		UTF8KEY,*PUTF8KEY;
struct tagUTF8KEY
{
	char szKey[MAX_CUST_UTF8KEY_LENGTH+1];
};

typedef struct tagMAC_ADDRESS	MAC_ADDRESS,*PMAC_ADDRESS;
struct tagMAC_ADDRESS
{
	union
	{
		BYTE	mac[6];
		struct
		{
			BYTE c0;
			BYTE c1;
			BYTE c2;
			BYTE c3;
			BYTE c4;
			BYTE c5;
		};
	};
};

#define PBS_LOG		1
#define PBS_FLUX	2

typedef struct tagPACKET_BUFFER	PACKET_BUFFER,*PPACKET_BUFFER;
struct tagPACKET_BUFFER
{
	UINT		uiFlag;
	BYTE		Style;
	BYTE		bFilter;
	union
	{
		struct
		{
			USHORT	TcpCode		: 6;
			USHORT	Reserved	: 10;
		};
		struct
		{
			USHORT	TcpFin		: 1;
			USHORT	TcpSyn		: 1;
			USHORT	TcpRst		: 1;
			USHORT	TcpPsh		: 1;
			USHORT	TcpAck		: 1;
			USHORT	TcpUrg		: 1;

			USHORT	Direction	: 1;		// 0: IN, 1: OUT
			USHORT	SendOrRecv	: 1;
			USHORT	Action		: 8;
		};
	};
	DWORD	dwSourceIp;
	DWORD	dwDestinationIp;
	WORD	wSourcePort;
	WORD	wDestinationPort;

	WORD	wPort;
	WORD	wTransmitNum;
	ULONG	Time;
//	MAC_ADDRESS		Mac;
	char	szUrl[MAX_URL_LENGTH];
};

typedef struct tagPACKET_BUFFER_POINT	PACKET_BUFFER_POINT,*PPACKET_BUFFER_POINT;
struct tagPACKET_BUFFER_POINT
{
	int		nMaxCount;
	int*	pnWriteIndex;
	int*	pnReadIndex;
	PPACKET_BUFFER pPacket;
};

typedef struct tagSHARE_MEMORY_HEADER	SHARE_MEMORY_HEADER,*PSHARE_MEMORY_HEADER;
struct tagSHARE_MEMORY_HEADER
{
	DWORD		dwFlag;						//
	WORD		wHeaderLength;				//
	WORD		wVersion;					//
	BYTE		bLock;						//
	BYTE		bEnableSexIpList;			//
	BYTE		bEnableSexUrlList;			//
	BYTE		bEnableSafetyIplist;		//
	BYTE		bEnableBanIplist;
	BYTE		bEnableCustIpList;
	BYTE		bEnableCustUrlList;
	BYTE		bEnableWebKeyList;
	BYTE		bEnableUrlKeyList;
	BYTE		bEnablePortList;
	BYTE		bEnableLog;
	BYTE		bLogBanedIpOnly;
	BYTE		bReserve1;
	BYTE		bReserve2;
	BYTE		bReserve3;
	BYTE		bReserve4;
	DWORD		dwSexIpNum;
	DWORD		dwSexUrlNum;
	DWORD		dwSafetyIpNum;
	DWORD		dwSafetyIpOffset;
	DWORD		dwKeyNum;
	DWORD		dwKeyOffset;
	DWORD		dwUtf8KeyNum;
	DWORD		dwUtf8KeyOffset;
	DWORD		dwPortNum;
	DWORD		dwPortOffset;
	DWORD		dwBanIpNum;
	DWORD		dwBanIpOffset;
	DWORD		dwCustIpNum;
	DWORD		dwCustIpOffset;
	DWORD		dwCustUrlNum;
	DWORD		dwCustUrlOffset;
};

#endif