
/**************************************************************
				     ����ṹ
***************************************************************/

struct	Fw_Global {
	char			fw_status;			    //����ǽ״̬
	char			log_enable;				//�Ƿ��������־
	char			fw_mode;				//���˽�ֹ�Ķ�������������Ķ���ֹ
};

struct	Fw_Rule_Node  {
	ULONG			id;				//Rule�ĺ���
	int				time;			//ʱ��
	UCHAR			protocol;       //Э������  
	ULONG			s_ip;			//ԭIP
	ULONG			s_mask;		//����
	unsigned short 	s_port;			//ԭ�˿�
	ULONG			d_ip;			//Ŀ��IP
	ULONG			d_mask;		//����
	unsigned short	d_port;			//Ŀ�Ķ˿�
	char			direct;			//����
	char 			action;        //�Ƿ�����
	char		    sMemo[100];//��ע	
	struct	Fw_Rule_Node	*next;			//ָ���¸��ڵ��ָ��
};

struct	Fw_Rule {
	char					enable;			//�Ƿ�����Э��
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




