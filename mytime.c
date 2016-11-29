#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/times.h> 
#include <sys/types.h> 
#include <stdint.h>
#include <time.h>
int fd[2];
void time_calculating();

main(int argc, char** argv){
		struct timeval startwtime,endwtime;
		double seq_time;
		struct tms end_time;
		
		char *mypath=argv[1];
		
		int pid,status,wait_res,pipe_res;
		
		pipe_res=pipe(fd);
		
		if(pipe_res<0){
			printf("Can't open the pipe. Exiting...\n");
			exit(0);
		}
		
		printf("Forking Process \n");
		//edw tha prepei na xekinaei i metrisi tou xronou
		gettimeofday (&startwtime, NULL);
		
		pid=fork();
		
		
		
		if(pid==0){
			//CHILD PROCESS
			
			time_calculating();
			execv(argv[1],&argv[1]);
			
		}else if(pid>0){
			//MOTHER PROCESS
			long double start_child_utime,end_child_utime,start_child_stime,end_child_stime,stime_diff,utime_diff;
			
			wait_res=wait(&status);
			
			
			if(wait_res!=-1){
				clock_t t1,t2,t3,t4;
				//edw tha tipwnetai o xronos cpu kai real time 
				close(fd[1]);
				
				read(fd[0],&t1,sizeof(clock_t));
				read(fd[0],&t2,sizeof(clock_t));
				read(fd[0],&t3,sizeof(clock_t));
				read(fd[0],&t4,sizeof(clock_t));
				
				start_child_utime = t1/(CLOCKS_PER_SEC/1000000);
				start_child_stime = t3/(CLOCKS_PER_SEC/1000000);
				//turnig the times to seconds
				start_child_utime=start_child_utime/1000000;
				start_child_stime=start_child_stime/1000000;
								
				close(fd[0]);
				
				times(&end_time);
				
				end_child_utime = end_time.tms_cutime /(CLOCKS_PER_SEC/1000000);
				end_child_stime = end_time.tms_cstime /(CLOCKS_PER_SEC/1000000);
				//turnig the times to seconds
				end_child_utime=end_child_utime/1000000;
				end_child_stime=end_child_stime/1000000;
				
				
				
				
				utime_diff = end_child_utime - start_child_utime;
				stime_diff = end_child_stime - start_child_stime;
				
				printf("Ending");
				gettimeofday (&endwtime, NULL);
				seq_time = (double)( ( endwtime.tv_usec - startwtime.tv_usec ) / 1.0e6 + endwtime.tv_sec - startwtime.tv_sec );
				printf("\n\nReal Wall clock time = %f seconds \n\n", seq_time);
				
				printf("User Time : %Lf seconds \n\n",utime_diff);
				printf("System Time : %Lf seconds \n\n",stime_diff);
				
				
				
			}else{
				printf("Error on wait! \n");
			}
		}
		
	
	
	
	
	




}

void time_calculating(){
	struct tms start_time;
			
	times(&start_time);
	
	close(fd[0]);
	
	write(fd[1], &(start_time.tms_cutime),sizeof(clock_t));
	write(fd[1], &(start_time.tms_utime),sizeof(clock_t));
	write(fd[1], &(start_time.tms_cstime),sizeof(clock_t));
	write(fd[1], &(start_time.tms_stime),sizeof(clock_t));
	
	close(fd[1]);
	

}
