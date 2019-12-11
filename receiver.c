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
        int term;
};


int isAckAll(struct Shm *pShm, int cur){   // 해당 공간의 데이터가 전부 수신이 되었는지를 체크하는 함수
        int i;
        for(i = 0; i < 4; i++){
                if(pShm->msgBuf[cur].ack[i] == 0)  // 한개라도 ack가 0
                        return -1;
        }
        return 0;
}


int main(int argc, char **argv){
        int i, idx, id;
        char str[512];
        int cur;   // 현재 수신자가 작업하는 곳의 인덱스

        // 공유메모리 초기화 생략
        struct Shm shm;

        id = atoi(argv[1]);
        //=== last Sem--
        cur = shm.last;   // 처음 입장시 last 값을 cur에 대입
        //=== last Sem++
        while(1){
                if(shm.term == 1) break;  // 부모로부터 종료를 고지받았으므로 종료
                if(cur == -1) continue;  // 현재 cur가 존재하지 않으므로 작업을 하지 않음
                //=== msgBuf[cur] Sem--
                printf("%d:%s", shm.msgBuf[cur].id, shm.msgBuf[cur].data);   // id와 data를 출력
                shm.msgBuf[cur].ack[id] = 1;   // 수신되었음
                if(isAckAll(&shm,cur)==0){  // cur 위치의 데이터가 모두에게 수신된 상태
                        shm.msgBuf[cur].on = 1;  // 사용 가능해졌으므로 버퍼에 반환
                        //=== empty Sem++       
                }
                cur = shm.msgBuf[cur].next;  // 다음 입력받을 공간으로 이동     
                //=== msgBuf[cur] Sem++
        }
        exit(0);
}
                                                