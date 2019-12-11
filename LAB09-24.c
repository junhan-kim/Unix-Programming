// q1
int list(const char *name, const struct stat *status, int type)   // os가 반복하여
 list 호출
// list 함수 포인터를 가진 함수를 그 자체의 자료구조(name, status)들을 활용하여 실
행
{
        if(type == FTW_D || type == FTW_DNR)  // 디렉토리
                printf("%s, %ld\n", name, status->st_size);
        else if(type == FTW_F)  // 파일 중
        {
                if(S_IXUSR & status->st_mode || S_IXGRP & status->st_mode || S_IXOTH & status->st_mode)  // 실행 파일
                        printf("%s, %ld\n", name, status->st_size);
        }
        return 0;  // os로 가는 리턴값.  , 일부러 멈출때도 0아닌 다른 값 주기 
}

int main()
{
        ftw(".", list, 100);

        return 0;
}


//q2
int main()
{
        printf("%ld\n", getpid());
        printf("%ld\n", getpgrp());
        printf("%ld\n", getsid(getpid()));

        return 0;
}


//q3
int main(int argc, char **argv)
{
        int i = 0;
        for(i = 1; i < argc; i++)
        {
                printf("%s\n", argv[i]);
        }


        return 0;
}


