/* cons_def.h	
	writing by qsc @ 2001-06-14
	for GUI develop in Visual C++
	constants for develop GUI applications. It is used for setting firewall_module rules_setting
*/
#define DT_IN		0x00
#define DT_OUT		0x01
#define DT_IN_OUT	0x02 //��ʱδ����˹���

//Filter status 
#define FWS_DENYALL	 0x00		//Deny all packets
#define FWS_ALLOWALL 0x01		//Allow all packets
#define FWS_NORMAL	 0x02		//Apply rules

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

//define operator
#define OP_GT 0x00
#define OP_LT 0x10
#define OP_EQ 0x20
#define OP_NE 0x30
#define OP_BW 0x40


