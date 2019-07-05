/**************************************************************
				    日志结构定义   add by xiaoqiang
***************************************************************/
#include "DDKLBLInc.h"
#ifndef fw_log
#define fw_log

#pragma pack(1)

struct Log {
	u_long			ID;				//Rule的号码
	CTime			time;			//时间
	u_char			protocol;       //协议类型  
	u_long			s_ip;			//原IP
	u_short			s_port;			//原端口
	u_long			d_ip;			//目的IP
	u_short			d_port;			//目的端口
	u_char			direct;			//方向
	u_char 			action;        //是否拦截
	char		    sMemo[100];//备注
};

#pragma pack()
#endif //fw_log