int mod, i, pipe[3][2], pid, num, cnt, cnt2, in, back;


void r_init() {
        static struct act;
        void catchint(int);
        act.sa_handler = SIG_IGN;
        sigaction(SIGINT, &act, NULL);   // 부모에선 SIG_IGN
        for(i=0; i<3; i++) {
                pipe(p[i]);
        }
        pid = fork();

        if(pid==0) {
                act.sa_handler = catchint;
                sigaction(SIGINT, &act, NULL);  // 자식에선 catchInt
        }
        // parent 최초 대기 (num)
        if(pid!=0) {
                read(p[1][0], &cnt, sizeof(int));  // 이 부분 blocking 푸는건 main에서 따로
 처리할듯(고려 x)
        }
}

void r_scanf(int *in) {
        if(mod == 0) {           // 정상 모드   
                if(pid==0) {     // child
                        scanf("%d", &num);
                        write(p[0][1], &num, sizeof(int));  // 파이프 0에 값 밀어넣음
                        cnt++;  // cnt 갱신
                }
        }
        else if(mod == 1) {     // 복구모드
                if(pid!=0) {            // parent
                        read(p[1][0], &cnt2, sizeof(int));      // 입력받은 갯수 읽기
                        read(p[2][0], &back, sizeof(int));      // 버릴 갯수 읽기
                        cnt = cnt2 + back;  // back은 음수이므로 cnt는 유효한 위치가 됨
                                            // 유효한 위치는 새로운 cnt가 됨
                        for(i=0; i<cnt; i++) {          // 0 ~ 유효한 위치까지 읽기
                                read(p[0][0], &num, sizeof(int));  // 유효한값은 받아서 
                                write(p[0][1], &num, sizeof(int)); // 다시 밀어넣고
                        }
                        for(i=back; i<cnt2; i++) {  // 나머지 뒷 부분은
                                read(p[0][0], &num, sizeof(int));
                                *in += num;  // 전부 읽어서 in에 저장
                        }
                        in = -1*in;   // 음수로 바꿔서 sum에서 빼게 함 

                        mod = 0;   // 정상 모드 설정                            
                        pid = fork();    // 복구 위치와 데이터를 가진채로 자식생성
                        if(pid!=0) {
                                read(p[1][0], &cnt, sizeof(int));  // 부모는 다시 입력 대기
                        }
                }
        }
}

void catchint(int signo) {
        mod = 1;             // ^C 입력시 복구모드
        fflush(stdin);
        scanf("%d", &back);
        write(p[1][1], &cnt, sizeof(int));    // 입력받은 갯수 전달     
        write(p[2][1], &back, sizeof(int));   // 뒤에서부터 버릴 갯수 전달
        exit(0);                              // child 종료
}

void r_close() {
        int back = 0;
        if(pid == 0) {
                // back에 값이 있다 = ^C 상태에서 back을 부모로 보내기 전에 종료된 상황
                if(read(p[1][0], &back, sizeof(int)) != 0) {
                        // 정상 종료가 아니므로 다시 back을 파이프에 넣어주고, 종료하지 않>음
                        write(p[1][1], &back, sizeof(int));
                }
        }
        else {
                read(p[1][0], &back, sizeof(int));
                if(back==0) wait(0);                // ?   
        }

}

void r_printf() {
        if(mod == 0) {
                printf("%d ... sum = %d\n", num, sum);
        }
}
                            