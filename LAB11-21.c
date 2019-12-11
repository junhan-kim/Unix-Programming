//q1_r
union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(){
        union semun arg;
        struct sembuf p_buf;
        int fd, i, n=0, semid;
        char *addr;

        fd = open("data1", O_CREAT|O_RDWR, 0600);
        addr = mmap(NULL, 512, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        ftruncate(fd, 512);

        semid = semget(0111, 1, 0600|IPC_CREAT|IPC_EXCL);
        if(semid == -1){
                semid = semget(0111, 1, 0600);
        }
        else {
                arg.val = 1;   // val의 의미 : 동시에 자원을 쓸 수 있는 사람은 1명뿐이다.
                semctl(semid, 0, SETVAL, arg);
        }

        for(i = 0; i < 3; i++){
                n = n + read(0, addr + n, 50);
        }
        p_buf.sem_num = 0;
        p_buf.sem_op = 1;    // 0인 세마포 값에 +1을 하여 동작하도록 하였다.
        p_buf.sem_flg = 0;
        semop(semid, &p_buf, 1);

        return 0;
}


//q1_w
union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(){
        int fd, i, semid, n = 0;
        char *addr;
        union semun arg;
        struct sembuf p_buf;

        fd = open("data1", O_RDWR | O_CREAT, 0600);
        addr = mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ftruncate(fd, 512);

        semid = semget(0111, 1, 0600|IPC_CREAT|IPC_EXCL);
        if(semid == -1)  // EXCL이므로 기존에 있었으면 -1  => 그냥 get만 함
                semid = semget(0111, 1, 0600);  // 세마포어들 만들어서 semid에 저장
        else{   // 처음 세마포어를 만든 경우
                arg.val = 1;   // 세마포어 값
                semctl(semid, 0, SETVAL, arg);  // semid의 0번 인덱스의 세마포어 값을 arg.val로 지정
        }
        p_buf.sem_num = 0;  // index 0
        p_buf.sem_op = -1;  // wait   => 세마포 값 1에서 -1을 하여 0이 되어 wait 상태이다. 
        p_buf.sem_flg = 0;
        semop(semid, &p_buf, 1);   // 1 : sem갯수
        for(i = 0; i < 3; i++){
                n = n + write(1, addr + n, 50);  // 출력한만큼 n에 더함 (n : 현재 인덱스)
        }
        exit(0);
}


//q2-1
// 세마포 3개로 처리하는 법
// 세마포 0, 1, 2 는 각각 id 2, 3, 4에 해당하는 세마포이다.

union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
};

int main(int argc, char **argv){
        int i, id, pid, semid;
        union semun arg;
        struct sembuf p_buf;
        ushort buf[3] = {0};

        id = atoi(argv[1]);

        semid = semget(0333, 3, 0600|IPC_CREAT|IPC_EXCL);
        if(semid == -1)
                semid = semget(0333, 3, 0600);
        else{
                arg.array = buf;
                semctl(semid, 0, SETALL, arg);
        }

        if(id > 1){
                p_buf.sem_num = id-2;  // 2,3,4는 각각 세마포 0,1,2에 -1 연산하여 0으로 만>듬(wait)
                p_buf.sem_op = -1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);    // 1-1=0이 되어 여기서 대기 
        }
        pid = getpid();
        for(i = 0; i < 5; i++){
                printf("%d %d\n", id, pid);
                sleep(1);
        }
        if(id < 4) {
                p_buf.sem_num = id-1;  // 1,2,3은 각각 작업을 끝낸 후, 세마포 0,1,2에 +1 연산하여 1로 만듬(signal)
                p_buf.sem_op = 1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);
        }

        exit(0);
}


//q2-2
// 세마포 1개로 처리하는 법

union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
};

int main(int argc, char **argv){
        int i, id, pid, semid;
        union semun arg;
        struct sembuf p_buf;
        ushort buf[3] = {0};

        id = atoi(argv[1]);

        semid = semget(0333, 3, 0600|IPC_CREAT|IPC_EXCL);
        if(semid == -1)
                semid = semget(0333, 3, 0600);
        else{
                arg.array = buf;
                semctl(semid, 0, SETALL, arg);
        }

        if(id > 1){
                p_buf.sem_num = id-2;  // 2,3,4는 각각 세마포 0,1,2에 -1 연산하여 0으로 만>듬(wait)
                p_buf.sem_op = -1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);    // 1-1=0이 되어 여기서 대기 
        }
        pid = getpid();
        for(i = 0; i < 5; i++){
                printf("%d %d\n", id, pid);
                sleep(1);
        }
        if(id < 4) {
                p_buf.sem_num = id-1;  // 1,2,3은 각각 작업을 끝낸 후, 세마포 0,1,2에 +1 연산하여 1로 만듬(signal)
                p_buf.sem_op = 1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);
        }

        exit(0);
}
