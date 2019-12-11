//q1_r
int main(){
        int i;

        int fd = open("temp", O_CREAT | O_TRUNC | O_RDWR, 0600);  // 메모리 매핑시
에는 무조건 RDWR로 열 것
        int *p = (int*)mmap(NULL, sizeof(int)*10, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        ftruncate(fd, sizeof(int)*10);  // CREAT 시에는 공간 할당

        for(i = 0; i < 10; i++){
                scanf("%d", p+i);
        }


        return 0;
}


//q1_w
int main(){
        int i;

        int fd = open("temp", O_RDWR);  // 메모리 매핑시에는 무조건 RDWR로 열 것
        int *p = (int*)mmap(NULL, sizeof(int)*10, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

        sleep(5);
        for(i = 0; i < 5; i++){
                printf("%d\n", p[i]);
        }
        sleep(5);
        for(i = 5; i < 10; i++){
                printf("%d\n", p[i]);
        }

        return 0;
}

//q2_r
int main(){
        int i, len = 0;
        int fd = open("temp", O_CREAT | O_TRUNC | O_RDWR, 0600);
        char *p = (char*)mmap(NULL, 512, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0); // 512
        ftruncate(fd, 512);
        for(i = 0; i < 3; i++){
                len += read(0, p + len, 512);    //
                if(len > 512) break;     //
        }
        return 0;
}


//q2_w
int main(){
        int i, len = 0;
        int fd = open("temp", O_RDWR);
        char *p = (char*)mmap(NULL, 512, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        for(i = 0; i < 3; i++){
                sleep(3);
                len += write(1, p + len, 512);  // p + len에 write 한만큼 더한 값>을 len으로 갱신
                if(len > 512) break;    //
        }
        return 0;
}

//q3
// 순차 실행 문제 풀이법 :
// start 제외하고 sleep, end 제외하고 다음 프로세스 wake

void do_child(int *turn, int id){
        int i, pid = getpid();
        while(*turn != id) {}  // sleep(스핀락)
        for(i = 0; i < 5; i++){
                sleep(1);
                printf("%d\n", pid);
        }
        if(id > 0) *turn = id - 1; // end제외하고 wake  
        exit(0);
}


int main(){
        int i;
        int fd = open("watch", O_CREAT | O_TRUNC | O_RDWR, 0600);
        int *p = (int*)mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        ftruncate(fd, sizeof(int));  // CREAT시 ftruncate
        *p = 2;  // start
        for(i = 0; i < 3; i++){
                if(fork() == 0){
                        do_child(p, i);
                }
        }
        for(i = 0; i < 3; i++){
                wait(0);
        }
        exit(0);
}

