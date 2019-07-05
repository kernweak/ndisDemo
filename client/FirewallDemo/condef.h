///////////////////////////////////////////////////////
//        Add by xiaoqiang mar 20 2006
//log显示的一些define定义
///////////////////////////////////////////////////////
#ifndef	CONDEF_H
#define CONDEF_H

#define CHAR_WIDTH		10

#define GUI_ACL_EXPLAIN_CONST			_T("'*'代表所有应用程序，0代表所有端口。")
#define GUI_ACL_EXPLAIN_PORT_ALL		_T("所有")
#define GUI_ACL_EXPLAIN_OUT_FORMAT		_T("%s%s通过%s协议的%s端口访问%s的%s端口将被%s。%s")
#define GUI_ACL_EXPLAIN_IN_FORMAT		_T("%s%s通过%s协议的%s端口向本机%s%s端口发出的连接请求将被%s。%s")
#define GUI_QUERY_EXPLAIN_APP_OUT		_T("%s %s 通过 %s 协议的 %u 端口访问 %s 的 %u 端口，是否放行？")
#define GUI_QUERY_EXPLAIN_APP_IN		_T("%s %s 通过 %s 协议的 %u 端口向本机 %s %u 端口发出连接请求，是否放行？")
#define GUI_QUERY_EXPLAIN_WEB			_T("应用程序 %s 要访问站点 %s，是否放行？")

// Popup Message

#define GUI_ACL_MESSAGE_INAVALID_PORT					_T("端口值无效，有效范围为 0 - 65535，请重新输入。")


static TCHAR *LOG_HEADER[] = {
	_T("规则ID"),
	_T("协议/方向"),
	_T("服务端IP/端口"),
	_T("客户端IP/端口"),
	_T("放行/拦截"),
	_T("时间"),
	_T("备注"),
};

static int LOG_HEADER_LENTH[] = {
	_tcslen(LOG_HEADER[0]) * CHAR_WIDTH - 10,
	_tcslen(LOG_HEADER[1]) * CHAR_WIDTH - 20,
	_tcslen(LOG_HEADER[2]) * CHAR_WIDTH + 30,
	_tcslen(LOG_HEADER[3]) * CHAR_WIDTH + 30,
	_tcslen(LOG_HEADER[4]) * CHAR_WIDTH - 20,
	_tcslen(LOG_HEADER[5]) * CHAR_WIDTH + 90,
	_tcslen(LOG_HEADER[6]) * CHAR_WIDTH + 50,
};
#define LOG_HEADER_COUNT sizeof(LOG_HEADER)/sizeof(TCHAR*)

static TCHAR *RULE_HEADER[] = {
	_T("号码"),
	_T("协议/方向"),
	_T("服务端IP/端口"),
	_T("客户端IP/端口"),
	_T("放行/拦截"),
	_T("备注"),
};

static int RULE_HEADER_LENTH[] = {
	_tcslen(RULE_HEADER[0]) * CHAR_WIDTH ,
	_tcslen(RULE_HEADER[1]) * CHAR_WIDTH - 15,
	_tcslen(RULE_HEADER[2]) * CHAR_WIDTH + 30,
	_tcslen(RULE_HEADER[3]) * CHAR_WIDTH + 30,
	_tcslen(RULE_HEADER[4]) * CHAR_WIDTH - 20,
	_tcslen(RULE_HEADER[5]) * CHAR_WIDTH + 50,
};
#define RULE_HEADER_COUNT sizeof(RULE_HEADER)/sizeof(TCHAR*)

#define GUI_ACL_ACTION_0					_T("拦截")
#define GUI_ACL_ACTION_1					_T("放行")
#define GUI_ACL_DERECTION_2					_T("双向")
#define GUI_ACL_DERECTION_0					_T("连入")
#define GUI_ACL_DERECTION_1					_T("连出")
#define GUI_ACL_SERVICE_TYPE_3				_T("任何")
#define GUI_ACL_SERVICE_TYPE_0				_T("TCP")
#define GUI_ACL_SERVICE_TYPE_1				_T("UDP")
#define GUI_ACL_SERVICE_TYPE_2				_T("ICMP")
#define GUI_ACL_SERVICE_TYPE_4				_T("NOTIP")

static TCHAR *GUI_ACTION[]		= {
	GUI_ACL_ACTION_0,
	GUI_ACL_ACTION_1,
};
#define GUI_ACTION_COUNT	sizeof(GUI_ACTION)/sizeof(TCHAR*)

static TCHAR *GUI_DIRECTION[]	= {
	GUI_ACL_DERECTION_0,
	GUI_ACL_DERECTION_1,
//	GUI_ACL_DERECTION_2,
};
#define GUI_DIRECTION_COUNT	  sizeof(GUI_DIRECTION)/sizeof(TCHAR*)

static TCHAR *GUI_SERVICE_TYPE[]= {
	 GUI_ACL_SERVICE_TYPE_0,	
	 GUI_ACL_SERVICE_TYPE_1,						
	 GUI_ACL_SERVICE_TYPE_2,						
//	 GUI_ACL_SERVICE_TYPE_3,
//	 GUI_ACL_SERVICE_TYPE_4,
};
#define GUI_SERVICE_TYPE_COUNT	sizeof(GUI_SERVICE_TYPE)/sizeof(TCHAR*) 

////////////////////////////////////////////////////////////////////////

#define DT_IN		0x01
#define DT_OUT		0x02
#define DT_IN_OUT	0x03   //暂时未允许此功能

//Filter status 
#define FWS_DENYALL		0x00		//Deny all packets
#define FWS_ALLOWALL	0x01		//Allow all packets
#define FWS_NORMAL		0x02		//Apply rules

//Mode规则策略模式
#define RM_DENY		0x01		//除了允许都禁止
#define RM_ALLOW	0x00		//除了禁止都允许

//协议类型定义
#define PT_TCP		0x00
#define PT_UDP		0x01
#define PT_ICMP		0x02
#define PT_ALLTYPE  0x03    //暂时未允许此功能

//define action
#define DENY	0x00
#define ALLOW	0x01
#define NOT_MATCH	0x02



#endif // CONDEF_H