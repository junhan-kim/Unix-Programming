//q1
int main()
{
        struct stat buf;

        stat("p1.c", &buf);
        printf("%o, %d, %ld\n", buf.st_mode&0777, buf.st_nlink, buf.st_size);

        return 0;
}


//q2
int main()
{
        int ret;
        ret = access("data1", F_OK);
        if(ret == 0)
        {
                ret = access("data1", R_OK | W_OK);
                if(ret == 0)
                {
                        printf("Read Write OK");
                }
                else
                {
                        printf("Read Write Not OK");
                }
        }
        else
        {
                printf("file not exist");
        }

        return 0;
}


//q3
int main()
{
        char str1[100], str2[100];
        scanf("%s %s", str1, str2);
        link(str1, str2);

        return 0;
}


//q4
int main()
{
        char str1[100], str2[100];
        scanf("%s %s", str1, str2);
        symlink(str1, str2);

        return 0;
}


//q6
int main()
{
        char sym[100] = {0,};
        scanf("%s", sym);

        struct stat buf;
        lstat(sym, &buf);
        printf("symbolic link : %s,  %o, %ld\n", sym, buf.st_mode&0777, buf.st_size);

        char file[100] = {0,};
        readlink(sym, file, 100);

        stat(file, &buf);
        printf("file name : %s, %o, %ld\n", file, buf.st_mode&0777, buf.st_size);

        return 0;
}


//q7
#define BUF 512   // 여기서도 read 사용하므로 512

int main()
{
        char input[BUF] = {0,};
        int ret = read(0, input, BUF);
        input[ret-1] = '\0';  // ret-1의"\n"을 "\0"으로 대체하여 문자열화

        struct stat buf;
        stat(input, &buf);
        printf("%o, %d, %d, %d, %ld, %s", buf.st_mode&0777, buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_size, ctime(&(buf.st_mtime)), input);

        // ctime에도 널문자 추가 가능

        return 0;
}