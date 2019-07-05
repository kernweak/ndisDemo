
/**************************************************************
				     规则结构
***************************************************************/
//#include "DDKLBLInc.h"

#pragma pack(1)

struct	Fw_Global {
	char			fw_status;			    //防火墙状态
	char			log_disable;				//是否允许记日志
	char			fw_mode;				//除了禁止的都允许、除了允许的都禁止
};

struct	Fw_Rule_Node  {
	u_long			id;				//Rule的号码
	u_char			protocol;       //协议类型  
	u_long			s_ip;			//原IP
	u_long			s_mask;		//掩码
	u_short			s_port;			//原端口
	u_long			d_ip;			//目的IP
	u_long			d_mask;		//掩码
	u_short	d_port;			//目的端口
	u_char			direct;			//方向
	u_char 			action;        //是否拦截
	char		    sMemo[100];//备注	
	struct	Fw_Rule_Node	*next;			//指向下个节点的指针
};

struct	Fw_Rule {
	u_char					disable;			//是否允许协议
	struct	Fw_Rule_Node	*pin, *pout;	//指向下个in, out节点的指针
};


//总体结构：
struct	 	Fw_Rules {
	struct		Fw_Global	global; 
	struct		Fw_Rule		tcp;
	struct		Fw_Rule		udp;
	struct		Fw_Rule		icmp;
	struct		Fw_Rule		ip;
};

#pragma pack()
