//q1
int main() {
        int i, fd, data1[10], data2[10];

        for(i=0;i<10;i++){
                scanf("%d", &data1[i]);
        }
        fd = open("data1", O_CREAT | O_RDWR);
        write(fd, data1, sizeof(int)*10);
        lseek(fd, 0, SEEK_SET);
        read(fd, data2, sizeof(int)*10);

        for(i=0; i<10; i++){
                printf("%-5d", data1[i]);
        }
        printf("\n");
        for(i=0; i<10; i++){
                printf("%-5d", data2[i]);
        }
        printf("\n");


        return 0;
}


//q2
int main()
{
        int fd = open("data2", O_RDWR | O_CREAT, 0700);
        char X[10];
        int i = 0;
        for(i = 0; i < 10; i++)
        {
                X[i] = 'X';
        }
        write(fd, X, sizeof(char)*10);
        lseek(fd, 0, SEEK_SET);
        write(fd, "Y", sizeof(char));  // 주소니깐 쌍따옴표 써야함
        lseek(fd, 0, SEEK_END);
        write(fd, "Y", sizeof(char));
        for(i = 1; i <= 7; i+=2)
        {
                lseek(fd, i, SEEK_SET);
                write(fd, "Z", sizeof(char));
        }
        lseek(fd, 14, SEEK_SET);
        write(fd, "T", sizeof(char));
        lseek(fd, -2, SEEK_END);
        write(fd, "S", sizeof(char));
        lseek(fd, -2, SEEK_CUR);
        write(fd, "W", sizeof(char));

        return 0;
}

//q3
int main()
{
        int i, fd, k, k1, data1[10], data2[10];

        fd = open("data3", O_RDWR|O_CREAT, 0600);
        for(i = 0; i < 10; i++)
        {
                scanf("%d", &data1[i]);
        }

        for(i = 0; i < 5; i++)
        {
                write(fd, &data1[i], sizeof(int));
                lseek(fd, sizeof(int), SEEK_CUR);
        }

        lseek(fd, 0, SEEK_SET);
        for(i = 5; i < 10; i++)
        {
                lseek(fd, sizeof(int), SEEK_CUR);
                write(fd, &data1[i], sizeof(int));
        }


        lseek(fd, 0, SEEK_SET);
        read(fd, data2, sizeof(int)*10);

        for(i=0; i<10; i++)
        {
                printf("%-5d", data1[i]);
        }
        printf("\n");
        
        for(i=0; i<10; i++)
        {
                printf("%-5d", data2[i]);
        }
        printf("\n");

        return 0;
}
