////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef struct Process {
  int number;
  int tau;
  float alpha;
  int * times;
} Process;

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void shortest_job_first(Process * processes[], int ticks, int n_processes);
void reset_processes(Process * processes[], int n_processes);
void shortest_job_first_live(Process * processes[], int ticks, int n_processes);
void merge_time(Process * arr[], int l, int m, int r, int index);
void merge_sort_time(Process * arr[], int l, int r, int index);
void merge_tau(Process * arr[], int l, int m, int r);
void merge_sort_tau(Process * arr[], int l, int r);
void merge_process_number(Process * arr[], int l, int m, int r);
void merge_sort_process_number(Process * arr[], int l, int r);

////////////////////////////////////////////////////////////////////////////////
//Main Entry Point
int main(){
	
  FILE* file = fopen ("data.txt", "r");
  int i;
  int j;  
  int ticks = 0;
  int proc = 0;

  fscanf (file, "%d", &ticks); 
  fscanf (file, "%d", &proc); 

  Process * processes[proc];

  for(i = 0; i < proc; i++){
    processes[i] = malloc(sizeof(Process));
    processes[i]->times = malloc(ticks*sizeof(int));
  	fscanf (file, "%d", &processes[i]->number);
  	fscanf (file, "%d", &processes[i]->tau);
  	fscanf (file, "%G", &processes[i]->alpha);
  	
  	for(j = 0; j < ticks; j++){
  		fscanf (file, "%d", &processes[i]->times[j]); 
  	}
  }  
  fclose (file);

  shortest_job_first(processes, ticks, proc);
  reset_processes(processes, proc);
  shortest_job_first_live(processes, ticks, proc);

  for(i = 0; i < proc; i++){    
    free(processes[i]->times);
    free(processes[i]);
  }      

}

void shortest_job_first(Process * processes[], int ticks, int n_processes){
  int i, j, k, wait = 0, time = 0;

  printf("==Shortest-Job-First==\n");

  for(i = 0; i < ticks; i++){
  	printf("Simulating the %dth round of processes @ time %d: \n", i, time);
  	
    merge_sort_time(processes, 0, n_processes-1, i);

    for(j = 0; j < n_processes; j++){
        printf("    Process %d took %d.\n", processes[j]->number, processes[j]->times[i]); 	

        time += processes[j]->times[i];

        if(j != n_processes -1)
        	wait += processes[j]->times[i];
		
    }
  }
  printf("Turnaround Time: %d \n", time+wait);
  printf("Waiting time: %d \n\n", wait);
}

void reset_processes(Process * processes[], int n_processes){
  merge_sort_process_number(processes,0,n_processes-1);
}

void shortest_job_first_live(Process * processes[], int ticks, int n_processes){
  int i, j, k, wait = 0, time = 0, error = 0;

  printf("==Shortest-Job-First Live==\n");

  for(i = 0; i < ticks; i++){
      printf("Simulating the %dth round of processes @ time %d: \n", i, time);
      
      merge_sort_tau(processes, 0, n_processes-1);  

      for(j = 0; j < n_processes; j++){
          
          printf("    Process %d was estimated for %d and took %d.\n", processes[j]->number, 
            processes[j]->tau, processes[j]->times[i]); 	

          time += processes[j]->times[i];
          error += abs(processes[j]->tau - processes[j]->times[i]);
          
          processes[j]->tau =  (((1 - processes[j]->alpha)*processes[j]->tau) + 
            (processes[j]->alpha*processes[j]->times[i]));

          if(j != n_processes -1)
          	wait += processes[j]->times[i];
  		
      }
  }
  printf("Turnaround Time: %d \n", time+wait);
  printf("Waiting time: %d \n", wait);
  printf("Estimation Error: %d \n\n", error);
  
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge_time(Process * arr[], int l, int m, int r, int index){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    Process * L[n1], * R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i]->times[index] <= R[j]->times[index])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void merge_sort_time(Process * arr[], int l, int r, int index){
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        merge_sort_time(arr, l, m, index);
        merge_sort_time(arr, m+1, r, index);
 
        merge_time(arr, l, m, r, index);
    }
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge_tau(Process * arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    Process * L[n1], * R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i]->tau <= R[j]->tau)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void merge_sort_tau(Process * arr[], int l, int r){
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        merge_sort_tau(arr, l, m);
        merge_sort_tau(arr, m+1, r);
 
        merge_tau(arr, l, m, r);
    }
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge_process_number(Process * arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    Process * L[n1], * R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i]->number <= R[j]->number)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void merge_sort_process_number(Process * arr[], int l, int r){
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        merge_sort_process_number(arr, l, m);
        merge_sort_process_number(arr, m+1, r);
 
        merge_process_number(arr, l, m, r);
    }
}
