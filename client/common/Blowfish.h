
//Blowfish.h

#ifndef __BLOWFISH_H__
#define __BLOWFISH_H__

//Block Structure
struct SBlock
{
	//Constructors
	SBlock(unsigned int l=0, unsigned int r=0) : m_uil(l), m_uir(r) {}
	//Copy Constructor
	SBlock(const SBlock& roBlock) : m_uil(roBlock.m_uil), m_uir(roBlock.m_uir) {}
	SBlock& operator^=(SBlock& b) { m_uil ^= b.m_uil; m_uir ^= b.m_uir; return *this; }
	unsigned int m_uil, m_uir;
};

class CBlowFish
{
private:
	enum { DATA_LEN=384, BUFF_LEN=1024 };
	enum { BLOCK_SIZE=8 };
	enum { KEY_LENGTH=56 };
	//Operation Modes
	//The Electronic Code Book (ECB), Cipher Block Chaining (CBC) and Cipher Feedback Block (CFB) modes
	//are implemented.
	//In ECB mode if the same block is encrypted twice with the same key, the resulting
	//ciphertext blocks are the same.
	//In CBC Mode a ciphertext block is obtained by first xoring the
	//plaintext block with the previous ciphertext block, and encrypting the resulting value.
	//In CFB mode a ciphertext block is obtained by encrypting the previous ciphertext block
	//and xoring the resulting value with the plaintext.
	enum { ECB=0, CBC=1, CFB=2 };

	//Padding Modes
	//ZEROES - The padding string consists of bytes set to zero.
	//BLANKS - The padding string consists of bytes set to blank.
	//PKCS7 - The Public-Key Cryptography Standards version 7 (PKCS7) padding string
	//consists of a sequence of bytes, each of which is equal to the total number of
	//padding bytes added. For example, if 24 bits (3 bytes) of padding need to be
	//added, the padding string is "03 03 03".
	enum { ZEROES=0, BLANKS=1, PKCS7=2 };

public:
	//CONSTRUCTOR
	CBlowFish();
	//Initialize the P and S boxes for a given Key
	bool Initialize(char const* szKey, int szKeyLen,int nMode=ECB, int nPadding=ZEROES);
	//Encryption for a string of chars
	bool Encrypt(char const* szIn, char* szOut, UINT uiLen);
	//Decryption for a string of chars
	bool Decrypt(char const* szIn, char* szOut, UINT uiLen);

//Private Functions
private:
	inline BYTE Uint2Byte(unsigned int ui);
	inline void BlockToBytes(SBlock const& b, unsigned char* p);
	unsigned int F(unsigned int ui);
	void Encrypt(SBlock&);
	void Decrypt(SBlock&);
	void Char2Hex(unsigned char ch, char* szHex);
	bool Hex2Char(char const* szHex, unsigned char& rch);
	void Binary2Hex(unsigned char const* pucBinStr, int iBinSize, char* pszHexStr);
	bool Hex2Binary(char const* pszHexStr, unsigned char* pucBinStr, int iBinSize);
	int Pad(char* in, int iLength);

	bool m_bInit;
	int	m_nBlockSize;
	int m_nKeyLength;
	int m_nMode;
	int m_nPadding;
	char m_szKey[KEY_LENGTH+1];

	//The Initialization Vector, by default {0, 0}
	SBlock m_oChain0;
	SBlock m_oChain;
	unsigned int m_auiP[18];
	unsigned int m_auiS[4][256];
	static const unsigned int scm_auiInitP[18];
	static const unsigned int scm_auiInitS[4][256];
};


#endif // __BLOWFISH_H__

