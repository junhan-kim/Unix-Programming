//q1
void catchUsr1(int signo)
{
        printf("child process ID : %ld\n", getpid());
}

void do_child(int i)
{
        pause();
        exit(i);
}

int main()
{
        static struct sigaction act;  // main에서 sigaction을 하더라도 fork를 했을
때 copy됨
        act.sa_handler = catchUsr1;
        sigaction(SIGUSR1, &act, NULL);

        pid_t pid[3];
        int i, status, N = 3;
        for(i = 0; i < N; i++)
        {
                pid[i] = fork();
                if(pid[i] == 0)
                {
                        do_child(i);
                }
        }
        for(i = 0; i < N; i++)
        {
                sleep(1);
                kill(pid[i], SIGUSR1);
        }
        int k;
        for(i = 0; i < N; i++)
        {
                k = wait(&status);
                if(WIFEXITED(status))
                {
                        printf("ID : %ld, exit status : %d\n", k, WEXITSTATUS(status));
                }
        }

        exit(0);
}

//q2
void catchInt(int signo){
}


void do_child(int i, int *cid){
        int j;

        pause();
        // 병렬 수행을 해야 하는 경우  kill 부분을 여기에 넣음으로써, 
        // 역순으로 실행하되,  병렬 수행이 가능하다.
        for(j = 0; j < 5; j++){
                printf("child sleep : %d\n", getpid());
                sleep(1);
        }

        // kill을 여기에 넣는 경우는 역순으로, 순차적으로 처리하기 위함이다.
        // 즉 일종의 lock.require(), lock.release() 와 같은 역할을 한다.
        if(i != 0) kill(cid[i-1], SIGUSR1);
        // 프로세스의 배열은 pid_t *cid로 받고, 각 프로세스의 ID역할을 하는 i를 사용한다.
        // i는 fork 시점에서 고정되므로 각 자식 프로세스의 ID로서 활용할 수 있다.

        exit(i);
}


int main(){
        int i, k, status;
        pid_t pid[NUM];

        static struct sigaction act;
        act.sa_handler = catchInt;
        sigaction(SIGUSR1, &act, NULL);


        for(i = 0; i < NUM; i++){
                pid[i] = fork();
                if(pid[i] == 0){
                        do_child(i, pid);
                }
        }

        // 반드시 sleep 필요.  마지막 프로세스에서 pause가 실행되기전 kill이 될 가능성이 있음.
        // 그럼 아무것도 안하고 멈춰있음.        
        sleep(1);
        kill(pid[NUM-1], SIGUSR1);  // 제일 마지막 프로세스에 시그널


        // 이 부분에서도 매우 흥미로운 현상이 벌어짐.
        // status에 대한 printf가  다음 프로세스의 첫번째 printf 보다 느리게 나온>다.
        // 시그널을 보내서 다음 프로세스의 pause가 풀리는 과정이,  exit 후의 status 출력보다 빠르다.
        for(i = 0; i < NUM; i++){
                k = wait(&status);
                if(WIFEXITED(status)){
                        printf("%d : %d\n", k, WEXITSTATUS(status));
                }
                if(WIFSIGNALED(status)){
                        printf("%d : %d\n", k, WTERMSIG(status));
                }
        }

        exit(0);
}


