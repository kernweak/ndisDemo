#ifndef Rule
#define Rule

void  UTIL_FreeMemory(void* p);
void*  UTIL_AllocateMemory(unsigned int size);

void CopyPacket2Buffer(IN PNDIS_PACKET pPacket,IN OUT PUCHAR pBuff,IN OUT PUINT pLength);

ULONG UTIL_htonl( ULONG hostlong );
ULONG UTIL_ntohl( ULONG netlong );

USHORT UTIL_htons( USHORT hostshort );
USHORT UTIL_ntohs( USHORT netshort );

ULONG GetCurrentTime(void);

void UTIL_CountPacket (USHORT ptype,char direct,char action,int packsize);

void UTIL_TcpWriteLog(struct Fw_Rule_Node *pTcpRule,struct ip *piphdr,struct tcphdr *ptcphdr,UCHAR Direct);
void UTIL_UdpWriteLog(struct Fw_Rule_Node *pUdpRule,struct ip *piphdr,struct udphdr *pudphdr,UCHAR Direct);
void UTIL_IcmpWriteLog(struct Fw_Rule_Node *pIcmpRule,struct ip *piphdr,struct icmp *picmphdr,UCHAR Direct);
//void UTIL_RestipWriteLog(struct Fw_Rule_Node *pRestipRule,struct ip *piphdr,UCHAR Direct);


BOOLEAN UTIL_SizeupPort(USHORT cur_port,char p_operator,USHORT port1,USHORT port2);

void do_nothing();

UCHAR	UTIL_MatchTcpRule(struct ip *pIpHeader, struct tcphdr *pTcpHeader, UCHAR Direct);
UCHAR	UTIL_MatchUdpRule(struct ip *pIpHeader, struct udphdr *pUdpHeader, UCHAR Direct);
UCHAR	UTIL_MatchIcmpRule(struct ip *pIpHeader, struct icmp *pIcmpHeader, UCHAR Direct);
UCHAR	UTIL_MatchIpRule(struct ip *pIpHeader, UCHAR Direct);

void  UTIL_PASSALL();
void  UTIL_DENYALL();
BOOLEAN UTIL_AddConfig(struct RuleSet *RuleSets);
BOOLEAN UTIL_AddOneRule(struct Rules *prules);
BOOLEAN UTIL_DelAllrules(void);
BOOLEAN UTIL_DelOneRule(struct Rules *prules);

UCHAR UTIL_MatchRule(char *pPacketContent, UCHAR Direct);

#endif