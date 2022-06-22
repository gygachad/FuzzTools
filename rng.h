void init_genrand(unsigned long s);
void init_by_array(unsigned long init_key[], int key_length);
unsigned long genrand(void);

unsigned long long genrand_int64(unsigned long long min, unsigned long long max);
unsigned long genrand_int32(unsigned long min, unsigned long max);
unsigned short genrand_int16(unsigned short min, unsigned short max);
unsigned char genrand_int8(unsigned char min, unsigned char max);
bool genrand_bool();