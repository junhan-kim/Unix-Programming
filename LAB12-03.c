//q1_r
union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
};


int main(){
        int i, semid, shmid, *buf;
        union semun arg;
        struct sembuf p_buf;


        semid = semget(0444, 1, 0600|IPC_CREAT|IPC_EXCL);
        if(semid == -1){
                semid = semget(0444, 1, 0600);
        }
        else{
                arg.val = 1;
                semctl(semid, 0, SETVAL, arg);
        }

        shmid = shmget(0125, sizeof(int)*10, 0600|IPC_CREAT);
        buf = (int*)shmat(shmid, 0, 0);

        for(i = 0; i < 10; i++){
                scanf("%d", buf+i);
                p_buf.sem_num = 0;
                p_buf.sem_op = 1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);
        }

        exit(0);
}


//q1_w
union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
};


int main(){
        int i, semid, shmid, *buf;
        union semun arg;
        struct sembuf p_buf;


        semid = semget(0444, 1, 0600|IPC_CREAT|IPC_EXCL);
        if(semid == -1){
                semid = semget(0444, 1, 0600);
        }
        else{
                arg.val = 1;
                semctl(semid, 0, SETVAL, arg);
        }

        shmid = shmget(0125, sizeof(int)*10, 0600|IPC_CREAT);
        buf = (int*)shmat(shmid, 0, 0);

        for(i = 0; i < 10; i++){
                p_buf.sem_num = 0;
                p_buf.sem_op = -1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);
                printf("%d\n", *(buf+i));
        }

        shmctl(shmid, IPC_RMID, 0);
        exit(0);
}


//q2_r
int main(){
        int i, shmid, *buf;

        shmid = shmget(0125, sizeof(int)*2, 0600|IPC_CREAT);
        buf = (int*)shmat(shmid, 0, 0);

        for(i = 0; i < 10; i++){
                scanf("%d", buf+1);
                *buf = 1;
        }

        exit(0);
}


//q2_w
int main(){
        int i, shmid, *buf;

        shmid = shmget(0125, sizeof(int), 0600|IPC_CREAT);
        buf = (int*)shmat(shmid, 0, 0);

        for(i = 0; i < 10; i++){
                while(*buf == 0) {}
                printf("%d", buf[1]);
                *buf = 0;
        }

        shmctl(shmid, IPC_RMID, 0);
        exit(0);
}


//q3_server
struct databuf{
        int flag;
        int data;
};

int main(void){
        int shmid, i, j;
        struct databuf *buf;

        shmid = shmget(0111, 6*sizeof(struct databuf), 0600|IPC_CREAT);
        buf = (struct databuf*)shmat(shmid, 0, 0);

        // flag : 1  -> snd    flag : 0  -> ack
        for(i = 0; i < 15; i++){
                for(j = 0;;j=(j+1)%3){   // 0~2 infinite loop
                        if((buf+j)->flag == 1)
                                break;
                }
                (buf+j)->flag = 0;    // j is client to server
                (buf+j+3)->data = ((buf+j)->data)+8;  // j + 3 is server to client
                (buf+j+3)->flag = 1;
        }

        exit(0);
}


//q3_client
struct databuf{
        int flag;
        int data;
};

int main(int argc, char **argv){
        int shmid, i, j, id;
        struct databuf *buf;

        id = atoi(argv[1]);

        shmid = shmget(0111, 6*sizeof(struct databuf), 0600);
        buf = (struct databuf*)shmat(shmid, 0, 0);

        for(i = 0; i < 5; i++){
                scanf("%d", &((buf+id)->data));
                (buf+id)->flag = 1;
                while((buf+id+3)->flag == 0) {}
                printf("%d\n", (buf+id+3)->data);
                (buf+id+3)->flag = 0;
        }

        exit(0);
}
                        