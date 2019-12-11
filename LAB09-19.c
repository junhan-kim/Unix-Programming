//q1
void change_directory(char *name)
{
        chdir(name);
}
void list()
{
        DIR *dp = opendir(".");  // "."
        struct dirent *d = readdir(dp);  // struct dirent*
        while(d!=NULL)
        {
                if(d->d_name[0] != '.')
                {
                        printf("%ld : %s\n", d->d_ino, d->d_name);
                }
                d = readdir(dp);
        }
}

int main()
{
        char name[50];

        while(1)
        {
                char cwd[100];

                getcwd(cwd, 99);
                printf("%s\n", cwd);

                scanf("%s", name);
                if(strcmp(name, "cd") == 0)
                {
                        scanf("%s", name);
                        change_directory(name);
                }
                else if(strcmp(name, "ls") == 0)
                {
                        list();
                }
                else
                        break;
        }
        return 0;
}
      

//q2
void list()
{
        DIR *dp = opendir(".");
        struct dirent *d = readdir(dp);
        struct stat buf;
        while(d!=NULL)
        {
                if(d->d_name[0] != '.')
                {

                        stat(d->d_name, &buf);
                        printf("%o, %d, %d, %d, %ld, %s, %s, %c\n", buf.st_mode&0777, buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_size, ctime(&(buf.st_mtime)), d->d_name, (S_ISREG(buf.st_mode))?'F':'D');
                }
                d = readdir(dp);
        }
}

int main()
{
        char name[50];

        while(1)
        {
                scanf("%s", name);
                if(strcmp(name, "ls") == 0)
                {
                        list();
                }
                else
                        break;
        }
        return 0;
}