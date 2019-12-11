//q1
int main(int argc, char **argv)
{
        pid_t pid;
        int status;
        int N = 3;
        int i;
        for(i = 0; i < N; i++)
        {
                pid = fork();
                if(pid == 0)
                {
                        if(i == 0)
                                execl("./q1-1", "q1-1", "0", "abcd", (char*)0);
                        else if(i == 1)
                                execl("./q1-2", "q1-2", "1", (char*)0);
                        else if(i == 2)
                                execl("./q1-3", "q1-3", "2", (char*)0);
                }
        }
        for(i = 0; i < N; i++)
        {
                wait(&status);
                if(WIFEXITED(status))
                {
                        printf("%d\n", WEXITSTATUS(status));
                }
        }

        exit(0);
}


//q1-1
int main(int argc, char **argv)
{
        int i;
        for(i = 0; i < 5; i++)
        {
                printf("%s\n", argv[2]);
                sleep(1);
        }
        int tmp = atoi(argv[1]);
        exit(tmp);
}


//q1-2
int main(int argc, char **argv)
{
        int i;
        for(i = 0; i < 5; i++)
        {
                printf("%ld, %ld, %ld\n", getpid(), getpgrp(), getsid(getpid()));
                sleep(1);
        }
        int tmp = atoi(argv[1]);
        exit(tmp);
}


//q1-3
int main(int argc, char **argv)
{
        int fd = open("data", O_RDONLY);

        int i;
        for(i = 0; i < 10; i++)
        {
                char c[2];
                c[1] = '\n';
                int ret = read(fd, &c, sizeof(char));
                write(1, &c, ret + 1);
                sleep(1);
        }
        int tmp = atoi(argv[1]);
        exit(tmp);
}


//q2
int main(int argc, char **argv)
{
        int n;
        pid_t pid;
        int status;
        int pidArr[3] = {0};
        int N = 3;
        int i;
        for(i = 0; i < N; i++)
        {
                pid = fork();
                pidArr[i] = pid;
                if(pid == 0)
                {
                        if(i == 0)
                                execl("./q1-1", "q1-1", "0", "abcd", (char*)0);
                        else if(i == 1)
                                execl("./q1-2", "q1-2", "1", (char*)0);
                        else if(i == 2)
                                execl("./q1-3", "q1-3", "2", (char*)0);
                }
        }
        int cnt = 0;
        while(1)
        {
                for(i = N - 1; i >= 0; i--)
                {
                        // 종료 이전 : 0 반한, 종료 이후 첫 waitpid : 0보다 큰 값 반환, 그 이후 : -1 반환
                        n = waitpid(pidArr[i], &status, WNOHANG);
                        printf("%d ", n);
                        if(n > 0 && WIFEXITED(status))
                        {
                                printf("%d\n", WEXITSTATUS(status));
                                cnt++;
                        }
                }
                printf("\n");
                if(cnt >= 3) exit(0);
                sleep(1);
        }
}