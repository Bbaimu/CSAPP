#include<sys/types.h>
#include<linux/sem.h>
#include<linux/shm.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<time.h>

int SAid;
int SOid;
int mutexid;
int main(int argc,char argv[])
{
	struct sembuf P,V;
	union semun arg;
	SAid = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	SOid = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	mutexid = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	arg.val = 0;
	if(semctl(SAid,0,SETVAL, arg)==-1)
             perror("semctl setval error");
	arg.val = 0;
	if(semctl(SOid,0,SETVAL,arg)==-1)
               perror("semctl setval error");
	arg.val = 1;
	if(semctl(mutexid,0,SETVAL,arg)==-1)
             perror("setctl setval error");
	P.sem_num = 0;
	P.sem_op = -1;
	P.sem_flg = SEM_UNDO;
	V.sem_num = 0;
	V.sem_op = 1;
	V.sem_flg = SEM_UNDO;
	if(fork()==0)
	{
            int i=0;
            while(i<2)
           {
		semop(mutexid,&P,1);
		printf("father put an apple to pan\n");
                i++;
		semop(SAid,&V,1);
		sleep(1);
            }
	    printf("father put is over\n");
	    exit(0);
	}
	else
	{
	   if(fork()==0)
	    {
                int j=0;
                while(j<2)
                {
	            semop(mutexid,&P,1);
        	    printf("mother put an orange to the pan\n");
		    j++;		  
                    semop(SOid,&V,1);
		    sleep(1);
                }
	        printf("mother put is over\n");
	        exit(0);	     }
	     else
	     {
		   if(fork()==0)
		   {
                       int k=0;
                       while(k<2)
                       {
		            semop(SOid,&P,1);					                                           printf("Son get an orange from the pan\n");
                            k++;
			    semop(mutexid,&V,1);
                            printf("Son eat an orange!\n");					                            sleep(3);
			}               
		       exit(0);
		    }
	            else
		    {
			if(fork()==0)
	                 {
                               int q=0;
                               while(q<2)
                               {
 				     semop(SAid,&P,1);				                                                    printf("Daughter get an apply from the pan\n");
                                     q++;
				     semop(mutexid,&V,1);
                                     printf("Duaghter eat an apple!\n");			                                     sleep(3);
                                }				                                exitt(0);
                           }
                     }
                }
           } 
	   wait(0);
           wait(0);
           wait(0);
           wait(0);
	  semctl(SOid,IPC_RMID,0);
          semctl(SAid,IPC_RMID,0);
	  semctl(mutexid,IPC_RMID,0);
          exit(0);
 }
