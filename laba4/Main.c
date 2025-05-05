#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <conio.h>

size_t arrSize = 0;
size_t threadsAmnt = 0;
int* common_array;
HANDLE hStartEvent;
HANDLE hContinueEvent;
HANDLE* stopEventsArr;
BOOL* terminateEventsArr;
CRITICAL_SECTION common_arrayCS;


DWORD WINAPI Marker(LPVOID number);
DWORD CloseThreads(HANDLE* thread_array, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		DWORD result = CloseHandle(thread_array[i]);
		if (NULL == result) return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
DWORD CheckError(DWORD valuable)
{
	if (NULL == valuable)
	{
		printf("Error occupied during threads start, error code: %u", valuable);
		system("pause");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main()
{
	printf("Enter array size: ");
	scanf("%ud", &arrSize);

	common_array = (int*)malloc(sizeof(int) * arrSize);
	memset(common_array, 0, sizeof(int) * arrSize);

	printf("Enter amount of threads: ");
	scanf("%ud", &threadsAmnt);

	HANDLE* threadsArr = (HANDLE*)malloc(sizeof(HANDLE) * threadsAmnt);
	stopEventsArr = (HANDLE*)malloc(sizeof(HANDLE) * threadsAmnt);
	terminateEventsArr = (BOOL*)malloc(sizeof(BOOL) * threadsAmnt);
	memset(terminateEventsArr, FALSE, sizeof(BOOL) * threadsAmnt);

	hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (CheckError(hStartEvent)) return EXIT_FAILURE;
	hContinueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (CheckError(hContinueEvent)) return EXIT_FAILURE;

	InitializeCriticalSection(&common_arrayCS);

	for (size_t i = 0; i < threadsAmnt; i++)
	{
		stopEventsArr[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (CheckError(stopEventsArr[i])) return EXIT_FAILURE;
		threadsArr[i] = CreateThread(NULL, 0, Marker, (LPVOID)i, 0, NULL);
	}

	SetEvent(hStartEvent);

	int currThrdsAmnt = threadsAmnt;
	while (-1 != currThrdsAmnt)
	{
		WaitForMultipleObjects(threadsAmnt, stopEventsArr, TRUE, 450);
		printf("\nThreads stop working...\n");

		for (size_t i = 0; i < arrSize; i++) printf("%u ", common_array[i]);

		if (0 == currThrdsAmnt) {
			printf("\nNo threads are available....\n");
			break;
		}

		size_t currNum;
		while (TRUE) {
			printf("\nPlease, enter thread num to terminate : ");
			
			scanf("%u", &currNum);
			if (terminateEventsArr[currNum - 1] == FALSE) {
				break;
			}
		}
		terminateEventsArr[currNum - 1] = TRUE;
		PulseEvent(hContinueEvent);
		--currThrdsAmnt;
	}
	DeleteCriticalSection(&common_arrayCS);
	CloseThreads(threadsArr, threadsAmnt, CloseHandle);
	CloseThreads(stopEventsArr, threadsAmnt, CloseHandle);

	free(threadsArr);
	free(terminateEventsArr);
	free(common_array);
	free(stopEventsArr);

	CloseHandle(hStartEvent);
	CloseHandle(hContinueEvent);
	return EXIT_SUCCESS;
}
