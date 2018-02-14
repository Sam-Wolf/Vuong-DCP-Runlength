#define DllExport   __declspec( dllexport )  

// Encode Function
extern "C" DllExport  unsigned int Encode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length);

// Decode Funktion
extern "C" DllExport void Decode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length);

// Compression Method ID (4 Capital Letter or 3 + Space)
extern "C" DllExport unsigned long  GetCompressDWORD() {
	return 'RUNL';
};

// Compression Method Name 
extern "C"  DllExport const char * GetCompressName() {
	return "Runlength";
};

// returns if a bigger output buffer is needed
extern "C" DllExport bool GetBigBuffer() {
	return 0;
};