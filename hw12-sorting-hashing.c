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
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 잘못된 메뉴값 선택하였을 경우 오류 메시지 출력
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

int insertionSort(int *a) // 삽입 정렬 실시 함수
{
	int i, j, t; // 삽입정렬을 실시하기 위해 필요한 변수 i, j, t 선언

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 기존 array 출력

	for(i = 1; i < MAX_ARRAY_SIZE; i++) // 2번째 index 요소값부터 마지막 요소까지 반복
	{
		t = a[i]; // t에 i번째 요소값인 a[i] 대입
		j = i; // j에 index값인 i 대입
		while (a[j-1] > t && j > 0) // array의 i 이전 요소부터 맨 앞 요소까지 반복 (j를 통해), a[j-1]이 i번째 요소값보다 큰 경우
		{
			a[j] = a[j-1]; // 요소값을 기존 index에서 +1만큼 array에서 오른쪽으로 미룸
			j--; // 왼쪽 요소에도 적용하기 위해 j--
		} // t값이 들어갈 알맞은 index인 j 찾음
		a[j] = t; // a[j]에 t값 대입하여 새로운 sorted subset 완성
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); // 삽입정렬이 완료된 array 출력

	return 0;
}

int bubbleSort(int *a) // 버블 정렬 실시 함수
{
	int i, j, t; // 버블정렬을 실시하기 위해 필요한 변수 i, j, t 선언

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 기존 array 출력

	for(i = 0; i < MAX_ARRAY_SIZE; i++) // 이중 for문을 통해 버블 정렬 실현
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j]) // 각 j에 대해 직전 index의 요소값보다 j번째 index의 요소값이 작은 경우
			{
				t = a[j-1]; // j-1번째 요소값과 j번째 요소값에 대한 값에 의한 swap 실시
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	} // 이중 for문을 통해 모든 요소에 대해 오름차순 정렬 완료

	printf("----------------------------------------------------------------\n");
	printArray(a); // 버블정렬이 완료된 array 출력

	return 0;
}

int shellSort(int *a) // 쉘 정렬 실시 함수
{
	int i, j, k, h, v; // 쉘 정렬에 필요한 변수인 i, j, k, h, v 선언

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2) // h만큼 간겨의 떨어진 레코드를 삽입 정렬, 초기값 원소 최대개수/2, h가 1이 될 때까지
	{
		for (i = 0; i < h; i++) // i가 0부터 h가 될 때까지 반복
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h) // 비교할 index값을 i+h부터 h간격 만큼 증가시켜 최대개수보다 작을 때까지
			{
				v = a[j]; // v에 비교할 index 요소값 대입
				k = j; // k에 비교할 index값 대입
				while (k > h-1 && a[k-h] > v) //  h간격 만큼 왼쪽에 있는 a[k-h]가 비교할 a[j]보다 클 때까지, k가 각 부분집합의 첫번째 index값과 같을 때까지 반복
				{
					a[k] = a[k-h]; // h 간격만큼 차이나는 왼쪽 index 요소값을 오른쪽으로 당김
					k -= h; // k값에서 간격인 h만큼 뺌
				} // v(a[j])값이 들어갈 수 있는 각 부분집합의 index k값 찾음
				a[k] = v; // array의 k번째 index에 v값 대입하여 쉘 정렬 실시
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a); // 쉘 정렬이 완료된 array 출력

	return 0;
}

int quickSort(int *a, int n) // 퀵 정렬 실시 함수
{
	int v, t; // 퀵 정렬 기준이 되는 pivot, 값에 의한 swap위해 값을 임시로 저장할 temp 선언
	int i, j; // 왼쪽, 오른쪽 부분집합의 index를 지정할 변수 선언

	if (n > 1) // n>1, 즉 recursive를 통해 더 퀵정렬을 실시해야 할 때
	{
		v = a[n-1]; // pivot값을 a[n-1]로 설정
		i = -1; // 왼쪽 부분집합 탐색을 위한 초기 i값을 -1로 설정
		j = n - 1; // 오른쪽 부분집합 탐색을 위한 초기 i값을 n-1로 설정

		while(1) // 주어진 조건을 만족할 때까지 반복
		{
			while(a[++i] < v); // 왼쪽 부분집합에서 pivot값보다 작은 요소 중 가장 큰 index 가지는 a[i] 찾음
			while(a[--j] > v); // 오른쪽 부분집합에서 pivot값보다 큰 요소 중 가장 작은 index 가지는 a[j] 찾음

			if (i >= j) break; // i가 j보다 크거나 같을 때, 즉 퀵 정렬 사이클이 마무리되었을 경우 break
			t = a[i]; // 왼쪽 부분집합에 속한 i번째 요소와 오른쪽 부분집합에 속한 j번째 요소 값에 의한 swap 실시하여 
			a[i] = a[j]; // pivot보다 작은 값은 왼쪽 부분집합으로 이동
			a[j] = t; // pivot보다 큰 값은 오른쪽 부분집합으로 이동
		}
		t = a[i]; // (퀵정렬 마무리) a[i]와 a[n-1](pivot) 요소값에 대한 swap 실시
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i); // 왼쪽 부분집합에 대한 퀵 정렬을 recursive로 실행
		quickSort(a+i+1, n-i-1); // 오른쪽 부분집합에 대한 퀵 정렬을 recursive로 실행
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE; // hash code 값을 구하여 return함
}

int hashing(int *a, int **ht) // 해싱을 실시하는 함수, 매개변수로 array 주소와 hashtable에 대한 이중 포인터 받음
{
	int *hashtable = NULL; // hashtable 포인터를 선언하여 NULL로 초기화

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) { // *ht가 NULL일 경우, 즉 hash table에 대한 공간 할당이 되지 않은 경우
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); // 동적 할당을 통해 array size와 동일하게 hash table에 공간 할당
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1; // 모든 hash table 요소에 대해 0으로 초기화

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1; // key, hashcode, index 변수롤 선언하여 -1로 초기화
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) // 모든 array 요소에 대해 오름차순을 반복
	{
		key = a[i]; // key에 array의 i번째 요소값 대입
		hashcode = hashCode(key); // 해당 key값에 대한 hash code 생성하여 대입
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1) // hashtable의 hashcode index 요소값이 -1인 경우, 즉 해당 index가 비었을 경우
		{
			hashtable[hashcode] = key; // 해당 index에 key값 대입
		} else 	{ // 해당 index가 비지 않았을 경우
 
			index = hashcode; // index에 hashcode 값 대입

			while(hashtable[index] != -1) // hashtable의 index 변수에 대한 공간이 비지 않았을 때 계속 반복
			{
				index = (++index) % MAX_HASH_TABLE_SIZE; // index에 (index+1)값을 hash table 크기만큼 나눈 나머지 값 대입(빈 index 찾음)
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key; // 빈 공간을 찾은 경우 해당 hashtable의index번째 요소값에 key 대입
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key); // index 변수 선언하여 사용자가 검색하고자 하는 key값에 대한 hashcode 대입

	if(ht[index] == key) // ht에서 hashcode를 통해 찾은 값이 key값과 동일한 경우
		return index; // 해당 index 반환

	while(ht[++index] != key) //동일하지 않은 경우 ht의 index+1번째 요소값과 key값이 같을 때까지 반복
	{
		index = index % MAX_HASH_TABLE_SIZE; // index에 index를 hash table 크기만큼 나눈 나머지값 대입
	}
	return index; // 조건을 만족하는 index 찾은 경우 반환
}