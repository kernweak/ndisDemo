

#ifndef __FILT_H__
#define __FILT_H__

#include "DDKLBLInc.h"

//
// Specify Structure Packing
//
#pragma pack(1)      // x86, MS compiler; MIPS, MIPS compiler

//----------------------------------------------------

#define MAX_RULE_NUM 255
typedef struct _RuleLog		RuleLog,		*pRuleLog;
#define RULELOG_LENTH		sizeof(RuleLog)
typedef struct _Config      Config ,        *pConfig;
#define CONFIG_LENTH	sizeof(Config)
typedef struct	Statistics   Stat  ,         *pStat;

#define ACL_DIRECTION_IN			0
#define ACL_DIRECTION_OUT			1
#define ACL_DIRECTION_IN_OUT		2

#define ACL_ACTION_PASS				1
#define ACL_ACTION_DENY				0

#define ACL_SERVICE_TYPE_ALL		3
#define ACL_SERVICE_TYPE_TCP		0
#define ACL_SERVICE_TYPE_UDP		1
#define ACL_SERVICE_TYPE_FTP		2


struct Proto_Restrict {
	char	tcp_disable;		//�Ƿ�����TCPЭ��
	char	udp_disable;		//�Ƿ�����UDPЭ��
	char	icmp_disable;		//�Ƿ�����ICMPЭ��
};

struct _RuleLog {
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



struct _Config {
	char firewall_status; 		//ȫ�ַ���ǽ�������й��򣩻�رգ�����ɣ�û�й��򣩻�ȫ�����öϿ����У��ȫ��
	char mode;			//ȫ��ģʽ���ã����˽�ֹ����������������ֹ��
	char log_disable;		//�Ƿ��¼��־
	struct Proto_Restrict proto;	//Э���ֹ����
//	USHORT	rules_num;		//������Ŀ
//	struct Rule Rules[MAX_RULE_NUM];//��������
};


struct		All_Rules {
u_short			rules_num;				//������Ŀ
RuleLog 	RuleLogs[MAX_RULE_NUM];	//��������
};


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
#endif // #ifndef __FILT_H__
