///////////////////////////////////////////////////////
//        Add by xiaoqiang mar 20 2006
//log��ʾ��һЩdefine����
///////////////////////////////////////////////////////
#ifndef	CONDEF_H
#define CONDEF_H

#define CHAR_WIDTH		10

#define GUI_ACL_EXPLAIN_CONST			_T("'*'��������Ӧ�ó���0�������ж˿ڡ�")
#define GUI_ACL_EXPLAIN_PORT_ALL		_T("����")
#define GUI_ACL_EXPLAIN_OUT_FORMAT		_T("%s%sͨ��%sЭ���%s�˿ڷ���%s��%s�˿ڽ���%s��%s")
#define GUI_ACL_EXPLAIN_IN_FORMAT		_T("%s%sͨ��%sЭ���%s�˿��򱾻�%s%s�˿ڷ������������󽫱�%s��%s")
#define GUI_QUERY_EXPLAIN_APP_OUT		_T("%s %s ͨ�� %s Э��� %u �˿ڷ��� %s �� %u �˿ڣ��Ƿ���У�")
#define GUI_QUERY_EXPLAIN_APP_IN		_T("%s %s ͨ�� %s Э��� %u �˿��򱾻� %s %u �˿ڷ������������Ƿ���У�")
#define GUI_QUERY_EXPLAIN_WEB			_T("Ӧ�ó��� %s Ҫ����վ�� %s���Ƿ���У�")

// Popup Message

#define GUI_ACL_MESSAGE_INAVALID_PORT					_T("�˿�ֵ��Ч����Ч��ΧΪ 0 - 65535�����������롣")


static TCHAR *LOG_HEADER[] = {
	_T("����ID"),
	_T("Э��/����"),
	_T("�����IP/�˿�"),
	_T("�ͻ���IP/�˿�"),
	_T("����/����"),
	_T("ʱ��"),
	_T("��ע"),
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
	_T("����"),
	_T("Э��/����"),
	_T("�����IP/�˿�"),
	_T("�ͻ���IP/�˿�"),
	_T("����/����"),
	_T("��ע"),
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

#define GUI_ACL_ACTION_0					_T("����")
#define GUI_ACL_ACTION_1					_T("����")
#define GUI_ACL_DERECTION_2					_T("˫��")
#define GUI_ACL_DERECTION_0					_T("����")
#define GUI_ACL_DERECTION_1					_T("����")
#define GUI_ACL_SERVICE_TYPE_3				_T("�κ�")
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
#define DT_IN_OUT	0x03   //��ʱδ����˹���

//Filter status 
#define FWS_DENYALL		0x00		//Deny all packets
#define FWS_ALLOWALL	0x01		//Allow all packets
#define FWS_NORMAL		0x02		//Apply rules

//Mode�������ģʽ
#define RM_DENY		0x01		//����������ֹ
#define RM_ALLOW	0x00		//���˽�ֹ������

//Э�����Ͷ���
#define PT_TCP		0x00
#define PT_UDP		0x01
#define PT_ICMP		0x02
#define PT_ALLTYPE  0x03    //��ʱδ����˹���

//define action
#define DENY	0x00
#define ALLOW	0x01
#define NOT_MATCH	0x02



#endif // CONDEF_H