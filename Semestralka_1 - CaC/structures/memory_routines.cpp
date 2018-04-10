#include "memory_routines.h"

using namespace std;

namespace structures {
	byte & MemoryRoutines::byteSet(byte & B)
	{
		B = MAX_BYTE;
		return B;
	}
	byte & MemoryRoutines::byteReset(byte & B)
	{
		B = 0;
		return B;
	}
	byte & MemoryRoutines::byteXOR(byte & B)
	{
		
		return B ^= B;
	}
	byte & MemoryRoutines::byteSHL(byte & B)
	{
		B = B << 1;
		return B;
	}
	byte & MemoryRoutines::byteSHR(byte & B)
	{
		B = B >> 1;
		return B;
	}
	bool MemoryRoutines::byteNthBitGet(byte & B, int n)
	{
		return (B >> n) & 1;
	}

	byte & MemoryRoutines::byteNthBitTo0(byte & B, int n)
	{
		B = (~(1 << n)) & B;
		return B;
	}

	byte & MemoryRoutines::byteNthBitTo1(byte & B, int n)
	{
		B = (1 << n) | B;
		return B;
	}

	byte & MemoryRoutines::byteNthBitToggle(byte & B, int n)
	{
		B = B ^ (1 << n);
		return B;
	}

	int MemoryRoutines::bitIndexOfBitMask(const byte * bytes, int bytesCount, byte bitMask)
	{
		for (int i = 0; i < bytesCount; i++) {
			cout << bitset< 8 >(bytes[i]) << ' ';
			cout << "\n";
		}
		//looking for mask in array
		uint8_t testNumber = bytes[0];
		uint8_t nextNumber = bytes[1];
		for (int i = 0; i < bytesCount; i++) {
			//test the first number without shifting
			cout << "Comparing whole number" << "\n";
			cout << bitset< 8 >(testNumber) << ' ';
			cout << "\n";
			cout << bitset< 8 >(bitMask) << ' ';
			cout << "\n";
			cout << "--------";
			cout << "\n";
			if (testNumber == bitMask) {
				cout << "found a match";
				cout << "\n";
				return (((i + 1) * 8) - 8);
			}
			//check if there is next element in array
			if (i < bytesCount - 1) {
				nextNumber = bytes[i + 1];
				for (int j = 0; j < 7; j++) {
					//shift the tested number to left
					testNumber = testNumber << 1;
					//get the first bit from next element
					if (nextNumber & 0x80) {
						//first bit is 1
						testNumber = testNumber + 1;
					}
					else {
						//first bit is 0
						testNumber = testNumber + 0;
					}
					//shift the next number to left
					nextNumber = nextNumber << 1;
					//write the result and compare it
					cout << "Comparing number" << "\n";
					cout << bitset< 8 >(testNumber) << ' ';
					cout << "\n";
					cout << bitset< 8 >(bitMask) << ' ';
					cout << "\n";
					if (testNumber == bitMask) {
						cout << "found a match" << endl;
						cout <<  " " << j;
						return ((((i + 1) * 8) + j + 1 ) - 8);
					}
				}
				//get new testNumber
				//shift the tested number to left
				testNumber = testNumber << 1;
				//get the first bit from next element
				if (nextNumber & 0x80) {
					//first bit is 1
					testNumber = testNumber + 1;
				}
				else {
					//first bit is 0
					testNumber = testNumber + 0;
				}
			}
		}


		return -1;
	}

	MemoryRoutines::MemoryRoutines()
	{
	}

}
