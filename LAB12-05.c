//q1

void do_child(int fd){
        int in;
        struct flock lock;

        // child가 parent와 sleep없이 동시에 작업하므로, lock 해줘야함
        lock.l_whence=SEEK_SET;  // 동일한 위치(SEEK_SET)
        lock.l_start=0;
        lock.l_len=4;
        lock.l_type=F_WRLCK;
        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, 0, SEEK_SET);
        read(fd, &in, sizeof(int));
        sleep(5);

        in=in+10;
        lseek(fd, 0, SEEK_SET);
        write(fd, &in, sizeof(int));

        lock.l_type=F_UNLCK;
        fcntl(fd, f_SETLK, &lock);

        exit(0);
}

int main(){
        int i, in, fd;
        pid_t pid;

        fd = open("data1", O_RDWR|O_CREAT, 0600);
        scanf("%d", &in);
        write(fd, &in, sizeof(int));

        for(i = 0; i < 3; i++){
                pid = fork();
                if(pid == 0)
                        do_child(fd);
        }

        for(i = 0; i < 3; i++)
                wait(0);

        lseek(fd, 0, SEEK_SET);
        read(fd, &in, sizeof(int));
        printf("%d\n", in);

        exit(0);
}
