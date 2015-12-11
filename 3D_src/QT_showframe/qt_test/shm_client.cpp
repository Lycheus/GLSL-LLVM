#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include<fstream>
#include <iostream>
#include<iomanip>
using namespace std; 

#define SEGSIZE 30720
void writeshm(int shmid, char *segptr, const char *text);
main(int argc, char *argv[])
{
	key_t key;
	int   shmid, cntr;
	char  *segptr;
	char buffer [10];
	ifstream fin;
	string ansi;


	fin.open(argv[1]);


	/* Create unique key via call to ftok() */
	key = ftok(".", 'S');

	/* Open the shared memory segment - create if necessary */
	if((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0666)) == -1) 
	{
		printf("Shared memory segment exists - opening as client\n");

		/* Segment probably already exists - try as a client */
		if((shmid = shmget(key, SEGSIZE, 0)) == -1) 
		{
			perror("shmget");
			exit(1);
		}
	}
	else
	{
		printf("Creating new shared memory segment\n");
	}

	/* Attach (map) the shared memory segment into the current process */
	if((segptr = (char *)shmat(shmid, 0, 0))==(char *)-1)
	{
		perror("shmat");
		exit(1);
	}
	pid_t child_pid;
  if ((child_pid=fork()) < 0) {
        exit(1);
  }
  char prog[]="./qt_test";
  if (child_pid == 0) // child
     execl(prog,prog,NULL,NULL,NULL);

	cout << "test" << endl;
	if(fin.is_open()){
		int c=0,num=1;
		string t;
		while(getline(fin,ansi,'\n')){
			sprintf (buffer, "%u", atol(ansi.c_str()));//for debug
			t.append(ansi + ",");
			c++;
//			cout << num << " : "<< buffer << endl;
			if(c==100){
				sprintf (buffer, "%d,", num++);
//				t.insert(0,buffer);
				writeshm(shmid, segptr, t.c_str());
				c=0;
				t="";
			}
		}
//		sprintf (buffer, "%d,", num++);
//		t.insert(0,buffer);
//		cout << "final: " << t << endl;
		writeshm(shmid, segptr, t.c_str());
	}else{
		std::cout << argv[1] << "File Miss" << endl;
	}
//Shared memory segment marked for deletion
	printf("file is finished...\n");
//	while(1) usleep(50);
	while(1){
		usleep(50);
		if(*(segptr)=='f')
			break;
		else{
                      cout << "wait for reading" << endl;
                        usleep(50);
                }
	}
	strcpy(segptr, "q");
	sleep(2);
	shmctl(shmid, IPC_RMID, 0);
	fin.close();
}

void writeshm(int shmid, char *segptr, const char *text)
{
	while(1){
		if((*(segptr)=='f'))
			break;
		else{
//			cout << "wait for reading" << endl;
			usleep(50);
		}
	}
	memset(segptr,'\0',SEGSIZE);
	strcpy(segptr, text);
//	cout << segptr << endl;
	cout << "Write data is Done...\n";
}

