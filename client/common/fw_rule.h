
/**************************************************************
				     规则结构
***************************************************************/

struct	Fw_Global {
	char			fw_status;			    //防火墙状态
	char			log_enable;				//是否允许记日志
	char			fw_mode;				//除了禁止的都允许、除了允许的都禁止
};

struct	Fw_Rule_Node  {
	ULONG			id;				//Rule的号码
	int				time;			//时间
	UCHAR			protocol;       //协议类型  
	ULONG			s_ip;			//原IP
	ULONG			s_mask;		//掩码
	unsigned short 	s_port;			//原端口
	ULONG			d_ip;			//目的IP
	ULONG			d_mask;		//掩码
	unsigned short	d_port;			//目的端口
	char			direct;			//方向
	char 			action;        //是否拦截
	char		    sMemo[100];//备注	
	struct	Fw_Rule_Node	*next;			//指向下个节点的指针
};

struct	Fw_Rule {
	char					enable;			//是否允许协议
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




