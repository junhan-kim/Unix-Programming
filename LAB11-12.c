//HW3
void catchUsr2(int signo){
}



int main(void){
        char in[50], *res[20]={0};
        int i, status, pid, pid_b=0;

        static struct sigaction act;
        act.sa_handler = SIG_IGN;
        sigaction(SIGINT, &act, NULL);
        static struct sigaction act1;
        act1.sa_handler = SIG_DFL;
        sigaction(SIGUSR1, &act1, NULL);
        static struct sigaction act2;
        act2.sa_handler = catchUsr2;
        sigaction(SIGUSR2, &act2, NULL);

        if(pid_b = fork() == 0){
                execlp("backup_m", "backup_m", (char*)0);
                exit(0);
        }

        while (1){
                printf("> ");
                gets(in);
                if (in[0]=='\0')
                        continue;
                i=0;
                res[i]=strtok(in, " ");
                while (res[i]){
                        res[++i]=strtok(NULL, " ");
                }

                if (strcmp(res[0], "exit")==0){
                        pause();
                        kill(pid_b, SIGUSR1);
                        waitpid(pid_b, &status, 0);
                        printf("BACKUP 종료 확인\n");
                        exit(0);
                }
                else if (strcmp(res[0], "cd_m")==0){
                        chdir(res[1]);
                }
                else{
                        if ((pid=fork())==0){
                                execvp(res[0], res);
                                exit(0);
                        }
                        else{
                                waitpid(pid, &status, 0);
                        }
                }
        }
        return 0;
}
                                                        

//backup
char dname[100];

void catchalarm(int signo);

int backup(const char *name, const struct stat *status, int type){
        char temp[100];
        int fd1, fd2, i;

        if(FTW_F){
                strcpy(temp, dname);
                strcat(temp, "/");
                int len1 = strlen(dname);
                strcat(temp, name + len1- 4);
                int len2 = strlen(temp);
                for(i = len1 + 1; i < len2; i++){
                        if(temp[i] == '/')
                                temp[i] = '_';
                }
                fd1 = open(name, O_RDONLY);
                fd2 = open(temp, O_WRONLY | O_CREAT | O_TRUNC, 0600);
                char buf[512] = {0};
                int ret = 512;
                while(ret == 512){
                        ret = read(fd1, buf, 512);
                        write(fd2, buf, ret);
                }
        }

        return 0;
}


// q1_c
int main(){
        char f[2][3] = {"f1", "f2"};
        int fd[2], num;

        fd[0] = open(f[0], O_WRONLY);  // write
        fd[1] = open(f[1], O_RDONLY);  // read
        while(1){
                scanf("%d", &num);
                if(num == -1)  exit(0);
                write(fd[0], &num, sizeof(int));
                read(fd[1], &num, sizeof(int));
                printf("%d\n", num);
        }
}


//q1_s
// 서버 (먼저 실행)
int main(){
        char f[2][3] = {"f1", "f2"};
        int fd[2], num;

        // 서버에서 mkfifo
        mkfifo(f[0], 0600);
        mkfifo(f[1], 0600);
        // fd와 f 맞춰서 순서대로 open
        fd[0] = open(f[0], O_RDONLY);  // read
        fd[1] = open(f[1], O_WRONLY);  // write
        while(1){
                read(fd[0], &num, sizeof(int));
                if(num == -1) exit(0);
                num += 8;
                write(fd[1], &num, sizeof(int));
        }
}


// q2_c
int main(int argc, char **argv){
        char f[6][3] = {"f1", "f2", "f3", "f4", "f5", "f6"};
        int fd[2], i, k, num;

        k = atoi(argv[1]);
        fd[0] = open(f[k], O_WRONLY);  // write
        fd[1] = open(f[k+3], O_RDONLY);  // read
        for(i = 0; i < 5; i++){
                scanf("%d", &num);
                write(fd[0], &num, sizeof(int));
                read(fd[1], &num, sizeof(int));
                printf("%d\n", num);
        }
        exit(0);
}


//q2_s
int main(){
        // 6개의 fifo
        char f[6][3] = {"f1", "f2", "f3", "f4", "f5", "f6"};
        int fd[6], i, k, num;//, cnt=3;
        fd_set master, set;
        struct timeval tv;

        for(i = 0; i < 6; i++){
                mkfifo(f[i], 0600);
        }
        for(i = 0; i < 3; i++){
                fd[i] = open(f[i], O_RDONLY);   // read
                fd[i+3] = open(f[i+3], O_WRONLY);  // write
        }
        FD_ZERO(&master);
        for(i = 0; i < 3; i++){
                FD_SET(fd[i], &master);
        }
        tv.tv_sec = 10;
        while(set = master, select(fd[2]+1, &set, NULL, NULL, &tv) > 0){
                for(i = 0; i < 3; i++){
                        if(FD_ISSET(fd[i], &set)){
                                // CLR 부분 주석 처리하고 >= 으로 바꾸면,
                                // 빈 파이프에 write  ->  SIGPIPE(종료)
                                if(k = read(fd[i], &num, sizeof(int)) > 0){
                                        num += 8;
                                        write(fd[i+3], &num, sizeof(int));
                                }
                                // 클라이언트 종료 -> 해당 파이프에서 계속 0 반환
                                // select > 0 이 계속 유지되어 무한 루프 -> 타임아웃 무한 >갱신
                               if(k == 0){
                                        FD_CLR(fd[i], &master);
                                        // 클라이언트가 모두 종료했을때 바로 꺼지게 함
                                        //cnt--;
                                        //if(cnt == 0) exit(0);
                                }
                        }
                }
                // 타임아웃 초기화 x  ->  타임아웃 발동 x
                tv.tv_sec = 10;
        }
        exit(0);
}


//q3
// proc[0] <---pipe[0]---> proc[1] <---pipe[1]---> proc[2] <---pipe[2]---> proc[3]
int main(int argc, char **argv){
        char f[3][3] = {"f1", "f2", "f3"};
        int fd[2], i, k, a, pid;

        for(i = 0; i < 3; i++){
                mkfifo(f[i], 0600);
        }
        k = atoi(argv[1]);
        pid = getpid();
        // 데드락 걱정 x
        if(k > 0) fd[0] = open(f[k-1], O_RDONLY);
        if(k < 3) fd[1] = open(f[k], O_WRONLY);

        if(k > 0) read(fd[0], &a, sizeof(char));
        for(i = 0; i < 5; i++){
                sleep(1);
                printf("%d\n", pid);
        }
        if(k < 3) write(fd[1], &a, sizeof(char));

        exit(0);
}
