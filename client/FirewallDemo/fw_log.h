/**************************************************************
				    ��־�ṹ����   add by xiaoqiang
***************************************************************/
#include "DDKLBLInc.h"
#ifndef fw_log
#define fw_log

#pragma pack(1)

struct Log {
	u_long			ID;				//Rule�ĺ���
	CTime			time;			//ʱ��
	u_char			protocol;       //Э������  
	u_long			s_ip;			//ԭIP
	u_short			s_port;			//ԭ�˿�
	u_long			d_ip;			//Ŀ��IP
	u_short			d_port;			//Ŀ�Ķ˿�
	u_char			direct;			//����
	u_char 			action;        //�Ƿ�����
	char		    sMemo[100];//��ע
};

#pragma pack()
#endif //fw_log