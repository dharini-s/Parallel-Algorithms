/**
 *  \file sort.cc
 *
 *  \brief Implements a generic sequential sort, plus other helper
 *  routines for managing keys. See 'sort.hh'.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "sort.hh"

/* ============================================================
 * The following code implements a sequentialSort().
 */

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

void sequentialSort (int N, keytype* A)
{
  qsort (A, N, sizeof (keytype), compare);
}

/* ============================================================
 * Some helper routines for managing an array of keys.
 */

keytype *
newKeys (int N)
{
  keytype* A = (keytype *)malloc (N * sizeof (keytype));
  assert (A);
  return A;
}

/** Returns a new copy of A[0:N-1] */
keytype *
newCopy (int N, const keytype* A)
{
  keytype* A_copy = newKeys (N);
  memcpy (A_copy, A, N * sizeof (keytype));
  return A_copy;
}

/* ============================================================
 * Code for checking the sorted results
 */

void assertIsSorted (int N, const keytype* A)
{
  for (int i = 1; i < N; ++i) {
    if (A[i-1] > A[i]) {
      fprintf (stderr, "*** ERROR ***\n");
      fprintf (stderr, "  A[i=%d] == %lu > A[%d] == %lu\n", i-1, A[i-1], i, A[i]);
      assert (A[i-1] <= A[i]);
    }
  } /* i */
  printf ("\t(Array is sorted.)\n");
}

void assertIsEqual (int N, const keytype* A, const keytype* B)
{
  for (int i = 0; i < N; ++i) {
    if (A[i] != B[i]) {
      fprintf (stderr, "*** ERROR ***\n");
      fprintf (stderr, "A[i]=%d, B[i]=%d", A[i], B[i]);
      fprintf (stderr, "  A[i=%d] == %lu, but B[%d] == %lu\n", i, A[i], i, B[i]);
      assert (A[i] == B[i]);
    }
  } /* i */
  printf ("\t(Arrays are equal.)\n");
}

int binarySearch(keytype *arr, keytype v, int p, int r)
{
	int low = p;
	int high = max(p, r + 1);
	while (low < high)
	{
		int mid = (high + low) / 2; //why?
		if (v <= arr[mid])
			high = mid;
		else
			low = mid + 1;
	}
	return high;
}

void swap(int &a, int &b)
{
        int temp = a;
        a = b;
        b = temp;
}

 
int max(int a, int b)
{
    return (a < b) ? b : a;
}
/* eof */
