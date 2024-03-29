// Runlength.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "Runlength.h"
#include <sstream>


void CountcharRow(const unsigned char * in, unsigned int length, int * table[256]) {
	const unsigned char * input = in;
	unsigned char current_input = *input;
	const unsigned char * const ending = input + length;
	int a = 0;

	do
	{
		current_input = *input;
		char val = *input;

		int loop_val = val;
		int rlecount = 0;
		do
		{
			input++;
			rlecount++;
			val = *input;
		} while (input < ending && val == current_input);


		a = (int)table[current_input];

		a += rlecount - 2;

		table[current_input] = (int *)a;


	} while (input < ending);
}

unsigned int Charprob(const unsigned char * in, unsigned int length, unsigned char * out, unsigned char * prob_table) {
	int * table[256];
	unsigned char result[256];
	memset(table, 0, sizeof(table));
	memset(result, 0, sizeof(result));
	int count = 0;

	CountcharRow(in, length, table);
	for (int i = 0; i < 256; i++) {
		if ((int)table[i] > 1) {
			result[count] = i;
			count++;
		}

	}
	*out = count;
	out++;
	for (int i = 0; i < count; i++) {
		*out = result[i];
		out++;
		prob_table[i] = result[i];
	}


	return count;

}

bool contains(unsigned char * in, const unsigned int length, const unsigned int element) {
	for (unsigned int i = 0; i < length; i++) {
		if (in[i] == element) {
			return true;
		}
	}
	return false;
}

DllExport unsigned int Encode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length){
	// pointer to last input
	const unsigned char * const ending = in + length;
	//pionter to out begin
	unsigned char * const count = out;
	//store current value
	unsigned char current_val = *in;

	//table to store values for RLE
	unsigned char prob_table[100];
	memset(prob_table, 0, sizeof(prob_table));

	//Get vaules to Encode
	int size = Charprob(in, length, out, prob_table);		
	out += size + 1;

	//std::stringstream stream;
	//stream << "Size Encode : " << size;
	//std::string result(stream.str());
	//MessageBoxA(NULL, result.c_str(), "Vuong-DCP", MB_ICONINFORMATION | MB_OK);

	do {
		// getting input data
		unsigned char val = *in;
		current_val = *in;

		// Check if the vaules is on the List
		bool exist = contains(prob_table, size, val);
		if (exist)
		{
			unsigned char loop_val = val;
			unsigned char rlecount = 0;
			// Count Row
			do
			{
				in++;
				rlecount++;
				val = *in;

			} while (in < ending && val == current_val && rlecount < 255);

			// Save value and count
			out[0] = current_val;
			out[1] = rlecount;
			out += 2;
		}
		else {
			*out = val;
			out++;
			in++; 
		}

	} while (in<ending);
	return (unsigned int)(out - count)+2;
}

DllExport void Decode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length)
{
	const unsigned char * const ending = out + length;
	int size = *in;

	//std::stringstream stream;
	//stream << "Size Decode : " << size;
	//std::string result(stream.str());
	//MessageBoxA(NULL, result.c_str(), "Vuong-DCP", MB_ICONINFORMATION | MB_OK);

	in++;
	unsigned char prob_table[100];
	memset(prob_table, 0, sizeof(prob_table));
	for (int i = 0; i < size; i++) {
		prob_table[i] = *in;
		in++;
	}

	unsigned char val = *in;

	in--;
	do {
		in++;
		val = *in;
		bool exist = contains(prob_table, size, val);
		if (exist)
		{
			in++;
			int count = *in;
			for (int i = 0; i < count && out<ending; i++) {
				*out = val;
				out++;
				//i += val;
			}
		}
		else {
			*out = val;
			out++;
		}
		//in++;

	} while (out<ending);
	//MessageBoxA(NULL, "RLE-Done", "Vuong-DCP", MB_ICONINFORMATION | MB_OK);
}
