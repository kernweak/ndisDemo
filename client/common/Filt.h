

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
	char	tcp_disable;		//是否允许TCP协议
	char	udp_disable;		//是否允许UDP协议
	char	icmp_disable;		//是否允许ICMP协议
};

struct _RuleLog {
	u_long			id;				//Rule的号码
	u_char			protocol;       //协议类型  
	u_long			s_ip;			//原IP
	u_long			s_mask;		//掩码
	u_short			s_port;			//原端口
	u_long			d_ip;			//目的IP
	u_long			d_mask;		//掩码
	u_short			d_port;			//目的端口
	u_char			direct;			//方向
	u_char 			action;        //是否拦截
	char		    sMemo[100]; //备注	
};



struct _Config {
	char firewall_status; 		//全局防火墙开启（有规则）或关闭（最放松，没有规则）或全局设置断开所有（最安全）
	char mode;			//全局模式设置（除了禁止都允许或除了允许都禁止）
	char log_disable;		//是否记录日志
	struct Proto_Restrict proto;	//协议禁止设置
//	USHORT	rules_num;		//规则数目
//	struct Rule Rules[MAX_RULE_NUM];//规则数组
};


struct		All_Rules {
u_short			rules_num;				//规则数目
RuleLog 	RuleLogs[MAX_RULE_NUM];	//规则数组
};


//Define firewall statistics  按规则拦截到的数据包的大小数据

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
