/*
 * hw12-sorting-hashing.c
 *
 *  Created on: May 22, 2021
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */ // array의 최대 사이즈를 13으로 지정
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE // hash table의 최대 사이즈 역시 array 최대 사이즈와 동일한 13으로 지정

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a); // array 초기화 함수, *a 자체의 값 변경 위해 이중 포인터 매개변수 사용
int freeArray(int *a); // array에 동적할당된 공간 할당 해제 함수
void printArray(int *a); // array의 요소를 출력하는 함수

int selectionSort(int *a); // 선택정렬을 실시하는 함수, 매개변수로 *a 보냄
int insertionSort(int *a); // 삽입정렬을 실시하는 함수, 매개변수로 *a 보냄
int bubbleSort(int *a); // 버블정렬을 실시하는 함수, 매개변수로 *a 보냄
int shellSort(int *a); // 쉘정렬을 실시하는 함수, 매개변수로 *a 보냄
/* recursive function으로 구현 */
int quickSort(int *a, int n); //퀵정렬을 실시하는 함수, 매개변수로 *a, array size 보냄


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key); // hash code 값을 생성하여 반환하는 함수

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht); // array에 대한 해시 테이블 생성을 하는 함수

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key); // 해시 테이블에서 해당 키값을 찾아 index를 반환하는 함수


int main()
{
	char command; // 사용자가 메뉴에서 선택한 명령 저장
	int *array = NULL; // 배열 초기화
	int *hashtable = NULL; // hash table 초기화
	int key = -1; // key값 -1로 초기화
	int index = -1; //index값 -1로 초기화

	srand(time(NULL));

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array); // array 초기화
			break;
		case 'q': case 'Q':
			freeArray(array); // array 할당 공간 해제
			break;
		case 's': case 'S':
			selectionSort(array); // array에 대한 선택정렬 시행
			break;
		case 'i': case 'I':
			insertionSort(array); // array에 대한 삽입정렬 시행
			break;
		case 'b': case 'B':
			bubbleSort(array); // array에 대한 버블정렬 시행
			break;
		case 'l': case 'L':
			shellSort(array); // array에 대한 쉘정렬 시행
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array); // 기존 array 출력
			quickSort(array, MAX_ARRAY_SIZE); // array에 대한 퀵정렬 시행
			printf("----------------------------------------------------------------\n");
			printArray(array); // array 출력

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array); // 기존 array 출력
			hashing(array, &hashtable); // array에 대한 해싱 실행 후 hash table 저장
			printArray(hashtable); // 해싱 이후 hashtable 출력
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);
			printArray(hashtable); // 기존 hash table 출력
			index = search(hashtable, key); // 입력한 key값에 대해 hash table에서 탐색 실시
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);  // 탐색 성공시 index값 출력
			break;

		case 'p': case 'P':
			printArray(array); // array에 대한 출력 실시
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a) // array에 대한 초기화 함수
{
	int *temp = NULL; // int형 포인터 temp를 선언하여 NULL로 초기화

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); // temp 포인터에 MAX_ARRAY_SIZE 크기 만큼의 배열 공간 할당
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a; // array가 NULL이 아닐 경우 temp에 a 주소 대입

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE; // array 각 요소에 MAX_ARRAY_SIZE 보다 작은 값을 랜덤으로 하여 대입

	return 0;
}

int freeArray(int *a)
{
	if(a != NULL) // a가 NULL이 아닐 때, 즉 array가 NULL이 아닐 때
		free(a); // a 공간 할당 해제
	return 0;
}

void printArray(int *a) // array 출력 함수
{
	if (a == NULL) { // array가 NULL일 경우
		printf("nothing to print.\n"); // 오류 메시지 출력
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) // index 출력
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) // 각 index에 속한 요소값 출력
		printf("%5d ", a[i]);
	printf("\n");
}


int selectionSort(int *a) // 선택정렬 실시 함수
{
	int min; // 최소 요소값 저장 변수
	int minindex; // 최소 요소값의 index 저장 변수
	int i, j; // 이중 for문 실행 위한 변수 선언

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 기존 배열 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++) // i를 0부터 array의 최대 index까지 
	{
		minindex = i; // 최소 index를 i로 설정
		min = a[i]; // 최소값을 i번째 index의 요소값으로 설정
		for(j = i+1; j < MAX_ARRAY_SIZE; j++) // 이중 for문
		{
			if (min > a[j]) // 최소값으로 설정된 값보다 a[j]가 작으면
			{
				min = a[j]; // 최소값을 a[j]로 설정
				minindex = j; // 최소값 index를 j로 설정
			}
		}
		a[minindex] = a[i]; // 기존 최소값이 있던 index에 a[i] 값 배치
		a[i] = min; // i번째 index에서는 탐색한 최소값 대입하여 선택정렬 반복 실시
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); // 선택정렬이 완료된 array 출력
	return 0;
}

int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i];
		j = i;
		while (a[j-1] > t && j > 0)
		{
			a[j] = a[j-1];
			j--;
		}
		a[j] = t;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j])
			{
				t = a[j-1];
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)
	{
		for (i = 0; i < h; i++)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				v = a[j];
				k = j;
				while (k > h-1 && a[k-h] > v)
				{
					a[k] = a[k-h];
					k -= h;
				}
				a[k] = v;
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v);
			while(a[--j] > v);

			if (i >= j) break;
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i);
		quickSort(a+i+1, n-i-1);
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE;
}

int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i];
		hashcode = hashCode(key);
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key;
		} else 	{

			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key;
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key);

	if(ht[index] == key)
		return index;

	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index;
}