#include "stdafx.h"

PBYTE
FuzzAddress(	PBYTE	pAddrToFuzz, 
				DWORD	FuzzingLength
)
{
	BYTE	bRand;
	WORD	wRand;
	DWORD	dwRand;
	
    PBYTE   pMutateAddr;
    
	DWORD dwBufferDelta = 4; 
    
	if (!FuzzingLength)
		return NULL;

	dwBufferDelta = (BYTE)getrand(0, FuzzingLength - 1);
    
    pMutateAddr = pAddrToFuzz + dwBufferDelta;
    
	BYTE bAction = (BYTE)getrand(0, FUZZING_SIZE);
    
    /*
    DbgPrintEx( DPFLTR_DEFAULT_ID, 
                DPFLTR_ERROR_LEVEL,
                "Action = %d\n",bAction);
    */
	switch (bAction)
	{
		case FUZZ_BYTE:
		{
			bRand = (BYTE)getrand(0,0xff);

			*((PBYTE)pMutateAddr) = bRand;

			break;
		}
			
		case FUZZ_WORD:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(WORD))
            {
                pMutateAddr = 0;
                break;
            }

			wRand = (WORD)getrand(0,0xffff);

			*((PWORD)pMutateAddr) = wRand;

			break;
		}
			
		case FUZZ_DWORD:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(DWORD))
            {
                pMutateAddr = 0;
                break;
            }
                
			dwRand = getrand(0,0xfffffffe);

			*((PDWORD)pMutateAddr) = dwRand;

			break;
		}
		case FUZZ_INCREMENT:
		{
			(*(PBYTE)pMutateAddr)++;

			break;
		}
		case FUZZ_DECREMENT:
		{
			(*(PBYTE)pMutateAddr)--;

			break;
		}
		case FUZZ_SETFLAG:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(DWORD))
            {
                pMutateAddr = 0;
                break;
            }
                
			dwRand = getrand(0,31);

			(*(PDWORD)pMutateAddr) = SET_BIT(dwRand,(*(PWORD)pMutateAddr));
			
			break;
		}
		case FUZZ_REMOVEFLAG:
        {
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(DWORD))
            {
                pMutateAddr = 0;
                break;
            }
                
			dwRand = getrand(0,31);

			(*(PDWORD)pMutateAddr) = CLEAR_BIT(dwRand,(*(PWORD)pMutateAddr));
			
			break;
		}
		case FUZZ_DEREFERENCE:
		{
			break;
		}
		case FUZZ_PREDEFINED:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(DWORD))
            {
                pMutateAddr = 0;
                break;
            }
                
			bRand = (BYTE)getrand(0,sizeof(m_DwordFuzzingConstants)/sizeof(((PULONG)m_DwordFuzzingConstants)[0])) - 1;

			dwRand = ((PULONG)m_DwordFuzzingConstants)[bRand];
			*((PDWORD)pMutateAddr) = dwRand;

			break;
		}
		default:
		{
			break;
		}
	}

	return pMutateAddr;
}

PVOID
GenerateRandBuffer(
	DWORD	dwSize
)
{
	PVOID pBuffer = AllocateBuffer(dwSize);

	if (!pBuffer)
		return NULL;

	for (size_t i = 0; i < dwSize; i++)
		((PBYTE)pBuffer)[i] = (BYTE)getrand(0x0, 0xff);

	return pBuffer;
}

PVOID
GenerateRandDigitStr(
	DWORD dwSize
)
{
	PVOID pBuffer = AllocateBuffer(dwSize);

	if (!pBuffer)
		return NULL;

	for (size_t i = 0; i < dwSize; i++)
		((PBYTE)pBuffer)[i] = (BYTE)(digit[getrand(0x0, _countof(digit) - 1)]);

	return pBuffer;
}

PVOID
GenerateRandStr(
	DWORD dwSize
)
{
	PVOID pBuffer = AllocateBuffer(dwSize + sizeof(char));

	if (!pBuffer)
		return NULL;

	memset(pBuffer, 0x0, dwSize + sizeof(char));

	for (size_t i = 0; i < dwSize; i++)
		((PBYTE)pBuffer)[i] = (BYTE)(letters[getrand(0x0, _countof(letters) - 1)]);

	return pBuffer;
}

PVOID
GenerateRandDigitWStr(
	DWORD dwSize
)
{
	PVOID pBuffer = AllocateBuffer(dwSize * 2 + sizeof(wchar_t));

	if (!pBuffer)
		return NULL;

	memset(pBuffer, 0x0, dwSize * 2 + sizeof(wchar_t));

	for (size_t i = 0; i < dwSize * 2; i += 2)
		((PBYTE)pBuffer)[i] = (BYTE)(digit[getrand(0x0, _countof(digit) - 1)]);

	return pBuffer;
}

PVOID
GenerateRandWStr(
	DWORD dwSize
)
{
	PVOID pBuffer = AllocateBuffer(dwSize*2 + sizeof(wchar_t));

	if (!pBuffer)
		return NULL;

	memset(pBuffer, 0x0, dwSize*2 + sizeof(wchar_t));

	for (size_t i = 0; i < dwSize*2; i += 2)
		((PBYTE)pBuffer)[i] = (BYTE)(letters[getrand(0x0, _countof(letters) - 1)]);

	return pBuffer;
}


VOID
FreeRandBuffer(PVOID pBuffer)
{
	if (pBuffer)
		FreeBuffer(pBuffer);
}

DWORD
ConcatBuffer(
	PBYTE* pDstBuf,
	PBYTE	pSrcBuf,
	DWORD	dwDstBufSize,
	DWORD	dwSrcBufSize
)
{
	*pDstBuf = (PBYTE)realloc(*pDstBuf, dwSrcBufSize + dwDstBufSize);
	memcpy(*pDstBuf + dwDstBufSize, pSrcBuf, dwSrcBufSize);

	return
		dwSrcBufSize + dwDstBufSize;
};