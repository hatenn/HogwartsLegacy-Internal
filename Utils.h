//
// FindPattern.h
//
#pragma once

#include <Windows.h>
#include <psapi.h>
#include <vector>

#pragma once

namespace Utils {

	//DMA == Find Dinamic Memory Allocation Adress
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets) {

		uintptr_t endereco = ptr;

		for (unsigned int i = 0; i < offsets.size(); i++) {
			endereco = *(uintptr_t*)endereco;
			endereco += offsets[i];
		}
		return endereco;
	}

	namespace Pattern {

		bool DataCompare(PBYTE pData, PBYTE bSig, char* szMask) {
			for (; *szMask; ++szMask, ++pData, ++bSig) {
				if (*szMask == 'x' && *pData != *bSig)
					return false;
			}
			return (*szMask) == 0;
		}

		PBYTE FindPattern(PBYTE dwAddress, DWORD dwSize, PBYTE pbSig, char* szMask, long offset) {
			size_t length = strlen(szMask);
			for (size_t i = NULL; i < dwSize - length; i++) {
				if (DataCompare(dwAddress + i, pbSig, szMask))
					return dwAddress + i + offset;
			}
			return 0;
		}
	}

	namespace Variables {

		unsigned __int64 ImageBase;
		HMODULE ImageModuleHandle;
		MODULEINFO ImageModuleInfo;
	}
}