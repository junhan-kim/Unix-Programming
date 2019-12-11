//q1_c
struct q_entry{
        long mtype;
        int data;
};

int main(int argc, char **argv){
        struct q_entry msg;
        key_t key;
        int qid, i, k, num;

        key = ftok("key", 3);
        qid = msgget(key, 0600 | IPC_CREAT);

        k = atoi(argv[1]);

        for(i = 0; i < 5; i++){
                scanf("%d", &num);
                msg.mtype = k;   // mtype 한번 보낼때마다 초기화 (반복문안에서 값 설정할 것
)
                msg.data = num;
                msgsnd(qid, &msg, sizeof(int), 0);
                msgrcv(qid, &msg, sizeof(int), k+3, 0);  // 수신용 타입으로 전환
                printf("%d\n", msg.data);
        }

        exit(0);
}


//q1_s
struct q_entry{
        long mtype;
        int data;
};

int main(){
        struct q_entry msg;
        key_t key;
        int qid;

        key = ftok("key", 3);
        qid = msgget(key, 0600 | IPC_CREAT);  // key 변수 대신 (key_t)1234 이런것도 가능

        while(1){
                msgrcv(qid, &msg, sizeof(int), -3, 0);  // 1,2,3 중 제일 먼저 도착한 메시지
                msg.mtype += 3;  // 송신용 타입으로 전환
                msg.data += 8;
                msgsnd(qid, &msg, sizeof(int), 0);
        }

        exit(0);
}


//q2
struct q_entry{
        long mtype;
        char a;
};

int main(int argc, char **argv){
        struct q_entry msg;
        key_t key;
        int qid, k, i, pid;

        k = atoi(argv[1]);
        key = ftok("key", 3);
        qid = msgget(key, 0600 | IPC_CREAT);

        // start 제외하고 sleep
        if(k > 1) msgrcv(qid, &msg, sizeof(char), k, 0);  // mtype이 1부터 시작하므로 k > 1
        pid = getpid();
        for(i = 0; i < 5; i++){
                sleep(1);
                printf("%d\n", pid);
        }
        // end 제외하고 wake
        msg.mtype = k + 1;
        if(k < 4) msgsnd(qid, &msg, sizeof(char), 0);
        exit(0);
}
