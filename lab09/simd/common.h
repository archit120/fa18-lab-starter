#ifndef COMMON_H
#define COMMON_H

#include <x86intrin.h>

#define NUM_ELEMS ((1 << 16) + 10)
#define OUTER_ITERATIONS (1 << 16)

long long int sum(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS; i++) {
			if(vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) { 
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				// This is where you should put your final result!
											// DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE.
	int store[4];
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* YOUR CODE GOES HERE */
		__m128i temp_sum = _mm_setzero_si128();
		for(unsigned int i = 0 ;i+3 < NUM_ELEMS; i+=4)
		{
			__m128i vals_mm = _mm_loadu_si128((__m128i *)(vals+i));
			__m128i mask = _mm_cmpgt_epi32(vals_mm, _127);
			vals_mm = _mm_and_si128(vals_mm, mask);
			temp_sum = _mm_add_epi32(temp_sum, vals_mm);

		}
		_mm_storeu_si128((__m128i_u *)store, temp_sum);
		result += store[0]+store[1]+store[2]+store[3];
		for(unsigned int i = (NUM_ELEMS/4)*4; i<NUM_ELEMS;i++)
			if(vals[i]>=128)
				result+=vals[i];
		/* You'll need a tail case. */

	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	int store[4];
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* COPY AND PASTE YOUR sum_simd() HERE */
		/* MODIFY IT BY UNROLLING IT */

		__m128i temp_sum = _mm_setzero_si128();
		for(unsigned int i = 0 ;i+15 < NUM_ELEMS; i+=16)
		{
			__m128i vals_mm = _mm_loadu_si128((__m128i *)(vals+i));
			__m128i mask = _mm_cmpgt_epi32(vals_mm, _127);
			vals_mm = _mm_and_si128(vals_mm, mask);
			temp_sum = _mm_add_epi32(temp_sum, vals_mm);

			vals_mm = _mm_loadu_si128((__m128i *)(vals+i+4));
			mask = _mm_cmpgt_epi32(vals_mm, _127);
			vals_mm = _mm_and_si128(vals_mm, mask);
			temp_sum = _mm_add_epi32(temp_sum, vals_mm);

			vals_mm = _mm_loadu_si128((__m128i *)(vals+i+8));
			mask = _mm_cmpgt_epi32(vals_mm, _127);
			vals_mm = _mm_and_si128(vals_mm, mask);
			temp_sum = _mm_add_epi32(temp_sum, vals_mm);

			vals_mm = _mm_loadu_si128((__m128i *)(vals+i+12));
			mask = _mm_cmpgt_epi32(vals_mm, _127);
			vals_mm = _mm_and_si128(vals_mm, mask);
			temp_sum = _mm_add_epi32(temp_sum, vals_mm);
		}

		for(unsigned int i = NUM_ELEMS/16 * 16 ;i+3 < NUM_ELEMS; i+=4)
		{
			__m128i vals_mm = _mm_loadu_si128((__m128i *)(vals+i));
			__m128i mask = _mm_cmpgt_epi32(vals_mm, _127);
			vals_mm = _mm_and_si128(vals_mm, mask);
			temp_sum = _mm_add_epi32(temp_sum, vals_mm);

		}

		_mm_storeu_si128((__m128i_u *)store, temp_sum);
		result += store[0]+store[1]+store[2]+store[3];
		for(unsigned int i = (NUM_ELEMS/4)*4; i<NUM_ELEMS;i++)
			if(vals[i]>=128)
				result+=vals[i];

		/* You'll need 1 or maybe 2 tail cases here. */

	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

/* DON'T TOUCH THIS FUNCTION */
int int_comparator(const void* a, const void* b) {
	if(*(unsigned int*)a == *(unsigned int*)b) return 0;
	else if(*(unsigned int*)a < *(unsigned int*)b) return -1;
	else return 1;
}

#endif
