#include "BST.h"
#define maximumLineLength 128

char stopIn[100][20]; //từ không có nghĩa để loại
int totalStopWord = 0;

char sasveLine[MAX][MAX]; //lưu các dòng
int totalLine = 0;       //lưu số lượng dòng trong file
// Check number of string
int checkIsNumber(char *s)
{
    int isNumber = 0; //0 is false, 1 is true
    for (int i = 0; i < strlen(s); i++)
    {
        for (int j = 48; j <= 57; j++) //in range number
        {
            if (s[i] == j || isalpha(s[i]) == 0)
                isNumber = 1;
            // break;
        }
    }
    return isNumber;
};
// Word stop -> go out
int checkWordInStop(char *word)
{
    int inStopWord = 0; //0 is not in, 1 is in.
    for (int i = 0; i < totalStopWord; i++)
    {
        if (strcmp(word, stopIn[i]) == 0)
        {
            inStopWord = 1;
            break;
        }
    }
    return inStopWord;
}
// spit and insert
void splitStringToArray(char *s, int line, LIST *l) //s la string cua 1 dong
{
    printf("%s\n", s);
    char array[100][100];
    int i = 0;
    char *p = strtok(s, " ");
    char test2[MAX][MAX]; //test2 la mang luu tru tam thoi cho tung substring
    //read full line
    while (p != NULL)
    {
        strcpy(test2[i], p);
        char value3[20];
        strcpy(value3, test2[i]);

        if (line == 0 && i == 0)
        {
            for (int indexChar = 0; indexChar < strlen(value3); indexChar++)
            {
                value3[indexChar] = tolower(value3[indexChar]);
            }
            printf("here");
            if (checkWordInStop(value3))
            {

                i++;
                p = strtok(NULL, " ");
                continue;
            }
            else
            {
                DT x;
                int idx_to_delete = strlen(value3) - 1;
                if (value3[strlen(value3) - 1] == '\n')
                {
                    printf("debug2: %s\n", value3);
                    strcpy(&value3[idx_to_delete], &value3[idx_to_delete + 1]);
                    idx_to_delete = strlen(value3) - 1;
                    printf("debug 5: %d\n", isalpha(value3[strlen(value3) - 1]) == 0);
                }
                if (isalpha(value3[strlen(value3) - 1]) == 0)
                {
                    printf("debug3: %s\n", value3);
                    strcpy(&value3[idx_to_delete], &value3[idx_to_delete + 1]);
                }
                strcpy(x.x, value3);
                Insert(l, x, line + 1);
                printf("ok %s\n", value3);
                i++;
                p = strtok(NULL, " ");
                continue;
            }
        }

        // //  Check xem có phải tên địa danh không nào
        else if (checkWordInStop(value3) || checkIsNumber(value3))
        {
            if (isalpha(value3[0]) != 0 && isalpha(value3[strlen(value3) - 1]) == 0 && value3[0] > 'Z') //example : miles)
            {
                printf("debug1: %s %d\n", value3, value3[strlen(value3) - 1] == '\n');
                DT x;
                int idx_to_delete = strlen(value3) - 1;
                if (value3[strlen(value3) - 1] == '\n')
                {
                    printf("debug2: %s\n", value3);
                    strcpy(&value3[idx_to_delete], &value3[idx_to_delete + 1]);
                    idx_to_delete = strlen(value3) - 1;
                    printf("debug 5: %d\n", isalpha(value3[strlen(value3) - 1]) == 0);
                }
                if (isalpha(value3[strlen(value3) - 1]) == 0)
                {
                    printf("debug3: %s\n", value3);
                    strcpy(&value3[idx_to_delete], &value3[idx_to_delete + 1]);
                }
                strcpy(x.x, value3);
                Insert(l, x, line + 1);
                printf("ok :%s\n", value3);
                i++;
                p = strtok(NULL, " ");
                continue;
            }
            else
            {
                i++;
                p = strtok(NULL, " ");
                continue;
            }
        }
        else
        {
            if (value3[0] >= 'A' && value3[0] <= 'Z') //gap ki tu in hoa va khong phai o dau cau
            {
                if (i == 0)
                {
                    if (isalpha(sasveLine[i - 1][strlen(sasveLine[i - 1]) - 1]) == 0)
                    {
                        for (int indexChar = 0; indexChar < strlen(value3); indexChar++)
                        {
                            value3[indexChar] = tolower(value3[indexChar]);
                        }
                        if (checkWordInStop(value3))
                        {
                            i++;
                            p = strtok(NULL, " ");
                            continue;
                        }
                        else
                        {
                            i++;
                            printf("ok: %s\n", value3);
                            DT x;
                            strcpy(x.x, value3);
                            Insert(l, x, line + 1);
                            printf("ok %s\n", value3);

                            p = strtok(NULL, " ");
                        }
                    }
                    else
                    {
                        p = strtok(NULL, " ");
                        i++;
                        continue;
                    }
                }
                else
                {
                    if (isalpha(test2[i - 1][strlen(test2[i - 1]) - 1]) != 0 || checkIsNumber(value3) == 1) //kiem  tra tu truoc do co phai so hoac dau ket thuc cau
                    {
                        i++;
                        p = strtok(NULL, " ");
                        continue;
                    }
                    else
                    {
                        for (int indexChar = 0; indexChar < strlen(value3); indexChar++)
                        {
                            value3[indexChar] = tolower(value3[indexChar]);
                        }
                        printf("after check number and isalpha %s\n", value3);
                        i++;
                        p = strtok(NULL, " ");
                    }
                }
            }
            else
            {
                DT x;
                strcpy(x.x, value3);
                Insert(l, x, line + 1);
                printf("ok %s\n", value3);

                i++;
                p = strtok(NULL, " ");
            }
        }
    }
}
void DNODE(DT x)
{
    int indexInLineArray = 0;
    printf("%-20s  %d,", x.x, x.timeOccur);
    // for (int indexOccured = 0; indexOccured < x.timeOccur; indexOccured++)
    // {
    //     printf("%d", x.line[indexOccured]);
    //     if (indexOccured < x.timeOccur - 1)
    //     {
    //         printf(",");
    //     }
    // }
    while (x.line[indexInLineArray] != NULL)
    {
        printf("%d", x.line[indexInLineArray]);
        if (x.line[indexInLineArray + 1] != NULL)
        {
            printf(",", x.line[indexInLineArray]);
        }
        indexInLineArray++;
    }
    printf("\n");
}
void DLIST(LIST *l)
{
    if (*l)
    {
        DLIST(&(*l)->Left);
        DNODE((*l)->x);
        DLIST(&(*l)->Right);
    }
}
int main()
{
    LIST *l = (LIST *)malloc(sizeof(LIST));
    Empty(l);
    FILE *f1, *f2;                  //khai báo file f1 và f2
    f1 = fopen("vanban.txt", "r+"); //readfile vanban.txt

    if (f1 == NULL) //check readfile successfull? not null => successfull
    {
        printf("Cannot open file \n");
        return 0; //iff null -> fail -> out.
    }
    f2 = fopen("stopw.txt", "r+");
    if (f2 == NULL)
    {
        printf("Cannot open file \n");
        return 0; //iff null -> fail -> out.
    }
    //tach dong
    while (!feof(f1))
    {
        fgets(sasveLine[totalLine], 1000, f1);
        totalLine++;
    }
    while (!feof(f2))
    {

        fscanf(f2, "%s\n", stopIn[totalStopWord]);
        totalStopWord++;
    }
    // splitStringToArray(sasveLine[4], 4);
    for (int lineRun = 0; lineRun < totalLine; lineRun++)
    {
        printf("---------line %d\n", lineRun + 1);
        splitStringToArray(sasveLine[lineRun], lineRun, l);
    }
    // splitStringToArray(sasveLine[0], 0, l);
    DLIST(l);
    Free(l);
    return 0;
}