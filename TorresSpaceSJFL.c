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
void shortest_job_first_live(Process * processes[], int ticks, int n_processes);

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
  	for(j = 0; j < n_processes; j++){        
       	for(k = j+1; k < n_processes; k++){
           	if(processes[k]->times[i] < processes[j]->times[i]){
              Process * temp;
           		temp = processes[j];
      				processes[j] = processes[k];
      				processes[k] = temp;
    		    }
        }
    }

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


void shortest_job_first_live(Process * processes[], int ticks, int n_processes){
  int i, j, k, wait = 0, time = 0, error = 0;

  printf("==Shortest-Job-First Live==\n");

  for(i = 0; i < ticks; i++){
      printf("Simulating the %dth round of processes @ time %d: \n", i, time);
      for(j = 0; j < n_processes; j++){        
          for(k = j+1; k < n_processes; k++){
             	if(processes[k]->tau < processes[j]->tau){
                Process * temp;
                temp = processes[j];
                processes[j] = processes[k];
                processes[k] = temp;
              }
          }
      }
  

      for(j = 0; j < n_processes; j++){
          
          printf("    Process %d was estimated for %d and took %d.\n", processes[j]->number, 
            processes[j]->tau, processes[j]->times[i]); 	

          time += processes[j]->times[i];
          error += abs(processes[j]->tau - processes[j]->times[i]);
          
          processes[j]->tau =  (((1 - processes[j]->alpha)*processes[j]->tau) + 
            (processes[j]->alpha*processes[j]->times[i]));

          if(i != n_processes -1)
          	wait += processes[j]->times[i];
  		
      }
  }
  printf("Turnaround Time: %d \n", time+wait);
  printf("Waiting time: %d \n", wait);
  printf("Estimation Error: %d \n\n", error);
  
}