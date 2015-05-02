#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
   int data;
   int fibdata;
   struct node* next;
};

/*int fib(int n) {
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fib(n - 1);
      y = fib(n - 2);
      return (x + y);
   }
}*/

int fib(int n) {
	if (n < 2) {
		return (n);
	}
	int x, y;
	x = fib(n - 1);
	y = fib(n - 2);
	return (x + y);
}

void processwork(struct node* p) 
{
//   int n;
//   n = p->data;
   p->fibdata = fib(p->data);
}

struct node* init_list(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;
    
    head = malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i=0; i< N; i++) {
       temp  =  malloc(sizeof(struct node));
       p->next = temp;
       p = temp;
       p->data = FS + i + 1;
       p->fibdata = i+1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[]) {
     double start, end;
     struct node *p=NULL;
     struct node *temp=NULL;
     struct node *head=NULL;
     
	 printf("Process linked list\n");
     printf("  Each linked list node will be processed by function 'processwork()'\n");
     printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);      
 
     p = init_list(p);
     head = p;

     start = omp_get_wtime();
/*     {
        while (p != NULL) {
		   processwork(p);
		   p = p->next;
        }
     }*/

     int iternum = 1;
     int nthreads;
#pragma omp parallel firstprivate(p,iternum) shared(nthreads)
	{
		int tid = omp_get_thread_num();
		int nths = omp_get_num_threads();
		if(tid == 0)
			nthreads = nths;
		#pragma omp barrier
		while (p != NULL) {
			if((FS + iternum) % nthreads == tid){
				printf("{%d}:%dshould do work for [%d]\n",
					tid,iternum,p->data);
				p->fibdata = fib(p->data);
			}
			p = p->next;
			iternum++;
		}
	}

     end = omp_get_wtime();
     printf("Compute Time: %f seconds\n", end - start);
     p = head;
     while (p != NULL) {
//        printf("%d : %d\n",p->data, p->fibdata);
        temp = p->next;
        free (p);
        p = temp;
     }  
	 free (p);


     return 0;
}

