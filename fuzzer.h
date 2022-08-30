#pragma once

#define AllocateBuffer(dwBytes)				malloc(dwBytes)
#define FreeBuffer(pBuffer)					free(pBuffer)
#define ReallocateBuffer(pBuffer, dwBytes)	realloc(pBuffer, dwBytes)

#define CHOICE(arr) arr[getrand(0,_countof(arr) - 1)]

// constants for dword fuzzing
static unsigned long m_DwordFuzzingConstants[] =
{
    0x00000000,
    0x10000000,
    0x01000000,
    0x00010000,
    0x00001000,
    0xFFFF0000,
	0x0000ffff,
	0xffffffff, 
	0x7fffffff, 
	0x70000000, 
	0xaaaaaaaa, 
	0xcccccccc,
	0xfffffffe, 
	0x7e000000, 
	0x1, 
	0x70000000, 
	0xf0000000, 
	0x000000ff, 
	0x80000000, 
	0x42
};

static const char digit[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '-', 'x'
};

static const char letters[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'\\','/','#','*','%','.',' ','[',']','{','}','|','_','%','%'
};

#define FUZZING_SIZE 12

#define FUZZ_BYTE		1
#define FUZZ_WORD		2
#define FUZZ_DWORD		4
#define FUZZ_INCREMENT	5
#define FUZZ_DECREMENT	6
#define FUZZ_SETFLAG	7
#define FUZZ_REMOVEFLAG	8
#define FUZZ_DEREFERENCE 9
#define FUZZ_PREDEFINED 10

#define  CLEAR_BIT(bitNum,number)(number&(~(0x1<<bitNum)))

#define  SET_BIT(bitNum,number)(number|(0x1<<bitNum))

char*
FuzzAddress(	char*	dwAddrToFuzz, 
				unsigned long	FuzzingLength);

unsigned char*
GenerateRandBuffer(unsigned long	dwSize);

void
FreeRandBuffer(unsigned char* pBuffer);

unsigned char*
GenerateRandDigitStr(
	unsigned long dwSize
);

unsigned char*
GenerateRandStr(
	unsigned long dwSize
);

unsigned char*
GenerateRandDigitWStr(
	unsigned long dwSize
);

unsigned char*
GenerateRandWStr(
	unsigned long dwSize
);

unsigned long
ConcatBuffer(
	char** pDstBuf,
	char*	pSrcBuf,
	unsigned long	dwDstBufSize,
	unsigned long	dwSrcBufSize
);