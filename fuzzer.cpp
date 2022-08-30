#include <stdlib.h>
#include <memory.h>

#include "rng.h"
#include "fuzzer.h"

char*
FuzzAddress(	char* pAddrToFuzz,
	unsigned long	FuzzingLength
)
{
	unsigned char	bRand;
	unsigned short	wRand;
	unsigned long	dwRand;
	
	char*   pMutateAddr;
    
	unsigned long dwBufferDelta = 4;
    
	if (!FuzzingLength)
		return 0;

	dwBufferDelta = genrand_int32(0, FuzzingLength - 1);
    
    pMutateAddr = pAddrToFuzz + dwBufferDelta;
    
	unsigned char bAction = genrand_int8(0, FUZZING_SIZE);
    
    /*
    DbgPrintEx( DPFLTR_DEFAULT_ID, 
                DPFLTR_ERROR_LEVEL,
                "Action = %d\n",bAction);
    */
	switch (bAction)
	{
		case FUZZ_BYTE:
		{
			bRand = genrand_int8(0,0xff);

			*((unsigned char*)pMutateAddr) = bRand;

			break;
		}
			
		case FUZZ_WORD:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(unsigned short))
            {
                pMutateAddr = 0;
                break;
            }

			wRand = genrand_int16(0,0xffff);

			*((unsigned short*)pMutateAddr) = wRand;

			break;
		}
			
		case FUZZ_DWORD:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(unsigned long))
            {
                pMutateAddr = 0;
                break;
            }
                
			dwRand = genrand_int32(0,0xfffffffe);

			*((unsigned long*)pMutateAddr) = dwRand;

			break;
		}
		case FUZZ_INCREMENT:
		{
			(*(unsigned char*)pMutateAddr)++;

			break;
		}
		case FUZZ_DECREMENT:
		{
			(*(unsigned char*)pMutateAddr)--;

			break;
		}
		case FUZZ_SETFLAG:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(unsigned long))
            {
                pMutateAddr = 0;
                break;
            }
                
			dwRand = genrand_int32(0,31);

			(*(unsigned long*)pMutateAddr) = SET_BIT(dwRand,(*(unsigned long*)pMutateAddr));
			
			break;
		}
		case FUZZ_REMOVEFLAG:
        {
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(unsigned long))
            {
                pMutateAddr = 0;
                break;
            }
                
			dwRand = genrand_int32(0,31);

			(*(unsigned long*)pMutateAddr) = CLEAR_BIT(dwRand,(*(unsigned long*)pMutateAddr));
			
			break;
		}
		case FUZZ_DEREFERENCE:
		{
			break;
		}
		case FUZZ_PREDEFINED:
		{
            if((FuzzingLength - (pMutateAddr - pAddrToFuzz)) < sizeof(unsigned long))
            {
                pMutateAddr = 0;
                break;
            }
                
			bRand = genrand_int8(0,sizeof(m_DwordFuzzingConstants)/sizeof(((unsigned long*)m_DwordFuzzingConstants)[0])) - 1;

			dwRand = ((unsigned long*)m_DwordFuzzingConstants)[bRand];
			*((unsigned long*)pMutateAddr) = dwRand;

			break;
		}
		default:
		{
			break;
		}
	}

	return pMutateAddr;
}

unsigned char*
GenerateRandBuffer(
	unsigned long	dwSize
)
{
	unsigned char* pBuffer = (unsigned char*)AllocateBuffer(dwSize);

	if (!pBuffer)
		return 0;

	for (size_t i = 0; i < dwSize; i++)
		((unsigned char*)pBuffer)[i] = genrand_int8(0x0, 0xff);

	return pBuffer;
}

unsigned char*
GenerateRandDigitStr(
	unsigned long dwSize
)
{
	unsigned char* pBuffer = (unsigned char*)AllocateBuffer(dwSize);

	if (!pBuffer)
		return NULL;

	for (size_t i = 0; i < dwSize; i++)
		((unsigned char*)pBuffer)[i] = (unsigned char)(digit[genrand_int32(0x0, _countof(digit) - 1)]);

	return pBuffer;
}

unsigned char*
GenerateRandStr(
	unsigned long dwSize
)
{
	unsigned char* pBuffer = (unsigned char*)AllocateBuffer(dwSize + sizeof(char));

	if (!pBuffer)
		return NULL;

	memset(pBuffer, 0x0, dwSize + sizeof(char));

	for (size_t i = 0; i < dwSize; i++)
		((unsigned char*)pBuffer)[i] = (unsigned char)(letters[genrand_int32(0x0, _countof(letters) - 1)]);

	return pBuffer;
}

unsigned char*
GenerateRandDigitWStr(
	unsigned long dwSize
)
{
	unsigned char* pBuffer = (unsigned char*)AllocateBuffer(dwSize * 2 + sizeof(wchar_t));

	if (!pBuffer)
		return NULL;

	memset(pBuffer, 0x0, dwSize * 2 + sizeof(wchar_t));

	for (size_t i = 0; i < dwSize * 2; i += 2)
		((unsigned char*)pBuffer)[i] = (unsigned char)(digit[genrand_int32(0x0, _countof(digit) - 1)]);

	return pBuffer;
}

unsigned char*
GenerateRandWStr(
	unsigned long dwSize
)
{
	unsigned char* pBuffer = (unsigned char*)AllocateBuffer(dwSize*2 + sizeof(wchar_t));

	if (!pBuffer)
		return NULL;

	memset(pBuffer, 0x0, dwSize*2 + sizeof(wchar_t));

	for (size_t i = 0; i < dwSize*2; i += 2)
		((unsigned char*)pBuffer)[i] = (unsigned char)(letters[genrand_int32(0x0, _countof(letters) - 1)]);

	return pBuffer;
}


void
FreeRandBuffer(unsigned char* pBuffer)
{
	if (pBuffer)
		FreeBuffer(pBuffer);
}

unsigned long
ConcatBuffer(
	unsigned char** pDstBuf,
	unsigned char* pSrcBuf,
	unsigned long	dwDstBufSize,
	unsigned long	dwSrcBufSize
)
{
	*pDstBuf = (unsigned char*)realloc(*pDstBuf, dwSrcBufSize + dwDstBufSize);
	memcpy(*pDstBuf + dwDstBufSize, pSrcBuf, dwSrcBufSize);

	return
		dwSrcBufSize + dwDstBufSize;
};