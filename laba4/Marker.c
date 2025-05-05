#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <math.h>

extern size_t arrSize;
extern int* common_array;
extern HANDLE hStartEvent;
extern HANDLE hContinueEvent;
extern HANDLE* stopEventsArr;
extern BOOL* terminateEventsArr;
extern CRITICAL_SECTION common_arrayCS;



DWORD WINAPI Marker(LPVOID number)
{
	// Thread initialization
	size_t threadNum = (size_t)number;
	size_t markedItems = 0;
	int* positionsArr = (int*)malloc(sizeof(int)*arrSize);
	memset(positionsArr, -1, sizeof(int) * arrSize);
	srand(threadNum + 1);
	printf("Thread %u created...\n", threadNum + 1);

	// Wait for other threads and order from main thread
	WaitForSingleObject(hStartEvent, INFINITE);

	// Marks common_array elements
	while (INFINITE)
	{
		size_t position = rand() % arrSize;
		EnterCriticalSection(&common_arrayCS);
		if (0 == common_array[position] && markedItems < arrSize)
		{

			Sleep(5);
			common_array[position] = threadNum + 1;
			Sleep(5);
			LeaveCriticalSection(&common_arrayCS);
			positionsArr[position] = 0;
			++markedItems;
		}
		else
		{
			LeaveCriticalSection(&common_arrayCS);

			printf("\nThread num %u\n", threadNum + 1);
			printf("Elements marked %u\n", markedItems);
			printf("Position of unmarkable element %u\n", position);
			// End work and waiting for terminating

			SignalObjectAndWait(stopEventsArr[threadNum], hContinueEvent, INFINITE, FALSE);
			if (terminateEventsArr[threadNum])
			{
				// Threads dies, and makes marked common_array elements initial
				EnterCriticalSection(&common_arrayCS);
				for (size_t i = 0; i < arrSize; i++)
				{
					if (-1 != positionsArr[i]) common_array[i] = 0;
				}
				LeaveCriticalSection(&common_arrayCS);
				printf("\nThread %u is dead...\n", threadNum + 1);
				SetEvent(stopEventsArr[threadNum]);
				break;
			}
		}
	}
	free(positionsArr);
	return EXIT_SUCCESS;
}
