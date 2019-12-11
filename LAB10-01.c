//q1
void do_child(void)
{
        printf("%ld, %ld, %ld\n", getpid(), getpgrp(), getsid(getpid()));
        exit(0);
}


int main(int argc, char **argv)
{
        int i = 0;
        int N = 0;
        pid_t pid;

        N = atoi(argv[1]);

        for(i = 0; i < N; i++)
        {
                pid = fork();
                if(pid == 0)
                {
                        do_child();
                }
        }
        for(i = 0; i < N; i++)
        {
                 wait(0);
        }

        return 0;
}


//q2
void do_child(int fd)
{
        char buf[3] = {0};
        buf[2] = '\0';
        read(fd, buf, 2);
        printf("%s\n", buf);
        exit(0);
}


int main(int argc, char **argv)
{
        int fd = open(argv[1], O_RDWR);
        if(fd == -1)
        {
                perror("");
                return 0;
        }
        int N = atoi(argv[2]);
        int i = 0;
        pid_t pid;
        for(i = 0; i < N; i++)
        {
                pid = fork();
                if(pid == 0)
                {
                        do_child(fd);
                }

        }
        for(i = 0; i < N; i++)
        {
                if(pid > 0)  wait(0);
        }

        return 0;
}

//q3
int main(int argc, char **argv)
{
        int i;
        for(i = 1; i < argc; i++)  // if exec, not include '1st argv = file name'
        {
                printf("%s\n", argv[i]);
        }

        return 0;
}


//q3-1
int main()
{
        execl("./test1", ,"test1", "abc", "def", "ghi", (char*)0);

        return 0;
}


//q3-2
int main()
{
        execlp("test2", "abc", "def", "ghi", (char*)0);

        return 0;
}


//q3-3
int main()
{
        char *const av[] = {"abc", "def", "ghi", (char*)0};  // parameters vector
          // const char *  : fix address    char *const  : fix memory
        execv("./test1", av);
        printf("exec failed test1\n");
        exit(1);

        return 0;
}


//q3-4
int main()
{
        char *const av[] = {"abc", "def", "ghi", (char*)0};  // parameters vector
          // const char *  : fix address    char *const  : fix memory
        execvp("test2", av);
        printf("exec failed test1\n");
        exit(1);

        return 0;
}
