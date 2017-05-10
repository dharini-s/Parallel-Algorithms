/**
 *  \file parallel-mergesort.cc
 *  Author: Dharini Sreenivasan
 *  Parallel merge sort based on the CLRS algorithm
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>

#include "sort.hh"

static int compare (const void* a, const void* b)
{
  keytype ka = *(const keytype *)a;
  keytype kb = *(const keytype *)b;
  if (ka < kb)
    return -1;
  else if (ka == kb)
    return 0;
  else
    return 1;
}

void merge(keytype *array, keytype *res, int p1, int r1, int p2, int r2, int p3, int threads)
{
	int n1 = r1 - p1 + 1;
	int n2 = r2 - p2 + 1;
	if (n1 < n2)
	{
		swap(r1, r2);
		swap(p1, p2);
		swap(n1, n2);
	}
		
	if (n1 == 0)
		return;
	else
	{

		int q1 = (p1 + r1) / 2;
		int q2 = binarySearch(array, array[q1], p2, r2);
		int q3 = p3 + (q1 - p1) + (q2 - p2);
		res[q3] = array[q1];
		if(threads <= 1)
        	{
			merge(array, res, p1, q1 - 1, p2, q2 - 1, p3, threads/2);
			merge(array, res, q1 + 1, r1, q2, r2, q3 + 1, threads/2);
		}	
		else
		{
			#pragma omp task
			{			
				merge(array, res, p1, q1 - 1, p2, q2 - 1, p3, threads/2);
			}
			merge(array, res, q1 + 1, r1, q2, r2, q3 + 1, threads/2);
			#pragma omp taskwait
		}
	}
}

void mergeSort(keytype* array, keytype *res, int low, int high, int s, int threads)
{
	int N = high - low;
	if (N == 0)
		res[s] = array[low];
	else if(threads <= 1)
        {
                qsort(array+low, N+1, sizeof(keytype), compare);
		memcpy(res + s, array + low, (N+1) * sizeof(keytype));
               //mergeSort(array, T, low, mid, 0, threads/2);
               //mergeSort(array, T, mid + 1, high, mid1 + 1, threads/2);
        }
	else
	{
		keytype *T = new keytype[N+1];
		int mid = (low + high) / 2;
		int mid1 = mid - low;
		#pragma omp task
		{ 
			mergeSort(array, T, low, mid, 0, threads/2);
		}
		mergeSort(array, T, mid + 1, high, mid1 + 1, threads/2);
		#pragma omp taskwait
		{	
			merge(T, res, 0, mid1, mid1 + 1, N, s, threads);
		}
		delete T;
	}
}

void parallelSort(int N, keytype* A)
{
	omp_set_num_threads(64);
	keytype* res = new keytype[N];
	#pragma omp parallel
	{
		#pragma omp single
		{		
        		mergeSort(A, res, 0, N-1, 0, omp_get_num_threads()); //Must pass N-1 according to impl.
		}
	}
	memcpy(A, res, N * sizeof(keytype));
	delete res;
}
