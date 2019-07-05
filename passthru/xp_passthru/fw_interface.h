
/* fw_interface.h	
	writing by qsc @ 2001-06-19
	define firewall NDIS IM Driver's interface and constants
	
*/


#ifdef _WINDOWS
#include <WinIoctl.h>   // Compiling Win32 Applications Or DLL's
#include <string.h>		// inline function
//#include "DDKLBLInc.h"
#pragma intrinsic(memcpy)
#endif // _WINDOWS


//
// Specify Structure Packing
//
#pragma pack(1)      // x86, MS compiler; MIPS, MIPS compiler

//----------------------------------------------------

#define MAX_RULE_NUM 255

struct Proto_Restrict {
	char	tcp_disable;		//�Ƿ�����TCPЭ��
	char	udp_disable;		//�Ƿ�����UDPЭ��
	char	icmp_disable;		//�Ƿ�����ICMPЭ��
};

struct Rules {
	u_long			id;				//Rule�ĺ���
	u_char			protocol;       //Э������  
	u_long			s_ip;			//ԭIP
	u_long			s_mask;		//����
	u_short			s_port;			//ԭ�˿�
	u_long			d_ip;			//Ŀ��IP
	u_long			d_mask;		//����
	u_short			d_port;			//Ŀ�Ķ˿�
	u_char			direct;			//����
	u_char 			action;        //�Ƿ�����
	char		    sMemo[100]; //��ע		
};

struct RuleSet {
	char firewall_status; 		//ȫ�ַ���ǽ�������й��򣩻�رգ�����ɣ�û�й��򣩻�ȫ�����öϿ����У��ȫ��
	char mode;			//ȫ��ģʽ���ã����˽�ֹ����������������ֹ��
	char log_disable;		//�Ƿ��¼��־
	struct Proto_Restrict proto;	//Э���ֹ����
//	USHORT	rules_num;		//������Ŀ
//	struct Rule Rules[MAX_RULE_NUM];//��������
};

/*
struct		All_Rules {
USHORT			rules_num;				//������Ŀ
struct 	Rule 	Rules[MAX_RULE_NUM];	//��������
};
*/

//Define firewall statistics  ���������ص������ݰ��Ĵ�С����

struct	Pkt_Stat {
	 int	allow_in_pkts;
	 int	allow_in_bytes;
	 int	allow_out_pkts;
	 int 	allow_out_bytes;
	
	 int	deny_in_pkts;
	 int 	deny_in_bytes;
	 int	deny_out_pkts;
	 int	deny_out_bytes;
};

struct	Ip_Stat	{
	struct	Pkt_Stat	tcp;
	struct	Pkt_Stat	udp;
	struct	Pkt_Stat	icmp;
//	struct	Pkt_Stat	restip;
};

struct	Notip_Stat {
	struct Pkt_Stat	other_stat;
};

struct	Statistics {
	struct	Ip_Stat	ip;
	struct	Notip_Stat	notip;
};

//
// Restore Default Structure Packing
//
#pragma pack()                  // x86, MS compiler; MIPS, MIPS compiler
