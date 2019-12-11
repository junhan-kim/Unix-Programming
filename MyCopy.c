#include "header.h"
#define BUF 512

int main()
{
        char filename1[100], filename2[100];
        int ret;

        scanf("%s %s", filename1, filename2);
        int filedes1 = open(filename1, O_RDONLY);
        if(filedes1 == -1)
        {
                perror("");
                return 0;
        }

        int filedes2 = open(filename2, O_EXCL | O_CREAT);
        if(filedes2 == -1)
        {
                printf("caution : this might be delete your file\n");
                int tmp;
                scanf("%d", &tmp);
                if(tmp != 1)
                {
                        return 0;
                }
        }
        filedes2 = open(filename2, O_TRUNC | O_WRONLY);

        char buf[BUF];
        
        ret = BUF;
        while(ret == BUF)
        {
                ret = read(filedes1, buf, BUF);
                write(filedes2, buf, ret);
        }       
        
        return 0;
}       
                           