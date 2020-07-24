#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define max_str 200
int block_num=0;

struct swinfo {
	char block[4][max_str]; // Block name, parameter save
	time_t time; // start time save
	char reason[max_str]; // reason save
	int pid;
	int count; // restart count save
	int live;
};
struct swinfo arr_file[10]; // 구조체 배열 10개 선언 

void heel_block(int n);
void block_init(void);
int read_file(void);

int read_file(void)
{
	printf("file read start..\n");
	FILE* fp;
	fp = fopen("swinfo.txt", "r"); // file open

	if (fp < 0) { 
		printf("file not found \n");
		return -1;
	}
	char buff[max_str];
	
	while (1)
	{
		if (fgets(buff, max_str, fp) == NULL) break;
		
		int i, j=0, cnt=0;
		for (i = 0; buff[i]; i++)
		{
			if (buff[i] == ';')
			{
				cnt++;
				i++;
				j = i;
			}
			arr_file[block_num].block[cnt][i - j] = buff[i];
		}
		block_num++;
	}
	fclose(fp); // file close
	printf("file read finish!!\n");
	
	int i;
	for(i=0; i<block_num; i++)
		printf("arr_file[%d] - %s %s %s %s\n", i, arr_file[i].block[0], arr_file[i].block[1], arr_file[i].block[2], arr_file[i].block[3]);
}

void block_init(void)
{
	printf("\nblock init start..\n");
	int i;
	for (i = 0; i < block_num; i++) // block_num만큼 구조체 배열 초기화 
	{
		arr_file[i].count = 0;
		arr_file[i].live = 0; // block is die 
		arr_file[i].time = 0;
		strcpy(arr_file[i].reason, "Init");
	}
	printf("block init finish!!\n\n");
}

void heel_block(int n)
{
	pid_t pid = fork();

	if (pid == 0) // child process
	{
		execl(arr_file[n].block[0], arr_file[n].block[0], arr_file[n].block[1], arr_file[n].block[2], arr_file[n].block[3], (char *)0);
		return;
	}
	arr_file[n].pid = pid; // save child pid
	arr_file[n].count++; // save child count
	arr_file[n].time = time(NULL);
	arr_file[n].live = 1; // block = live
	
}

void handler(int signum)
{
	int pid;
	int status;
	char buff[max_str]="";

	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		if(WIFEXITED(status))
			sprintf(buff, "exit (%d)", WEXITSTATUS(status)); // buff에 "exit(번호)" 저장 

		if(WIFSIGNALED(status))
			sprintf(buff, "signal (%d, %s)", WTERMSIG(status), strsignal(WTERMSIG(status))); // buff에 "exit(번호, 이유)" 저장 
		// WTERMSIG(status) => signal number
		// strsignal() => signal이 가리키는 이름을 문자열로 반환 

		int i;
		for(i=0; i<block_num; i++) // find child pid
		{
			if(arr_file[i].pid == pid) // 반환된 pid로 몇번째 블록인지 찾기 
			{
				arr_file[i].live = 0; // block die
				strcpy(arr_file[i].reason, buff); // save child die reason
			}
		}
	}
}

int main(void)
{
	if(read_file()==-1) // read file
	{
		printf("error - read file\n");
		return -1;
	}
	
	block_init(); // block init

	struct sigaction act;
	sigset_t set;

	sigemptyset(&(act.sa_mask)); // sigaction init
	sigaddset(&(act.sa_mask), SIGCHLD); // sigaction set add SIGCHLD 
	act.sa_handler = handler; // handler 등록 
	sigaction(SIGCHLD, &act, NULL);

	while (1)
	{
		int i;
		for (i = 0; i < block_num; i++) // block_num개수만큼 반복 
		{
			if (arr_file[i].live == 0) // 블록이 죽었을 때 실행 
			{
				heel_block(i); // heel block
				printf("%d block - count : %d, pid : %d, reason : %s  -  time : %s",
					i, arr_file[i].count, arr_file[i].pid, arr_file[i].reason, ctime(&arr_file[i].time)); // block information print
			}
		}
		sleep(1);
	}
	return 0;
}
