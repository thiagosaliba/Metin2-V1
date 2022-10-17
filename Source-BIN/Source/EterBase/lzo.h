#ifndef __INC_ETER_BASE_LZO_H__
#define __INC_ETER_BASE_LZO_H__

#include <windows.h>
#include <lzo/lzo1x.h>
#include "Singleton.h"

class CLZObject
{
public:
#pragma pack(4)
	typedef struct SHeader
	{
		DWORD dwFourCC;
		DWORD dwEncryptSize; // ��ȣȭ�� ũ��
		DWORD dwCompressedSize; // ����� ������ ũ��
		DWORD dwRealSize; // ���� ������ ũ��
	} THeader;
#pragma pack()

	CLZObject();
	~CLZObject();

	void Clear();

	void BeginCompress(const void* pvIn, UINT uiInLen);
	void BeginCompressInBuffer(const void* pvIn, UINT uiInLen, void* pvOut);
	bool Compress();

	bool BeginDecompress(const void* pvIn);
	bool Decompress(uint16_t uiKeySet);

	bool Encrypt(uint16_t uiKeySet);
	bool __Decrypt(uint16_t uiKeySet, BYTE* pbData);

	const THeader& GetHeader() { return *m_pHeader; }
	BYTE* GetBuffer() { return m_pbBuffer; }
	DWORD GetSize();
	void AllocBuffer(DWORD dwSize);
	DWORD GetBufferSize() { return m_dwBufferSize; }
	//void CopyBuffer(const char* pbSrc, DWORD dwSrcSize);

private:
	void Initialize();

	BYTE* m_pbBuffer;
	DWORD m_dwBufferSize;

	THeader* m_pHeader;
	const BYTE* m_pbIn;
	bool m_bCompressed;

	bool m_bInBuffer;

public:
	static DWORD ms_dwFourCC;
};

class CLZO : public CSingleton<CLZO>
{
public:
	CLZO();
	virtual ~CLZO();

	bool CompressMemory(CLZObject& rObj, const void* pIn, UINT uiInLen);
	bool CompressEncryptedMemory(CLZObject& rObj, const void* pIn, UINT uiInLen, uint16_t uiKeySet);
	bool Decompress(CLZObject& rObj, const BYTE* pbBuf, uint16_t uiKeySet = 0);
	BYTE* GetWorkMemory();

private:
	BYTE* m_pWorkMem;

};

#endif // __INC_ETER_BASE_LZO_H__
