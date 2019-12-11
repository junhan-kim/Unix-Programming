
//q1
int main(){
        int filedes = open("test1", O_WRONLY | O_CREAT, 0600);
        char ch = 'X';
        int i = 0;
        for(i = 0; i < 10; i++)
        {
                write(filedes, &ch, sizeof(char));
        }
        close(filedes);
        return 0;
}

//q2
int main(){
        int filedes = open("test2", O_WRONLY | O_CREAT, 0640);
        int data = 35;
        int i = 0;
        for(i = 0; i < 10; i++)
        {
                write(filedes, &data, sizeof(int));
        }
        close(filedes);
        return 0;
}

//q3
int main(){
        int filedes = open("test1", O_WRONLY);
        char ch = 'Y';
        int i = 0;
        for(i = 0; i < 5; i++)
        {
                write(filedes, &ch, sizeof(char));
        }
        close(filedes);
        return 0;
}

//q4
int main(){
        int filedes = open("test1", O_WRONLY | O_APPEND);
        char ch = 'Z';
        int i = 0;
        for(i = 0; i < 5; i++)
        {
                write(filedes, &ch, sizeof(char));
        }
        close(filedes);
        return 0;
}

//q5
int main(){
        int filedes = open("test1", O_WRONLY | O_TRUNC);
        char ch = 'K';
        int i = 0;
        for(i = 0; i < 3; i++)
        {
                write(filedes, &ch, sizeof(char));
        }
        close(filedes);
        return 0;
}

//q6
int main(){
        int filedes = open("test3", O_CREAT | O_WRONLY | O_EXCL, 0644);
        char ch = 'K';
        int i = 0;

        if(filedes == -1) {
                printf("Error occured");
        }
        else {
                for(i = 0; i < 10; i++)
                {
                        write(filedes, &ch, sizeof(char));
                }
                close(filedes);
        }
        return 0;
}

//q8
int main(){
        int filedes = open("test2", O_RDONLY);
        int data[10] = {0, };
        int i = 0;

        read(filedes, &data[i], sizeof(int)*5);

        for(i = 0; i < 5; i++)
        {
                printf("%d\n", data[i]);
        }
        close(filedes);
        return 0;
}
