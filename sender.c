struct MsgBuf{
        int id;   // 송신자
        int ack[4];  // 수신자 각각이 수신 완료된 상태인지를 체크하는 플래그
        char data[512];  // 데이터
        int next;  // 이후 순서의 인덱스
        int on;  // 현재 인덱스가 사용가능한 상태인지를 체크하는 플래그
};

struct User{
        int id;  // 유저 id
        int term;   // 유저가 종료되었는지를 체크하는 플래그
};

struct Shm{
        struct MsgBuf msgBuf[1024];  // 메시지들의 history가 저장되는 공간
        struct User info[4];    // 각 유저의 정보가 저장되는 공간
        int last;   // 송신자가 제일 마지막으로 보낸 메시지의 인덱스
        int term;   // 수신자에게 종료 상태를 고지하는 플래그
};


int main(int argc, char **argv){
        int i, idx, id;
        char str[512];

        //////// 초기화 : 이 부분은 오직 맨 처음 송신자에서만 실행됨(shared memory가 최초로 생성될때)
        // EXCL 옵션을 이용
        struct Shm shm;
        for(i = 0; i < 4; i++){
                shm.info[i].id = i+1;  // id 1 ~ 4
                shm.info[i].term = 1;  // 전부 종료된 상태로 초기화
        }
        shm.last = 0;
        shm.term = 0;
        ////////

        id = atoi(argv[1]);
        printf("id=%d\n", id);  // 입장

        while(1){
                scanf("%s", str);
                // === empty Sem--  => empty Sem 값은 1024
                // === shm.msgBuf--
                for(i = 0; i < 1024; i++){
                        if(shm.msgBuf[i].on == 1)  // 새로 입력 가능한 공간이 존재하면
                                break;
                }
                // === shm.msgBuf++
                idx = i;
                // === shm.last--
                shm.msgBuf[shm.last].next = idx;  // 그 공간의 인덱스를 이전 last의 next에 저장
                // === shm.last++
                // === msgBuf[idx] Sem--
                shm.msgBuf[idx].id = id;  // 자신의 id 입력 
                strcpy(shm.msgBuf[idx].data, str);  // data 입력
                shm.msgBuf[idx].on = 0;  // 모두 수신되어 이 공간이 반환될때까지 점유되었음 
                for(i = 0; i < 4; i++){
                        // 종료 상태가 아니거나, id가 같지 않은 수신자들은
                        if(shm.info[i].term == 0 || shm.info[i].id != id)
                                shm.msgBuf[idx].ack[i] = 0;  // 아직 수신되지 않은 것으로 초기화
                        else
                                shm.msgBuf[idx].ack[i] = 1;  // 그 외에는 수신된 것으로 침
                }
                if(strcmp(str, "talk_quit")==0) {
                        // === shm.info[id].term Sem--
                        shm.info[id].term = 1;
                        // === shm.info[id].term Sem++
                        shm.term = 1;
                        break;  // talk_quit 메시지 보내고 나감
                }
                // === msgBuf[idx] Sem++
                // === last Sem--
                shm.last = idx;  // last 갱신
                shm.msgBuf[shm.last].next = -1;  // last의 next는 -1 (현재 존재하지 않음으로 표시)
                // === last Sem++       
        }
        exit(0);
}
                         

