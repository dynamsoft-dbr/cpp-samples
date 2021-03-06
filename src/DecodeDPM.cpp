// BarcodeReaderDemo.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <conio.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include "DynamsoftBarcodeReader.h"
using namespace std;

void ToHexString(unsigned char* pSrc, int iLen, char* pDest)
{
	const char HEXCHARS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	int i;
	char* ptr = pDest;

	for (i = 0; i < iLen; ++i)
	{
#if defined(_WIN32) || defined(_WIN64)
		sprintf_s(ptr, 4, "%c%c ", HEXCHARS[(pSrc[i] & 0xF0) >> 4], HEXCHARS[(pSrc[i] & 0x0F) >> 0]);
#else
		snprintf(ptr, 4, "%c%c ", HEXCHARS[(pSrc[i] & 0xF0) >> 4], HEXCHARS[(pSrc[i] & 0x0F) >> 0]);
#endif
		ptr += 3;
	}
}

void OutputResult(CBarcodeReader& reader, int errorcode, float time)
{
	char * pszTemp = NULL;
	char * pszTemp1 = NULL;
	char * pszTemp2 = NULL;
	int iRet = errorcode;
	pszTemp = (char*)malloc(4096);
	if (iRet != DBR_OK && iRet != DBRERR_MAXICODE_LICENSE_INVALID && iRet != DBRERR_AZTEC_LICENSE_INVALID && iRet != DBRERR_LICENSE_EXPIRED && iRet != DBRERR_QR_LICENSE_INVALID && iRet != DBRERR_GS1_COMPOSITE_LICENSE_INVALID &&
		iRet != DBRERR_1D_LICENSE_INVALID && iRet != DBRERR_PDF417_LICENSE_INVALID && iRet != DBRERR_DATAMATRIX_LICENSE_INVALID && iRet != DBRERR_GS1_DATABAR_LICENSE_INVALID && iRet != DBRERR_PATCHCODE_LICENSE_INVALID &&
		iRet != DBRERR_POSTALCODE_LICENSE_INVALID && iRet != DBRERR_DOTCODE_LICENSE_INVALID && iRet != DBRERR_DPM_LICENSE_INVALID && iRet != DBRERR_IRT_LICENSE_INVALID)
	{
#if defined(_WIN32) || defined(_WIN64)
		sprintf_s(pszTemp, 4096, "Failed to read barcode: %s\r\n", DBR_GetErrorString(iRet));
#else
		snprintf(pszTemp, 4096, "Failed to read barcode: %s\r\n", DBR_GetErrorString(iRet));
#endif
		printf(pszTemp);
		free(pszTemp);
		return;
	}

	TextResultArray *paryResult = NULL;
	reader.GetAllTextResults(&paryResult);

	if (paryResult->resultsCount == 0)
	{
#if defined(_WIN32) || defined(_WIN64)
		sprintf_s(pszTemp, 4096, "No barcode found. Total time spent: %.3f seconds.\r\n", time);
#else
		snprintf(pszTemp, 4096, "No barcode found. Total time spent: %.3f seconds.\r\n", time);
#endif
		free(pszTemp);
		CBarcodeReader::FreeTextResults(&paryResult);
		return;
	}

#if defined(_WIN32) || defined(_WIN64)
	sprintf_s(pszTemp, 4096, "Total barcode(s) found: %d. Total time spent: %.3f seconds\r\n\r\n", paryResult->resultsCount, time);
#else
	snprintf(pszTemp, 4096, "Total barcode(s) found: %d. Total time spent: %.3f seconds\r\n\r\n", paryResult->resultsCount, time);
#endif
	printf(pszTemp);
	for (int iIndex = 0; iIndex < paryResult->resultsCount; iIndex++)
	{
#if defined(_WIN32) || defined(_WIN64)
		sprintf_s(pszTemp, 4096, "Barcode %d:\r\n", iIndex + 1);
#else
		snprintf(pszTemp, 4096, "Barcode %d:\r\n", iIndex + 1);
#endif
		printf(pszTemp);
		if (paryResult->results[iIndex]->barcodeFormat != 0)
		{
#if defined(_WIN32) || defined(_WIN64)
			sprintf_s(pszTemp, 4096, "    Type: %s\r\n", paryResult->results[iIndex]->barcodeFormatString);
#else
			snprintf(pszTemp, 4096, "    Type: %s\r\n", paryResult->results[iIndex]->barcodeFormatString);
#endif	
		}
		else
		{
#if defined(_WIN32) || defined(_WIN64)
			sprintf_s(pszTemp, 4096, "    Type: %s\r\n", paryResult->results[iIndex]->barcodeFormatString_2);
#else
			snprintf(pszTemp, 4096, "    Type: %s\r\n", paryResult->results[iIndex]->barcodeFormatString_2);
#endif
		}
		printf(pszTemp);
#if defined(_WIN32) || defined(_WIN64)
		sprintf_s(pszTemp, 4096, "    Value: %s\r\n", paryResult->results[iIndex]->barcodeText);
#else
		snprintf(pszTemp, 4096, "    Value: %s\r\n", paryResult->results[iIndex]->barcodeText);
#endif
		printf(pszTemp);

		pszTemp1 = (char*)malloc(paryResult->results[iIndex]->barcodeBytesLength * 3 + 1);
		pszTemp2 = (char*)malloc(paryResult->results[iIndex]->barcodeBytesLength * 3 + 100);
		ToHexString(paryResult->results[iIndex]->barcodeBytes, paryResult->results[iIndex]->barcodeBytesLength, pszTemp1);
#if defined(_WIN32) || defined(_WIN64)
		sprintf_s(pszTemp2, paryResult->results[iIndex]->barcodeBytesLength * 3 + 100, "    Hex Data: %s\r\n", pszTemp1);
#else
		snprintf(pszTemp2, paryResult->results[iIndex]->barcodeBytesLength * 3 + 100, "    Hex Data: %s\r\n", pszTemp1);
#endif
		printf(pszTemp2);
		free(pszTemp1);
		free(pszTemp2);
	}

	free(pszTemp);
	CBarcodeReader::FreeTextResults(&paryResult);
}

bool GetExeDirectory(string &strPath)
{
	char path[512] = { 0 };
#if defined(_WIN32) || defined(_WIN64)
	GetModuleFileName(NULL, path, sizeof(path));
	strPath = path;

	size_t pos = strPath.find_last_of('\\');
	strPath = strPath.substr(0, pos);
	return true;
#elif defined(__linux__)
	readlink("/proc/self/exe", path, sizeof(path));
	strPath = path;

	size_t pos = strPath.find_last_of('\\');
	strPath = strPath.substr(0, pos);
	return true;
#elif defined(__APPLE__) && defined(TARGET_OS_MAC)
#endif
	return false;
}

int main(int argc, const char* argv[])
{
	int iRet = -1;
#if defined(_WIN32) || defined(_WIN64)
	unsigned _int64 ullTimeBegin = 0;
	unsigned _int64 ullTimeEnd = 0;
#else
	struct timeval ullTimeBegin, ullTimeEnd;
#endif
	size_t iLen = 0;
	char szErrorMsg[256];
	PublicRuntimeSettings runtimeSettings;

	printf("*************************************************\r\n");
	printf("Welcome to Dynamsoft Barcode Reader Demo\r\n");
	printf("*************************************************\r\n");
	printf("This Sample.\r\n");

	CBarcodeReader reader;
	reader.InitLicense("Put your license key here");

	string file;
	GetExeDirectory(file);
	size_t pos = file.find_last_of('/');
	file = file.substr(0, pos + 1) + "resource/DPM-Sample-1.png";



	//DPM settings with json string
	//iRet = reader.InitRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"DMPSetting\",\"DPMCodeReadingModes\":[\"DPMCRM_GENERAL\"]}}", CM_OVERWRITE, szErrorMsg, 256);

	//DPM settings with runtimeSetting
	reader.GetRuntimeSettings(&runtimeSettings);
	runtimeSettings.barcodeFormatIds = BF_DATAMATRIX;
	runtimeSettings.furtherModes.dpmCodeReadingModes[0] = DPMCRM_GENERAL;
	iRet = reader.UpdateRuntimeSettings(&runtimeSettings, szErrorMsg, 256);
	if (iRet != DBR_OK)
	{
		printf("Error code: %d. Error message: %s\n", iRet, szErrorMsg);
		return -1;
	}

#if defined(_WIN32) || defined(_WIN64)
	// Read barcode
	ullTimeBegin = GetTickCount();
	//iRet = reader.DecodeFile(file.c_str(), "DMPSetting"); //use string setting
	iRet = reader.DecodeFile(file.c_str(), "");
	ullTimeEnd = GetTickCount();

	// Output barcode result
	OutputResult(reader, iRet, (((float)(ullTimeEnd - ullTimeBegin)) / 1000));
#else
	gettimeofday(&ullTimeBegin, NULL);
	iRet = reader.DecodeFile(file.c_str(), "");
	gettimeofday(&ullTimeEnd, NULL);
	float fCostTime = (float)((ullTimeEnd.tv_sec * 1000 * 1000 + ullTimeEnd.tv_usec) - (ullTimeBegin.tv_sec * 1000 * 1000 + ullTimeBegin.tv_usec)) / (1000 * 1000);
	OutputResult(reader, iRet, fCostTime);
#endif


	return 0;
}
