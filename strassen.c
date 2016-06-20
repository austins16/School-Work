/************************************************************************/
// gcc -O1 -fopenmp -pthread -o strassen strassen.c -lm -lrt
// Strassen Method Matrix Matrix Multiply using OMP, PThreads, and loop unrolling
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <pthread.h>

#define GIG 1000000000
#define CPG 3.611957           // Cycles per GHz -- Adjust to your computer
// cache size = 2048 KB

#define BASE	0
#define ITERS	1
#define DELTA	1024 // matrix size Iters x Delta

#define OPTIONS	6 // number of outputs
#define IDENT	0

#define THRESHOLD	DELTA/4
#define UNROLL_THRESHOLD	8
#define TOLERANCE	0.1

#define OMP_THREADS		8

#define DEBUG	0

typedef float data_t;

/* Create abstract data type for matrix */
typedef struct {
	long int len;
	data_t *data;
} matrix_rec, *matrix_ptr;

struct thread_data{
	int thread_id;
	matrix_ptr a;
	matrix_ptr b;
	matrix_ptr c;
	matrix_ptr p;
};

int NUM_THREADS = 7;
int chunk = 16;

/************************************************************************/
main(int argc, char *argv[])
{
	int OPTION;
	struct timespec diff(struct timespec start, struct timespec end);
	struct timespec time1, time2;
	struct timespec time_stamp[OPTIONS][ITERS+1];
	int clock_gettime(clockid_t clk_id, struct timespec *tp);
	matrix_ptr new_matrix(long int len);
	int set_matrix_length(matrix_ptr m, long int index);
	long int get_matrix_length(matrix_ptr m);
	int init_matrix(matrix_ptr m, long int len);
	int zero_matrix(matrix_ptr m, long int len);
	data_t *get_matrix_start(matrix_ptr m);
	void mmm_kij(matrix_ptr a, matrix_ptr b, matrix_ptr c);
	void mmm_kij_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c);
	void mmm_strassen(matrix_ptr a, matrix_ptr b, matrix_ptr c);
	void mmm_strassen_pt(matrix_ptr a, matrix_ptr b, matrix_ptr c);
	void mmm_strassen_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c);
	void mmm_strassen_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c);

	long int i, j, k;
	long int time_sec, time_ns;
	long int MAXSIZE = BASE+(ITERS+1)*DELTA;

	printf("\n Hello World -- MMM \n");

	// declare and initialize the matrix structure
	matrix_ptr a0 = new_matrix(MAXSIZE);
	init_matrix(a0, MAXSIZE);
	matrix_ptr b0 = new_matrix(MAXSIZE);
	init_matrix(b0, MAXSIZE);
	matrix_ptr c0 = new_matrix(MAXSIZE);
	zero_matrix(c0, MAXSIZE);
	matrix_ptr c1 = new_matrix(MAXSIZE);
	zero_matrix(c1, MAXSIZE);
	matrix_ptr c2 = new_matrix(MAXSIZE);
	zero_matrix(c2, MAXSIZE);

	OPTION = 0;
	for (i = 0; i < ITERS; i++) {
		set_matrix_length(a0,BASE+(i+1)*DELTA);
		set_matrix_length(b0,BASE+(i+1)*DELTA);
		set_matrix_length(c0,BASE+(i+1)*DELTA);
		clock_gettime(CLOCK_REALTIME, &time1);  // use clock to determine runtime of function
		mmm_kij(a0,b0,c0);
		clock_gettime(CLOCK_REALTIME, &time2);
		time_stamp[OPTION][i] = diff(time1,time2);
		//printf("\niter = %d", i);
	}
	printf("Done original kij MMM\n");
	
	OPTION++;
	for (i = 0; i < ITERS; i++) {
		set_matrix_length(a0,BASE+(i+1)*DELTA);
		set_matrix_length(b0,BASE+(i+1)*DELTA);
		set_matrix_length(c0,BASE+(i+1)*DELTA);
		clock_gettime(CLOCK_REALTIME, &time1);  // use clock to determine runtime of function
		mmm_kij_omp(a0,b0,c0);
		clock_gettime(CLOCK_REALTIME, &time2);
		time_stamp[OPTION][i] = diff(time1,time2);
		//printf("\niter = %d", i);
	}
	printf("Done kij_omp\n");
	
	OPTION++;
	for (i = 0; i < ITERS; i++) {
		set_matrix_length(a0,BASE+(i+1)*DELTA);
		set_matrix_length(b0,BASE+(i+1)*DELTA);
		set_matrix_length(c0,BASE+(i+1)*DELTA);
		clock_gettime(CLOCK_REALTIME, &time1);  // use clock to determine runtime of function
		mmm_strassen(a0,b0,c0);
		clock_gettime(CLOCK_REALTIME, &time2);
		time_stamp[OPTION][i] = diff(time1,time2);
		//printf("\niter = %d", i);
	}
	printf("Done strassen\n");
	
	OPTION++;
	for (i = 0; i < ITERS; i++) {
		set_matrix_length(a0,BASE+(i+1)*DELTA);
		set_matrix_length(b0,BASE+(i+1)*DELTA);
		set_matrix_length(c0,BASE+(i+1)*DELTA);
		clock_gettime(CLOCK_REALTIME, &time1);  // use clock to determine runtime of function
		mmm_strassen_unroll(a0,b0,c0);
		clock_gettime(CLOCK_REALTIME, &time2);
		time_stamp[OPTION][i] = diff(time1,time2);
		//printf("\niter = %d", i);
	}
	printf("Done strassen_unroll\n");
	
	OPTION++;
	for (i = 0; i < ITERS; i++) {
		set_matrix_length(a0,BASE+(i+1)*DELTA);
		set_matrix_length(b0,BASE+(i+1)*DELTA);
		set_matrix_length(c0,BASE+(i+1)*DELTA);
		clock_gettime(CLOCK_REALTIME, &time1);  // use clock to determine runtime of function
		mmm_strassen_omp(a0,b0,c0);
		clock_gettime(CLOCK_REALTIME, &time2);
		time_stamp[OPTION][i] = diff(time1,time2);
		//printf("\niter = %d", i);
	}
	printf("Done strassen_omp\n");
	
	OPTION++;
	for (i = 0; i < ITERS; i++) {
		set_matrix_length(a0,BASE+(i+1)*DELTA);
		set_matrix_length(b0,BASE+(i+1)*DELTA);
		set_matrix_length(c0,BASE+(i+1)*DELTA);
		clock_gettime(CLOCK_REALTIME, &time1);  // use clock to determine runtime of function
		mmm_strassen_pt(a0,b0,c0);
		clock_gettime(CLOCK_REALTIME, &time2);
		time_stamp[OPTION][i] = diff(time1,time2);
		//printf("\niter = %d", i);
	}
	printf("Done strassen_pt\n");

	int length = get_matrix_length(c0);
	int size = length*length;
	data_t *h0 = get_matrix_start(c0);
	data_t *h1 = get_matrix_start(c2);
	data_t *a = get_matrix_start(a0);
	data_t *b = get_matrix_start(b0);

	#if(DEBUG)
	printf("\nMatrix A:\n");
	for(i=0; i<length; i++){
		for(j=0; j<length; j++){
		  printf("%5.1f ", a[i*length+j]);
		}
		printf("\n");
	}
	printf("\nMatrix B:\n");
	for(i=0; i<length; i++){
		for(j=0; j<length; j++){
		  printf("%5.1f ", b[i*length+j]);
		}
		printf("\n");
	}
	printf("\nijk Matrix:\n");
	for(i=0; i<length; i++){
		for(j=0; j<length; j++){
		  printf("%5.1f ", h0[i*length+j]);
		}
		printf("\n");
	}
	printf("\nstrassen Matrix:\n");
	for(i=0; i<length; i++){
		for(j=0; j<length; j++){
		  printf("%5.1f ", h1[i*length+j]);
		}
		printf("\n");
	}
	#endif
	
	/*
	int counter = 0;
	for(i=0; i<size; i++){
		if(abs(h0[i] - h1[i]) > 0.1){
			counter++;
		}
	}
	if(counter != 0){
	  printf("TEST FAILED: %d incorrect elements\n", counter);
	} else{
	  printf("TEST PASSED, ALL RESULTS CORRECT\n");
	}
	*/

	printf("\nlength, kij, kij_omp, strassen, strassen_unroll, strassen_omp, strassen_pt");
	for (i = 0; i < ITERS; i++) {
	printf("\n%d, ", BASE+(i+1)*DELTA);
		for (j = 0; j < OPTIONS; j++) {
		  if (j != 0) printf(", ");
		  printf("%ld", (long int)((double)(CPG)*(double)
			 (GIG * time_stamp[j][i].tv_sec + time_stamp[j][i].tv_nsec)));
		}
	}

	printf("\n");

}/* end main */

/**********************************************/

/* Create matrix of specified length */
matrix_ptr new_matrix(long int len)
{
	long int i;

	/* Allocate and declare header structure */
	matrix_ptr result = (matrix_ptr) malloc(sizeof(matrix_rec));
	if (!result) return NULL;  /* Couldn't allocate storage */
	result->len = len;


	if (len > 0) {
		data_t *data = (data_t *) calloc(len*len, sizeof(data_t));
		if (!data) {
			free((void *) result);
			printf("\n COULDN'T ALLOCATE STORAGE \n", result->len);
			return NULL;  /* Couldn't allocate storage */
		}
		result->data = data;
	}
	else result->data = NULL;

	return result;
}

/* Set length of matrix */
int set_matrix_length(matrix_ptr m, long int index)
{
	m->len = index;
	return 1;
}

/* Return length of matrix */
long int get_matrix_length(matrix_ptr m)
{
	return m->len;
}

/* initialize matrix */
int init_matrix(matrix_ptr m, long int len)
{
	long int i;
	srand(1);

	if (len > 0) {
		m->len = len;
		for (i = 0; i < len*len; i++)
			m->data[i] = (data_t)(rand() % 10);
		return 1;
	}
	else return 0;
}

/* initialize matrix */
int zero_matrix(matrix_ptr m, long int len)
{
	long int i,j;

	if (len > 0) {
		m->len = len;
		for (i = 0; i < len*len; i++)
			m->data[i] = (data_t)(IDENT);
		return 1;
	}
	else return 0;
}

data_t *get_matrix_start(matrix_ptr m)
{
	return m->data;
}

/*************************************************/

struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

/*************************************************/

/* mmm kij */
void mmm_kij(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
  long int i, j, k;
  long int get_matrix_length(matrix_ptr m);
  data_t *get_matrix_start(matrix_ptr m);
  long int length = get_matrix_length(a);
  data_t *a0 = get_matrix_start(a);
  data_t *b0 = get_matrix_start(b);
  data_t *c0 = get_matrix_start(c);
  data_t sum;

  for (k = 0; k < length; k++)
    for (i = 0; i < length; i++) {
      sum = a0[i*length+k];
      for (j = 0; j < length; j++)
		c0[i*length+j] += sum*b0[k*length+j];
    }
}

void mmm_kij_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c){
	int i, j, k;
	int length = get_matrix_length(a);
	data_t *a0 = get_matrix_start(a);
	data_t *b0 = get_matrix_start(b);
	data_t *c0 = get_matrix_start(c);
	data_t sum;

	#pragma omp parallel shared(a0,b0,c0) private(i,j,k,sum) num_threads(OMP_THREADS)
	{
		#pragma omp for schedule(static)
			for (k = 0; k < length; k++){
				for (i = 0; i < length; i++){
					sum = a0[i*length+k];
					for (j = 0; j < length; j++)
						c0[i*length+j] += sum*b0[k*length+j];
				}
			}
	}
}

void mmm_strassen(matrix_ptr a, matrix_ptr b, matrix_ptr c);
void mmm_strassen_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c);

void m_add(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2);
void m_add_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2);
void m_add_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2);

void m_add_full(matrix_ptr a, matrix_ptr b, matrix_ptr c);
void m_add_full_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c);
void m_add_full_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c);

void m_sub(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2);
void m_sub_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2);
void m_sub_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2);

void m_sub_full(matrix_ptr a, matrix_ptr b, matrix_ptr c);
void m_sub_full_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c);
void m_sub_full_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c);

void m_mul(matrix_ptr a, matrix_ptr b, matrix_ptr c);

void *pt_strassen_work(void *threadarg){
	int i, j;
	struct thread_data *my_data;
	my_data = (struct thread_data*) threadarg;
	int taskid = my_data->thread_id;
	matrix_ptr aM = my_data->a;
	matrix_ptr bM = my_data->b;
	int length = get_matrix_length(aM);
	int sublength = length/2;
	
	matrix_ptr P = new_matrix(sublength);
	my_data->p = P;
	
	matrix_ptr temp1 = new_matrix(sublength);
	matrix_ptr temp2 = new_matrix(sublength);
	
	switch(taskid){ // Do 7 recursive matrix multiplications for Strassen Method 
		case 0: { // P1 = (a11 + a22)(b11 + b22)
			m_add(aM, aM, temp1, 0, 0, sublength, sublength);
			m_add(bM, bM, temp2, 0, 0, sublength, sublength);
			mmm_strassen(temp1, temp2, P);
		} break;
		case 1: { // P2 = (a21 + a22)(b11)
			matrix_ptr b11 = new_matrix(sublength);
			for(i=0; i < sublength; i++){
				for(j=0; j < sublength; j++){
					b11->data[i*sublength+j] = bM->data[i*length+j];
				} 
			}
			m_add(aM, aM, temp1, sublength, 0, sublength, sublength);
			mmm_strassen(temp1, b11, P);
		} break;
		case 2: { // P3 = (a11)(b12 - b22)
			matrix_ptr a11 = new_matrix(sublength);
			for(i=0; i < sublength; i++){
				for(j=0; j < sublength; j++){
					a11->data[i*sublength+j] = bM->data[i*length+j];
				} 
			}
			m_sub(bM, bM, temp1, 0, sublength, sublength, sublength);
			mmm_strassen(a11, temp1, P);
		} break;
		case 3: { // P4 = (a22)(b21 - b11)
			matrix_ptr a22 = new_matrix(sublength);
			for(i=0; i < sublength; i++){
				for(j=0; j < sublength; j++){						  
					a22->data[i*sublength+j] = aM->data[(i+sublength)*length+sublength+j];
				}
			}
			m_sub(bM, bM, temp1, sublength, 0, 0, 0);
			mmm_strassen(a22, temp1, P);
		} break;
		case 4: { // P5 = (a11 + 12)(b22)
			matrix_ptr b22 = new_matrix(sublength);
			for(i=0; i < sublength; i++){
				for(j=0; j < sublength; j++){						  
					b22->data[i*sublength+j] = aM->data[(i+sublength)*length+sublength+j];
				}
			}
			m_add(aM, aM, temp1, 0, 0, 0, sublength);
			mmm_strassen(temp1, b22, P);
		} break;
		case 5: { // P6 = (a21 - a11)(b11 + b12)
			m_sub(aM, aM, temp1, sublength, 0, 0, 0);
			m_add(bM, bM, temp2, 0, 0, 0, sublength);
			mmm_strassen(temp1, temp2, P);
		} break;
		case 6: { // P7 = (a12 - a22)(b21 + b22)
			m_sub(aM, aM, temp1, 0, sublength, sublength, sublength);
			m_add(bM, bM, temp2, sublength, 0, sublength, sublength);
			mmm_strassen(temp1, temp2, P);
		} break;
		default: {
			printf("ERROR: Program should not reach here\n");
		} break;
	}
	
	free(temp1);
	free(temp2);
	
	pthread_exit(NULL);
}
/* mmm using Strassen method with Pthreads */
void mmm_strassen_pt(matrix_ptr a, matrix_ptr b, matrix_ptr c){
	int i, j, k, rc, t;
	int length = get_matrix_length(a);
	int sublength = length/2;	
	pthread_t threads[NUM_THREADS];
	struct thread_data thread_data_array[NUM_THREADS];
	data_t *c0 = get_matrix_start(c);
		  
	matrix_ptr c11 = new_matrix(sublength);
	matrix_ptr c12 = new_matrix(sublength);
	matrix_ptr c21 = new_matrix(sublength);
	matrix_ptr c22 = new_matrix(sublength);
		  
	matrix_ptr temp1 = new_matrix(sublength);
	matrix_ptr temp2 = new_matrix(sublength);
	
	for(t=0; t < NUM_THREADS; t++){
		thread_data_array[t].thread_id = t;
		thread_data_array[t].a = a;
		thread_data_array[t].b = b;
		thread_data_array[t].c = c;
		rc = pthread_create(&threads[t], NULL, pt_strassen_work, (void*)&thread_data_array[t]);
		if(rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	
	for(t=0; t < NUM_THREADS; t++){
		if(pthread_join(threads[t], NULL)){
			printf("ERROR; code on return from join is %d\n", rc);
			exit(-1);
		}
	}
	
	//recombining at the very end
	m_add_full(thread_data_array[0].p, thread_data_array[3].p, temp1);
	m_sub_full(temp1, thread_data_array[4].p, temp2);
	m_add_full(temp2, thread_data_array[6].p, c11);
		  
	m_add_full(thread_data_array[2].p, thread_data_array[4].p, c12);
		  
	m_add_full(thread_data_array[1].p, thread_data_array[3].p, c21);
		  
	m_sub_full(thread_data_array[0].p, thread_data_array[1].p, temp1);
	m_add_full(temp1, thread_data_array[2].p, temp2);
	m_add_full(temp2, thread_data_array[5].p, c22);
	  
	for(i=0; i<sublength; i++){
		for(j=0; j<sublength; j++){
			c0[i*length+j] = c11->data[i*sublength+j];		  
			c0[i*length+sublength+j] = c12->data[i*sublength+j];	  
			c0[(i+sublength)*length+j] = c21->data[i*sublength+j];
			c0[(i+sublength)*length+sublength+j] = c22->data[i*sublength+j];
		}
	}

	free(c11);
	free(c12);
	free(c21);
	free(c22);
	free(temp1);
	free(temp2);	
	
}
/* mmm using Strassen method with OMP */
void mmm_strassen_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c)//, int startrow1, int startcol1, int startrow2, int startcol2, int startrow3, int startcol3)
{
	
	int i, j, k;
	int sublength;
	int length = get_matrix_length(a);
	data_t *a0 = get_matrix_start(a);
	data_t *b0 = get_matrix_start(b);
	data_t *c0 = get_matrix_start(c);
  
	if(length <= THRESHOLD){
		m_mul(a, b, c);
		//mmm_kij_omp(a,b,c);
	} 
	else{
		sublength = length/2;
		
		matrix_ptr a11 = new_matrix(sublength);	matrix_ptr a22 = new_matrix(sublength);
		
		matrix_ptr b11 = new_matrix(sublength);	matrix_ptr b22 = new_matrix(sublength);
			  
		matrix_ptr c11 = new_matrix(sublength);	matrix_ptr c12 = new_matrix(sublength);
		matrix_ptr c21 = new_matrix(sublength);	matrix_ptr c22 = new_matrix(sublength);
		
		matrix_ptr P1 = new_matrix(sublength);	matrix_ptr P2 = new_matrix(sublength);
		matrix_ptr P3 = new_matrix(sublength);	matrix_ptr P4 = new_matrix(sublength);
		matrix_ptr P5 = new_matrix(sublength);	matrix_ptr P6 = new_matrix(sublength);
		matrix_ptr P7 = new_matrix(sublength);
			  
		matrix_ptr temp1 = new_matrix(sublength);	matrix_ptr temp2 = new_matrix(sublength);
		
		#pragma omp parallel shared(a0,b0,a11,a22,b11,b22,sublength,chunk) private(i,j) num_threads(OMP_THREADS)
		{
			#pragma omp for schedule (dynamic,chunk) nowait
			for(i=0; i < sublength; ++i){
				for(j=0; j < sublength; ++j){
					a11->data[i*sublength+j] = a0[i*length+j];
					b11->data[i*sublength+j] = b0[i*length+j];
					a22->data[i*sublength+j] = a0[(i+sublength)*length+sublength+j];
					b22->data[i*sublength+j] = b0[(i+sublength)*length+sublength+j];
				} 
			}
		}
		// Do 7 recursive Matrix multiplications for Strassen Method
		//P1 = (a11 + a22)(b11 + b22)
		m_add_omp(a, a, temp1, 0, 0, sublength, sublength);
		m_add_omp(b, b, temp2, 0, 0, sublength, sublength);
		mmm_strassen(temp1, temp2, P1);
				  
		//P2 = (a21 + a22)(b11)
		m_add_omp(a, a, temp1, sublength, 0, sublength, sublength);
		mmm_strassen(temp1, b11, P2);
			  
		//P3 = (a11)(b12 - b22)
		m_sub_omp(b, b, temp1, 0, sublength, sublength, sublength);
		mmm_strassen(a11, temp1, P3);
			  
		//P4 = (a22)(b21 - b11)
		m_sub_omp(b, b, temp1, sublength, 0, 0, 0);
		mmm_strassen(a22, temp1, P4);
			  
		//P5 = (a11 + 12)(b22)
		m_add_omp(a, a, temp1, 0, 0, 0, sublength);
		mmm_strassen(temp1, b22, P5);
			  
		//P6 = (a21 - a11)(b11 + b12)
		m_sub_omp(a, a, temp1, sublength, 0, 0, 0);
		m_add_omp(b, b, temp2, 0, 0, 0, sublength);
		mmm_strassen(temp1, temp2, P6);
			  
		//P7 = (a12 - a22)(b21 + b22)
		m_sub_omp(a, a, temp1, 0, sublength, sublength, sublength);
		m_add_omp(b, b, temp2, sublength, 0, sublength, sublength);
		mmm_strassen(temp1, temp2, P7);
			  
		m_add_full_omp(P1, P4, temp1);
		m_sub_full_omp(temp1, P5, temp2);
		m_add_full_omp(temp2, P7, c11);
			  
		m_add_full_omp(P3, P5, c12);
			  
		m_add_full_omp(P2, P4, c21);
			  
		m_sub_full_omp(P1, P2, temp1);
		m_add_full_omp(temp1, P3, temp2);
		m_add_full_omp(temp2, P6, c22);
		
		#pragma omp parallel shared(c0,c11,c12,c21,c22,sublength,chunk) private(i,j) num_threads(OMP_THREADS)
		{
			#pragma omp for schedule (dynamic,chunk) nowait
			for(i=0; i<sublength; ++i){
				for(j=0; j<sublength; ++j){
					c0[i*length+j] = c11->data[i*sublength+j];		  
					c0[i*length+sublength+j] = c12->data[i*sublength+j];	  
					c0[(i+sublength)*length+j] = c21->data[i*sublength+j];
					c0[(i+sublength)*length+sublength+j] = c22->data[i*sublength+j];
				}
			}	
		}
		
		free(a11); free(a22);
		free(b11); free(b22);
		free(c11); free(c12); free(c21); free(c22);
		free(P1); free(P2); free(P3); free(P4); free(P5); free(P6); free(P7);
		free(temp1); free(temp2);
		
	}  
}
// Base Case Strassen Method
void mmm_strassen(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
	int i, j, k;
	int sublength;
	int length = get_matrix_length(a);
	data_t *a0 = get_matrix_start(a);
	data_t *b0 = get_matrix_start(b);
	data_t *c0 = get_matrix_start(c);
  
	if(length <= THRESHOLD){
		m_mul(a, b, c);
		//mmm_kij_omp(a,b,c);
	} 
	else{
		sublength = length/2;
		
		matrix_ptr a11 = new_matrix(sublength);	matrix_ptr a22 = new_matrix(sublength);
		
		matrix_ptr b11 = new_matrix(sublength);	matrix_ptr b22 = new_matrix(sublength);
			  
		matrix_ptr c11 = new_matrix(sublength);	matrix_ptr c12 = new_matrix(sublength);
		matrix_ptr c21 = new_matrix(sublength);	matrix_ptr c22 = new_matrix(sublength);
		
		matrix_ptr P1 = new_matrix(sublength);	matrix_ptr P2 = new_matrix(sublength);
		matrix_ptr P3 = new_matrix(sublength);	matrix_ptr P4 = new_matrix(sublength);
		matrix_ptr P5 = new_matrix(sublength);	matrix_ptr P6 = new_matrix(sublength);
		matrix_ptr P7 = new_matrix(sublength);
			  
		matrix_ptr temp1 = new_matrix(sublength);	matrix_ptr temp2 = new_matrix(sublength);
		  
		for(i=0; i < sublength; ++i){
			for(j=0; j < sublength; ++j){
				a11->data[i*sublength+j] = a0[i*length+j];
				b11->data[i*sublength+j] = b0[i*length+j];
				a22->data[i*sublength+j] = a0[(i+sublength)*length+sublength+j];
				b22->data[i*sublength+j] = b0[(i+sublength)*length+sublength+j];
			} 
		}
			
		//P1 = (a11 + a22)(b11 + b22)
		m_add(a, a, temp1, 0, 0, sublength, sublength);
		m_add(b, b, temp2, 0, 0, sublength, sublength);
		mmm_strassen(temp1, temp2, P1);
				  
		//P2 = (a21 + a22)(b11)
		m_add(a, a, temp1, sublength, 0, sublength, sublength);
		mmm_strassen(temp1, b11, P2);
			  
		//P3 = (a11)(b12 - b22)
		m_sub(b, b, temp1, 0, sublength, sublength, sublength);
		mmm_strassen(a11, temp1, P3);
			  
		//P4 = (a22)(b21 - b11)
		m_sub(b, b, temp1, sublength, 0, 0, 0);
		mmm_strassen(a22, temp1, P4);
			  
		//P5 = (a11 + 12)(b22)
		m_add(a, a, temp1, 0, 0, 0, sublength);
		mmm_strassen(temp1, b22, P5);
			  
		//P6 = (a21 - a11)(b11 + b12)
		m_sub(a, a, temp1, sublength, 0, 0, 0);
		m_add(b, b, temp2, 0, 0, 0, sublength);
		mmm_strassen(temp1, temp2, P6);
			  
		//P7 = (a12 - a22)(b21 + b22)
		m_sub(a, a, temp1, 0, sublength, sublength, sublength);
		m_add(b, b, temp2, sublength, 0, sublength, sublength);
		mmm_strassen(temp1, temp2, P7);
			  
		m_add_full(P1, P4, temp1);
		m_sub_full(temp1, P5, temp2);
		m_add_full(temp2, P7, c11);
			  
		m_add_full(P3, P5, c12);
			  
		m_add_full(P2, P4, c21);
			  
		m_sub_full(P1, P2, temp1);
		m_add_full(temp1, P3, temp2);
		m_add_full(temp2, P6, c22);
		  
		for(i=0; i<sublength; ++i){
			for(j=0; j<sublength; ++j){
				c0[i*length+j] = c11->data[i*sublength+j];		  
				c0[i*length+sublength+j] = c12->data[i*sublength+j];	  
				c0[(i+sublength)*length+j] = c21->data[i*sublength+j];
				c0[(i+sublength)*length+sublength+j] = c22->data[i*sublength+j];
			}
		}	
		
		free(a11); free(a22);
		free(b11); free(b22);
		free(c11); free(c12); free(c21); free(c22);
		free(P1); free(P2); free(P3); free(P4); free(P5); free(P6); free(P7);
		free(temp1); free(temp2);
		
	}  
}
// Strassen Method with Unrolling
void mmm_strassen_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
	int i, j, k;
	int sublength;
	int length = get_matrix_length(a);
	data_t *a0 = get_matrix_start(a);
	data_t *b0 = get_matrix_start(b);
	data_t *c0 = get_matrix_start(c);
  
	if(length <= UNROLL_THRESHOLD){
		m_mul(a, b, c);
		//mmm_kij_omp(a,b,c);
	} 
	else{
		sublength = length/2;
		
		matrix_ptr a11 = new_matrix(sublength);	matrix_ptr a22 = new_matrix(sublength);
		
		matrix_ptr b11 = new_matrix(sublength);	matrix_ptr b22 = new_matrix(sublength);
			  
		matrix_ptr c11 = new_matrix(sublength);	matrix_ptr c12 = new_matrix(sublength);
		matrix_ptr c21 = new_matrix(sublength);	matrix_ptr c22 = new_matrix(sublength);
		
		matrix_ptr P1 = new_matrix(sublength);	matrix_ptr P2 = new_matrix(sublength);
		matrix_ptr P3 = new_matrix(sublength);	matrix_ptr P4 = new_matrix(sublength);
		matrix_ptr P5 = new_matrix(sublength);	matrix_ptr P6 = new_matrix(sublength);
		matrix_ptr P7 = new_matrix(sublength);
			  
		matrix_ptr temp1 = new_matrix(sublength);	matrix_ptr temp2 = new_matrix(sublength);
		  
		for(i=0; i < sublength; i++){
			for(j=0; j < sublength; j+=UNROLL_THRESHOLD){
				a11->data[i*sublength+j] = a0[i*length+j];
				b11->data[i*sublength+j] = b0[i*length+j];
				a22->data[i*sublength+j] = a0[(i+sublength)*length+sublength+j];
				b22->data[i*sublength+j] = b0[(i+sublength)*length+sublength+j];
				
				a11->data[i*sublength+j+1] = a0[i*length+j+1];
				b11->data[i*sublength+j+1] = b0[i*length+j+1];
				a22->data[i*sublength+j+1] = a0[(i+sublength)*length+sublength+j+1];
				b22->data[i*sublength+j+1] = b0[(i+sublength)*length+sublength+j+1];
				
				a11->data[i*sublength+j+2] = a0[i*length+j+2];
				b11->data[i*sublength+j+2] = b0[i*length+j+2];
				a22->data[i*sublength+j+2] = a0[(i+sublength)*length+sublength+j+2];
				b22->data[i*sublength+j+2] = b0[(i+sublength)*length+sublength+j+2];
				
				a11->data[i*sublength+j+3] = a0[i*length+j+3];
				b11->data[i*sublength+j+3] = b0[i*length+j+3];
				a22->data[i*sublength+j+3] = a0[(i+sublength)*length+sublength+j+3];
				b22->data[i*sublength+j+3] = b0[(i+sublength)*length+sublength+j+3];
				
				a11->data[i*sublength+j+4] = a0[i*length+j+4];
				b11->data[i*sublength+j+4] = b0[i*length+j+4];
				a22->data[i*sublength+j+4] = a0[(i+sublength)*length+sublength+j+4];
				b22->data[i*sublength+j+4] = b0[(i+sublength)*length+sublength+j+4];
				
				a11->data[i*sublength+j+5] = a0[i*length+j+5];
				b11->data[i*sublength+j+5] = b0[i*length+j+5];
				a22->data[i*sublength+j+5] = a0[(i+sublength)*length+sublength+j+5];
				b22->data[i*sublength+j+5] = b0[(i+sublength)*length+sublength+j+5];
				
				a11->data[i*sublength+j+6] = a0[i*length+j+6];
				b11->data[i*sublength+j+6] = b0[i*length+j+6];
				a22->data[i*sublength+j+6] = a0[(i+sublength)*length+sublength+j+6];
				b22->data[i*sublength+j+6] = b0[(i+sublength)*length+sublength+j+6];
				
				a11->data[i*sublength+j+7] = a0[i*length+j+7];
				b11->data[i*sublength+j+7] = b0[i*length+j+7];
				a22->data[i*sublength+j+7] = a0[(i+sublength)*length+sublength+j+7];
				b22->data[i*sublength+j+7] = b0[(i+sublength)*length+sublength+j+7];
				
			} 
		}
			
		//P1 = (a11 + a22)(b11 + b22)
		m_add_unroll(a, a, temp1, 0, 0, sublength, sublength);
		m_add_unroll(b, b, temp2, 0, 0, sublength, sublength);
		mmm_strassen(temp1, temp2, P1);
				  
		//P2 = (a21 + a22)(b11)
		m_add_unroll(a, a, temp1, sublength, 0, sublength, sublength);
		mmm_strassen(temp1, b11, P2);
			  
		//P3 = (a11)(b12 - b22)
		m_sub_unroll(b, b, temp1, 0, sublength, sublength, sublength);
		mmm_strassen(a11, temp1, P3);
			  
		//P4 = (a22)(b21 - b11)
		m_sub_unroll(b, b, temp1, sublength, 0, 0, 0);
		mmm_strassen(a22, temp1, P4);
			  
		//P5 = (a11 + 12)(b22)
		m_add_unroll(a, a, temp1, 0, 0, 0, sublength);
		mmm_strassen(temp1, b22, P5);
			  
		//P6 = (a21 - a11)(b11 + b12)
		m_sub_unroll(a, a, temp1, sublength, 0, 0, 0);
		m_add_unroll(b, b, temp2, 0, 0, 0, sublength);
		mmm_strassen(temp1, temp2, P6);
			  
		//P7 = (a12 - a22)(b21 + b22)
		m_sub_unroll(a, a, temp1, 0, sublength, sublength, sublength);
		m_add_unroll(b, b, temp2, sublength, 0, sublength, sublength);
		mmm_strassen(temp1, temp2, P7);
			  
		m_add_full_unroll(P1, P4, temp1);
		m_sub_full_unroll(temp1, P5, temp2);
		m_add_full_unroll(temp2, P7, c11);
			  
		m_add_full_unroll(P3, P5, c12);
			  
		m_add_full_unroll(P2, P4, c21);
			  
		m_sub_full_unroll(P1, P2, temp1);
		m_add_full_unroll(temp1, P3, temp2);
		m_add_full_unroll(temp2, P6, c22);
		  
		for(i=0; i<sublength; i++){
			for(j=0; j<sublength; j+=UNROLL_THRESHOLD){
				c0[i*length+j] = c11->data[i*sublength+j];		  
				c0[i*length+sublength+j] = c12->data[i*sublength+j];	  
				c0[(i+sublength)*length+j] = c21->data[i*sublength+j];
				c0[(i+sublength)*length+sublength+j] = c22->data[i*sublength+j];
				
				c0[i*length+j+1] = c11->data[i*sublength+j+1];		  
				c0[i*length+sublength+j+1] = c12->data[i*sublength+j+1];	  
				c0[(i+sublength)*length+j+1] = c21->data[i*sublength+j+1];
				c0[(i+sublength)*length+sublength+j+1] = c22->data[i*sublength+j+1];
				
				c0[i*length+j+2] = c11->data[i*sublength+j+2];		  
				c0[i*length+sublength+j+2] = c12->data[i*sublength+j+2];	  
				c0[(i+sublength)*length+j+2] = c21->data[i*sublength+j+2];
				c0[(i+sublength)*length+sublength+j+2] = c22->data[i*sublength+j+2];
				
				c0[i*length+j+3] = c11->data[i*sublength+j+3];		  
				c0[i*length+sublength+j+3] = c12->data[i*sublength+j+3];	  
				c0[(i+sublength)*length+j+3] = c21->data[i*sublength+j+3];
				c0[(i+sublength)*length+sublength+j+3] = c22->data[i*sublength+j+3];
															
				c0[i*length+j+4] = c11->data[i*sublength+j+4];		  
				c0[i*length+sublength+j+4] = c12->data[i*sublength+j+4];	  
				c0[(i+sublength)*length+j+4] = c21->data[i*sublength+j+4];
				c0[(i+sublength)*length+sublength+j+4] = c22->data[i*sublength+j+4];
				
				c0[i*length+j+5] = c11->data[i*sublength+j+5];		  
				c0[i*length+sublength+j+5] = c12->data[i*sublength+j+5];	  
				c0[(i+sublength)*length+j+5] = c21->data[i*sublength+j+5];
				c0[(i+sublength)*length+sublength+j+5] = c22->data[i*sublength+j+5];
				
				c0[i*length+j+6] = c11->data[i*sublength+j+6];		  
				c0[i*length+sublength+j+6] = c12->data[i*sublength+j+6];	  
				c0[(i+sublength)*length+j+6] = c21->data[i*sublength+j+6];
				c0[(i+sublength)*length+sublength+j+6] = c22->data[i*sublength+j+6];
				
				c0[i*length+j+7] = c11->data[i*sublength+j+7];		  
				c0[i*length+sublength+j+7] = c12->data[i*sublength+j+7];	  
				c0[(i+sublength)*length+j+7] = c21->data[i*sublength+j+7];
				c0[(i+sublength)*length+sublength+j+7] = c22->data[i*sublength+j+7];
				
			}
		}	
		
		free(a11); free(a22);
		free(b11); free(b22);
		free(c11); free(c12); free(c21); free(c22);
		free(P1); free(P2); free(P3); free(P4); free(P5); free(P6); free(P7);
		free(temp1); free(temp2);
		
	} 
}
// Matrix Addition OMP
void m_add_omp(matrix_ptr a, matrix_ptr b,  matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2)
{
	int i, j;
	int length = get_matrix_length(a);
	int sublength = get_matrix_length(c);
	//long int size = length*length;
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	#pragma omp parallel shared(m1,m2,m3,sublength,chunk,a,b,c,startcol1,startrow1,startcol2,startrow2) private(i,j) num_threads(OMP_THREADS)
	{
		#pragma omp for schedule (dynamic,chunk) nowait
		for(i=0; i < sublength; i++){
			for(j=0; j < sublength; j++){
				m3[i*sublength + j] = m1[(i+startrow1)*length + startcol1 + j] + m2[(i+startrow2)*length + startcol2 + j];
			}
		}
	}
}
// Matrix subtraction OMP
void m_sub_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2)
{
	int i, j;
	int length = get_matrix_length(a);
	int sublength = get_matrix_length(c);
	//long int size = length*length;
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	#pragma omp parallel shared(m1,m2,m3,sublength,chunk,a,b,c,startcol1,startrow1,startcol2,startrow2) private(i,j) num_threads(OMP_THREADS)
	{
		#pragma omp for schedule (dynamic,chunk) nowait
		for(i=0; i < sublength; i++){
			for(j=0; j < sublength; j++){
				m3[i*sublength + j] = m1[(i+startrow1)*length + startcol1 + j] - m2[(i+startrow2)*length + startcol2 + j];
			}
		}
	}
}
// Matrix Addition OMP
void m_add_full_omp(matrix_ptr a, matrix_ptr b,  matrix_ptr c)
{
	int i, j;
	
	int sublength = get_matrix_length(a);
	
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	#pragma omp parallel shared(m1,m2,m3,sublength,chunk) private(i,j) num_threads(OMP_THREADS)
	{
		#pragma omp for schedule(dynamic,chunk) nowait
		for(i=0; i < sublength; i++){
			for(j=0; j < sublength; j++){
				m3[i*sublength + j] = m1[i*sublength + j] + m2[i*sublength + j];
			}
		}
	}
}
//matrix subtraction OMP
void m_sub_full_omp(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
	int i, j;
	
	int sublength = get_matrix_length(a);
	
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	#pragma omp parallel shared(m1,m2,m3,sublength,chunk) private(i,j) num_threads(OMP_THREADS)
	{
		#pragma omp for schedule(dynamic,chunk) nowait
		for(i=0; i < sublength; i++){
			for(j=0; j < sublength; j++){
				m3[i*sublength + j] = m1[i*sublength + j] - m2[i*sublength + j];
			}
		}
	}
}
//matrix addition (sub matrices)
void m_add(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2)
{
	int i, j;
	int length = get_matrix_length(a);
	int sublength = get_matrix_length(c);
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	
	for(i=0; i < sublength; i++){
		for(j=0; j < sublength; j++){
			m3[i*sublength + j] = m1[(i+startrow1)*length + startcol1 + j] + m2[(i+startrow2)*length + startcol2 + j];
		}
	}
}
//matrix addition with unrolling (sub matrices)
void m_add_unroll(matrix_ptr a, matrix_ptr b,  matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2)
{
	int i, j;
	int length = get_matrix_length(a);
	int sublength = get_matrix_length(c);
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	
	for(i=0; i < sublength; i++){
		for(j=0; j < sublength; j+=UNROLL_THRESHOLD){
			m3[i*sublength + j] = m1[(i+startrow1)*length + startcol1 + j] + m2[(i+startrow2)*length + startcol2 + j];
			
			m3[i*sublength + j + 1] = m1[(i+startrow1)*length + startcol1 + j + 1] + m2[(i+startrow2)*length + startcol2 + j + 1];
			m3[i*sublength + j + 2] = m1[(i+startrow1)*length + startcol1 + j + 2] + m2[(i+startrow2)*length + startcol2 + j + 2];
			m3[i*sublength + j + 3] = m1[(i+startrow1)*length + startcol1 + j + 3] + m2[(i+startrow2)*length + startcol2 + j + 3];
			
			m3[i*sublength + j + 4] = m1[(i+startrow1)*length + startcol1 + j + 4] + m2[(i+startrow2)*length + startcol2 + j + 4];
			m3[i*sublength + j + 5] = m1[(i+startrow1)*length + startcol1 + j + 5] + m2[(i+startrow2)*length + startcol2 + j + 5];
			m3[i*sublength + j + 6] = m1[(i+startrow1)*length + startcol1 + j + 6] + m2[(i+startrow2)*length + startcol2 + j + 6];
			m3[i*sublength + j + 7] = m1[(i+startrow1)*length + startcol1 + j + 7] + m2[(i+startrow2)*length + startcol2 + j + 7];
			
		}
	}
}
//matrix subtraction
void m_sub(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2)
{
	int i, j;
	int length = get_matrix_length(a);
	int sublength = get_matrix_length(c);
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	
	for(i=0; i < sublength; i++){
		for(j=0; j < sublength; j++){
			m3[i*sublength + j] = m1[(i+startrow1)*length + startcol1 + j] - m2[(i+startrow2)*length + startcol2 + j];
		}
	}
}
//matrix subtraction with unrolling
void m_sub_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c, int startrow1, int startcol1, int startrow2, int startcol2)
{
	int i, j;
	int length = get_matrix_length(a);
	int sublength = get_matrix_length(c);
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	
	for(i=0; i < sublength; i++){
		for(j=0; j < sublength; j+=UNROLL_THRESHOLD){
			m3[i*sublength + j] = m1[(i+startrow1)*length + startcol1 + j] - m2[(i+startrow2)*length + startcol2 + j];
			
			m3[i*sublength + j + 1] = m1[(i+startrow1)*length + startcol1 + j + 1] - m2[(i+startrow2)*length + startcol2 + j + 1];
			m3[i*sublength + j + 2] = m1[(i+startrow1)*length + startcol1 + j + 2] - m2[(i+startrow2)*length + startcol2 + j + 2];
			m3[i*sublength + j + 3] = m1[(i+startrow1)*length + startcol1 + j + 3] - m2[(i+startrow2)*length + startcol2 + j + 3];
			
			m3[i*sublength + j + 4] = m1[(i+startrow1)*length + startcol1 + j + 4] - m2[(i+startrow2)*length + startcol2 + j + 4];
			m3[i*sublength + j + 5] = m1[(i+startrow1)*length + startcol1 + j + 5] - m2[(i+startrow2)*length + startcol2 + j + 5];
			m3[i*sublength + j + 6] = m1[(i+startrow1)*length + startcol1 + j + 6] - m2[(i+startrow2)*length + startcol2 + j + 6];
			m3[i*sublength + j + 7] = m1[(i+startrow1)*length + startcol1 + j + 7] - m2[(i+startrow2)*length + startcol2 + j + 7];
			
		}
	}
}
//matrix addition
void m_add_full(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
	int i, start, sublength;
	int length = get_matrix_length(a);
	int size = length*length;
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	  
	for(i = 0; i < size; i++)
	{
		m3[i] = m1[i] + m2[i];
	}
 
}
//matrix addition with unrolling
void m_add_full_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
	int i, start, sublength;
	int length = get_matrix_length(a);
	int size = length*length;
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	  
	for(i = 0; i < size; i+=UNROLL_THRESHOLD)
	{
		m3[i] = m1[i] + m2[i];
		
		m3[i+1] = m1[i+1] + m2[i+1];
		m3[i+2] = m1[i+2] + m2[i+2];
		m3[i+3] = m1[i+3] + m2[i+3];
		
		m3[i+4] = m1[i+4] + m2[i+4];
		m3[i+5] = m1[i+5] + m2[i+5];
		m3[i+6] = m1[i+6] + m2[i+6];
		m3[i+7] = m1[i+7] + m2[i+7];
		
	}
 
}
//full matrix subtraction
void m_sub_full(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
	int i;
	int length = get_matrix_length(a);
	int size = length*length;
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	  
	for(i = 0; i < size; i++)
	{
		m3[i] = m1[i] - m2[i];
	}
}
// matrix subtraction with unrolling
void m_sub_full_unroll(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
	int i;
	int length = get_matrix_length(a);
	int size = length*length;
	data_t *m1 = get_matrix_start(a);
	data_t *m2 = get_matrix_start(b);
	data_t *m3 = get_matrix_start(c);
	  
	for(i = 0; i < size; i+=UNROLL_THRESHOLD)
	{
		m3[i] = m1[i] - m2[i];
		
		m3[i+1] = m1[i+1] - m2[i+1];
		m3[i+2] = m1[i+2] - m2[i+2];
		m3[i+3] = m1[i+3] - m2[i+3];
		
		m3[i+4] = m1[i+4] - m2[i+4];
		m3[i+5] = m1[i+5] - m2[i+5];
		m3[i+6] = m1[i+6] - m2[i+6];
		m3[i+7] = m1[i+7] - m2[i+7];
		
	}
}
// matrix multiplication k i j 
void m_mul(matrix_ptr m1, matrix_ptr m2, matrix_ptr mout)
{
	int i,j,k;
	int length = get_matrix_length(m1);
	data_t *mm1 = get_matrix_start(m1);
	data_t *mm2 = get_matrix_start(m2);
	data_t *mm3 = get_matrix_start(mout);
	data_t sum;
	 
	for (k = 0; k < length; k++){
		for (i = 0; i < length; i++){
			sum = mm1[i*length+k];
			for (j = 0; j < length; j++)
				mm3[i*length+j] += sum*mm2[k*length+j];
		}
	}
}


