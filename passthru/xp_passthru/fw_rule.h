
/**************************************************************
				     ����ṹ
***************************************************************/
//#include "DDKLBLInc.h"

#pragma pack(1)

struct	Fw_Global {
	char			fw_status;			    //����ǽ״̬
	char			log_disable;				//�Ƿ��������־
	char			fw_mode;				//���˽�ֹ�Ķ�������������Ķ���ֹ
};

struct	Fw_Rule_Node  {
	u_long			id;				//Rule�ĺ���
	u_char			protocol;       //Э������  
	u_long			s_ip;			//ԭIP
	u_long			s_mask;		//����
	u_short			s_port;			//ԭ�˿�
	u_long			d_ip;			//Ŀ��IP
	u_long			d_mask;		//����
	u_short	d_port;			//Ŀ�Ķ˿�
	u_char			direct;			//����
	u_char 			action;        //�Ƿ�����
	char		    sMemo[100];//��ע	
	struct	Fw_Rule_Node	*next;			//ָ���¸��ڵ��ָ��
};

struct	Fw_Rule {
	u_char					disable;			//�Ƿ�����Э��
	struct	Fw_Rule_Node	*pin, *pout;	//ָ���¸�in, out�ڵ��ָ��
};


//����ṹ��
struct	 	Fw_Rules {
	struct		Fw_Global	global; 
	struct		Fw_Rule		tcp;
	struct		Fw_Rule		udp;
	struct		Fw_Rule		icmp;
	struct		Fw_Rule		ip;
};

#pragma pack()
