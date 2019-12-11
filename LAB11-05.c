//q1
void do_child(int p[2]){
        int num, i;

        close(p[1]);
        while(1){
                read(p[0], &num, sizeof(int));   // read 로 blocking
                if(num == -1)  exit(0);
                printf("num : %d, pid : %ld\n", num, getpid());
        }
}

int main(){
        int i, num, p[3][2];

        for(i = 0; i < 3; i++){
                pipe(p[i]);    // 자식에게 pipe를 복사하기 이전에 다른 작업 x  (close 등)
                if(fork() == 0){
                        do_child(p[i]);
                }
        }
        for(i = 0; i < 3; i++){
                close(p[i][0]);
        }
        for(i = 0; i < 12; i++){
                scanf("%d", &num);
                write(p[i%3][1], &num, sizeof(int));   // 모듈로 3
        }
        num = -1;
        for(i = 0; i < 3; i++){
                write(p[i][1], &num, sizeof(int));   // -1 각각 보내놓고
        }
        for(i = 0; i < 3; i++){
                wait(0);  // wait
        }
        exit(0);
}


//q2
// [proc 0] <---pipe 0---> [proc 1] <---pipe 1---> [proc 2]

void child(int p[2][2], int id){
        char a = 'a';
        int i;
        int pid = getpid();

        // start 제외하고 sleep
        if(id < 2) read(p[id][0], &a, sizeof(char));
        for(i = 0; i < 5; i++){
                sleep(1);
                printf("%d\n", pid);
        }
        // end 제외하고 wake
        if(id > 0) write(p[id-1][1], &a, sizeof(char));
        exit(0);
}


int main(){
        int i;
        int p[2][2];

        pipe(p[0]);
        pipe(p[1]);
        for(i = 0; i < 3; i++){
                if(fork() == 0){
                        child(p, i);
                }
        }
        for(i = 0; i < 3; i++){
                wait(0);
        }
        exit(0);
}
                       

// server_client
#define MSGSIZE 6

char *msg1 = "hello";
char *msg2 = "bye!!";

void parent(int p[3][2]){
        char buf[MSGSIZE];
        fd_set set, master;
        int i;

        for(i = 0; i < 3; i++)
                close(p[i][1]);

        FD_ZERO(&master);
        for(i = 0; i < 3; i++)
                FD_SET(p[i][0], &master);
        while(set = master, select(p[2][0]+1, &set, NULL, NULL, NULL) > 0){
                for(i = 0; i < 3; i++){
                        if(FD_ISSET(p[i][0], &set)){
                                if(read(p[i][0], buf, MSGSIZE)>0)
                                        printf("%i %s\n", i, buf);
                        }
                }
                if(waitpid(-1, NULL, WNOHANG) == -1)  // 
                        return;
        }
}

int child(int p[2]){
        int i;

        close(p[0]);
        for(i = 0; i < 2; i++){
                write(p[1], msg1, MSGSIZE);
                sleep(getpid()%4);
        }
        write(p[1], msg2, MSGSIZE);
        exit(0);
}

int main(){
        int p[3][2];
        int i;
        for(i = 0; i < 3; i++){
                pipe(p[i]);
                if(fork() == 0){
                        child(p[i]);
                }
        }
        parent(p);
        for(i = 0; i < 3; i++){
                wait(0);
        }
        exit(0);
}
                                  

// FIFO
 int main(){
        int fd, n;
        char buf[512];
        int counter = 0;

        mkfifo("fifo", 0600);
        fd = open("fifo", O_RDWR);
        //if(fd == -1)  perror("");
        for(;;){
                n = read(fd, buf, 512);
                //printf("%d %s\n", counter++, buf);
                //write(1, buf, n);
                printf("%s\n", buf);
        }
}


//FIFO-1
int main(){
        int fd, i, n;
        char buf[512];

        if(fd = open("fifo", O_WRONLY | O_NONBLOCK) < 0){
                exit(1);
        }
        for(i = 0; i < 3; i++){
                n = read(0, buf, 512);
                write(fd, buf, n);
        }
        //close(fd);

        exit(0);
}

