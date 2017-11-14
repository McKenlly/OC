// C++ program to perform TimSort.
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

int sizeThreads;
int RUN;
int n;
inline int GetRun(int n);
struct Range {
	int left;
	int mid;
	int right;
	int pidThread;
};
int *arr;
// this function sorts array from left index to
// to right index which is of size atmost RUN
void *insertionSort(void *arg);
// merge function merges the sorted runs
void *merge(void *arg);
// iterative Timsort function to sort the
// array[0...n-1] (similar to merge sort)
void TimSort();
// utility function to print the Array
void printArray(int arr[], int n);
// Driver program to test above function
int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: ThreadsNumber\n");
		exit(EXIT_FAILURE);
	}
	sizeThreads = atoi(argv[1]);
	if (sizeThreads <= 0) {
		printf("ERROR: usage ThreadNumber > 0\n");
		exit(EXIT_FAILURE);
	}
	std::cin >> n;
	arr = (int *)malloc(sizeof(int)*n);
	for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    //clock_t start = clock();
	TimSort();
	//clock_t finish = clock();
	//double time = (double) (finish - start)/ CLOCKS_PER_SEC;
	//std::cout << time << std::endl;
	printArray(arr, n);
	for (int i = 0; i < n-1; i++) {
		if (arr[i] > arr[i+1]) std::cout <<"WRONG ANSWER " << arr[i] << "\t" << arr[i+1] << std::endl;
	}
	free(arr);
    return 0;
}

void TimSort()
{
	RUN = GetRun(n);
	// ====================> Sort individual subarrays of size RUN <====================
	pthread_t *thread = (pthread_t *)malloc(sizeThreads*sizeof(pthread_t *));
	//printf("Thread Count %d\n", sizeThreads);
	//Range k[sizeThreads];
	Range *k = (Range *)malloc(sizeof(Range)*sizeThreads);
	int count = 0;
	int shifted = 0;
    for (int i = 0; i < n; i += RUN) {
		if (count == sizeThreads) {
			count = 0;
			shifted++;
		}
		if (shifted > 0) {
			pthread_join(thread[count], NULL);
		}
		k[count].pidThread = count;
		k[count].left = i;
		k[count].right = min(i + RUN, n);
		pthread_create(&thread[count], NULL, insertionSort, &k[count]);
		count++;
	}
	//=================> Wait all thread <======================
	if (shifted > 0) {
		count = sizeThreads;
	}
	for (int i = 0; i < count; i++) {
		pthread_join(thread[i], NULL);
	}
	//std::cout << RUN;
	//=================> Merging all subarray <===================
	for (int size = RUN; size < n; size = 2*size)
    {
		shifted = 0;
		count = 0;
		for (int left = 0; left < n; left += 2*size)
        {
			if (count == sizeThreads) {
				count = 0;
				shifted++;
			}
			if (shifted > 0) {
				pthread_join(thread[count], NULL);
			}
			k[count].pidThread = count;
			k[count].right = min((left + 2*size), n);
			k[count].left = left;
			k[count].mid = left+size;
			//std::cout << "Thread num " << count << std::endl;
			pthread_create(&thread[count], NULL, merge, &k[count]);
			count++;
		}

		if (shifted > 0) {
			count = sizeThreads;
		}
		for (int i = 0; i < count; i++) {
			pthread_join(thread[i], NULL);
		}
	}
	free(k);
	free(thread);
}

void *insertionSort(void *arg)
{
	Range *tmp = (Range *) arg;
	int left = tmp->left;
	int right = tmp->right;
	
	for (int i = left; i < right; i++) { 
    	int x = arr[i];
		int j;
    	for (j = i - 1; j >= left && arr[j] > x; j--)
    		arr[j+1] = arr[j];  	
    	arr[j+1] = x;
	}
} 

void *merge(void *arg)
{
    // original array is broken in two parts
	// left and right array
	Range *tmp = (Range *) arg; 
    
	int len1 = tmp->mid - tmp->left, len2 = tmp->right - tmp->mid;
    
	//int left[len1], right[len2];
	int *left = (int *)malloc(sizeof(int)*len1);
	int *right = (int *)malloc(sizeof(int)*len2);
    //std::cout << "TREAD " << tmp->pidThread << " left " << tmp->left << " mid " << tmp->mid << " right " << tmp->right;
	//std::cout << " len1 " << len1 << " len2 " << len2  << std::endl;
	for (int i = 0; i < len1; i++)
        left[i] = arr[tmp->left + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[tmp->mid + i];
    int i = 0;
    int j = 0;
	
    int k = tmp->left;
    // after comparing, we merge those two array
    // in larger sub array
    while (i < len1 && j < len2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // copy remaining elements of left, if any
    while (i < len1)
    {
        arr[k] = left[i];
        k++;
        i++;
    }
    // copy remaining element of right, if any
    while (j < len2)
    {
        arr[k] = right[j];
        k++;
        j++;
	}
	free(left);
	free(right);
}

inline int GetRun(int n) {
    int r = 0;
    while (n >= 64) {
        n >>= 1;
        r |= n & 1;
    }
    return n + r;
}


void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d\t", arr[i]);
	std::cout << std::endl;
}
 