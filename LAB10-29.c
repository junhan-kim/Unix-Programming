void catchAlarm(int signo){
        printf("please Input!!\n");
        alarm(10);  // 경고 메시지 후에도 알람 설정
}

int main(){
        int i, tmp, sum = 0;
        static struct sigaction act;
        act.sa_handler = catchAlarm;
        sigaction(SIGALRM, &act, NULL);

        for(i = 0; i < 10; i++){
                do{
                        alarm(10);  // 맨 처음에도 알람 설정
                }
                while(scanf("%d", &tmp) < 0);  // 입력 오류 시에도 알람 설정
                alarm(0);  // 잔여 알람 제거
                sum += tmp;
        }
        printf("sum : %d\n", sum);

        return 0;
}