/**************************************************************************************************************************** *** *
 * SymbolTable Implementation (STsize = 1000)
Contributors : Team 01(�ڱ���, ������, ������)
Date: 2021-03-27
�Է� :
        " .,;:?!\t\n" �� ���е� �ĺ��ڵ�� �̷���� ����(FILE_NAME)
��� :
        �ĺ��ڵ�� stringtable(ST)������ index, ���� ����
Global ���� :
    ST: String Table �迭
    HT: Hash Table�� ST�� index�� ǥ���ȴ�.
    letters: A..Z, a..z, _
    digits: 0~9
    seperators: null , . ; : ? ! \t \n \r
*****************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "witherr_input3.txt"
#define STsize 1000 //String Table ũ��
#define HTsize 100 //Hash Table ũ��
#define Isize 25 //�Է� �ĺ����� �ִ� ũ��
#define isWord(x) ((((x>='a'&&(x)<='z') || ((x)>='A'&&(x)<='Z')) || (x=='_') )
#define isNum(x) ((x) >= '0' && (x) <= '9')
#define idlen 12 //identifier �ִ� ���� 12

    typedef struct HTentry* HTpointer;
typedef struct HTentry
{
    int index;      //ST������ �ĺ��� index(head index)
    HTpointer next; //���� �ĺ����� ������
} HTentry;

//���� �з��� ������ �ִ� errorTypes
enum errorTypes
{
    noerror,
    illsp,
    illid,
    overst,
    overfl,
    noidentifier
};

typedef enum errorTypes ERRORtypes;
char separators[] = " .,;:?!\t\r\n";
HTpointer HT[HTsize];
char ST[STsize];

int nid = 0;   //���� identifier ��ġ index
int nfree = 0; //ST���� �������� �� index
int sameid = 0;//identifier�� ù���� index
int flag = 0;  //0�̸� �ߺ� ����, 1�̸� �ߺ� ����
int EOFflag = 1;
int initalize_done = 0;

ERRORtypes errr;
FILE* fp; //���� ������
char input;
char string[Isize];
int hashcode; //ComputeHT ��� ���� ����

void initialize() {
    fp = fopen(FILE_NAME, "r");
    input = fgetc(fp); initalize_done = 1;
}

//isSeperator �Լ�: char c�� seperator�� �ش翩��(0/1) ���
int isSeparator(char c)
{
    for (int i = 0; i < strlen(separators); i++)
    {
        if (c == separators[i] || c == '\r')
            return 1;
    }
    return 0;
}

//isWordNum �Լ�: char c�� word/Num�� �ش翩��(0/1) ���
int isWordNum(char c)
{
    if (isWord(c) || isNum(c)))
    return 1;
    else
        return 0;
}

// PrintHStable �Լ�: HashTable �ᱣ�� ��� - hashcode, identifier list�� ��ü character ���
void PrintHStable()
{
    printf("[[ HASH TABLE ]]\n\n");

    int i = 0;
    while (i < HTsize) {
        if (HT[i] != NULL) {
            printf("Hash code %-3d: ", i); //hashcode ���
            HTpointer pt = HT[i];
            while (pt != NULL) {
                //�� hashcode�� identifier list ���
                int st_index = pt->index;
                while (ST[st_index] != '\0' && st_index < STsize) printf("%c", ST[st_index++]);
                printf(" ");
                pt = pt->next;
            }
            printf("\n");
        }
        i++;
    }
    printf("\n<%d characters are used in the string table>\n", nfree); //nfree�� ��ü character ���
}

//PrintError �Լ�: ERRORtypes ��ü ������ �������� ERROR ������ ���
// error: illsp(������ �ʴ� ���� ����� �ĺ��� ����, SkipSeperators�Լ�)
//        illid(���ڿ� �Բ� �����ϴ� �ĺ��� ����,SkipStartWithNumber�Լ�)
//        overst(�ĺ��ڱ���idlen�� �Ѵ� �ĺ��� ����, get_identifier�Լ�)
//        overfl(���� ũ��� overflow �߻�, ReadIO�Լ�)
void PrintError(ERRORtypes err)
{
    printf("***ERROR***\t\t");
    if (err == illsp)
    {
        for (int i = 0; string[i] != '\0'; i++) printf("%c", string[i]);
        printf(" is not allowed\n");
    }

    else if (err == illid)
    {
        for (int i = 0; string[i] != '\0'; i++) printf("%c", string[i]);
        printf(" starts with digit\n");
    }

    else if (err == overst)
    {
        for (int i = 0; string[i] != '\0'; i++) printf("%c", string[i]);
        printf(" is more than 12 words\n");
    }

    else if (err == overfl)
    {
        printf("\tOVERFLOW\n");
        printf("\n\n==================================================\n");
        PrintHStable();
        exit(0);
    }
}

//get_identifier �Լ�: identifier ������ �����Ͽ� overst ���� Ȯ��
void get_identifier()
{
    int i = 0; char temp;
    if (initalize_done == 1) { temp = input; initalize_done = 0; }
    else temp = fgetc(fp);
    while (1) {
        if (!isSeparator(temp)) break;
        temp = fgetc(fp);
    }

    for (i = 0; i < Isize; i++) {
        if (temp == EOF || isSeparator(temp))
        {
            string[i] = '\0';
            if (temp == EOF) EOFflag = 0;
            break;
        }
        else
        {
            string[i] = temp;
        }
        temp = fgetc(fp);
    }
    if (i == 0) {
        //separator �ڿ� EOF�� ��� string�� '\0'�� ������
        errr = noidentifier;
    }
    if (i > idlen) {
        errr = overst;
        PrintError(errr);
    }
}

//SkipSeperators �Լ�: ������� ���� ���� �ִ��� Ȯ��(illsp ���� üũ)
void SkipSeparators()
{
    int i = 0;
    while (string[i] != '\0')
    {
        if (!isWordNum(string[i]))
        {
            errr = illsp;
            PrintError(errr);
            break;
        }
        i++;
    }
}

//SkipStartWithNumber �Լ�: ���ڷ� �����ϴ��� Ȯ��(illid ���� üũ)
void SkipStartWithNumber()
{
    if (isNum(string[0]))
    {
        errr = illid;
        PrintError(errr);
    }
}

//ReadIO �Լ�: FILE_NAME�� �ش��ϴ� fp�� ���� �о ST�� �ֱ�(overfl ���� üũ)
void ReadID()
{
    nid = nfree;
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (nfree >= STsize) //STSize overflow
        {
            errr = overfl;
            PrintError(errr);
            break;
        }
        else {
            ST[nfree++] = string[i];
        }
    }
}

//ComputeHS �Լ�: ST�� �����ϴ� [nid~(nfree-2)]������ character�� �̿��� �ؽ��Լ� ����
//               H(x) = (f(x) mod m)+1
void ComputeHS(int nid, int nfree)
{
    int tot_ascii = 0;
    for (int i = nid; i < nfree - 1; i++)
        tot_ascii += (int)ST[i];
    hashcode = tot_ascii % HTsize;
}

// LookupHS: identifier�� �ؽ� ��� �ߺ� �߻� ���ο� ���� flag �� ����
void LookupHS(int nid, int hscode)
{
    HTpointer temp;
    int a, b; //a: HT�� index, b: ���� identifier�� index
    flag = 0;

    if (HT[hscode] != NULL) {
        temp = HT[hscode];
        while (temp != NULL && flag == 0) {
            flag = 1;
            a = temp->index;
            b = nid;
            sameid = a;

            while (ST[a] != '\0' && flag == 1) {
                //�ߺ� �߻�
                if (ST[a] == ST[b]) {
                    a++;
                    b++;
                }
                //�ߺ����� ���� ���
                else flag = 0;
            }
            temp = temp->next;
        }
    }
}

// ADDHT �Լ�: HTpointer �Ҵ� �޾Ƽ� HS�� hscode�� identifier ����
void ADDHT(int hscode)
{
    HTpointer pt;
    pt = (HTpointer)malloc(sizeof(pt));
    pt->index = nid;
    pt->next = HT[hscode];
    HT[hscode] = pt;
}

void PrintHeading() {
    printf("==================================================\n");
    printf("�����Ϸ� ���� 1\n");
    printf("1�� - �ڱ���, ������, ������\n");
    printf("==================================================\n\n");
}

// main �Լ�
// FILE_NAME�� �ش��ϴ� ���� ����
// while{
//        identifer�� ����(overst, illsp, illid) Ȯ��
//        1. ReadID: ST�� identifier ����, overfl Ȯ��
//        2. ComputeHS: identifier�� ���� �ؽ� �Լ� ��� ����
//        3. LookupHS: �ؽ� �Լ� ��� �������� HS�� ����, flag �� ����
//        4. ADDHT: HT�� identifier �߰�
// }
// Hash Table ��� ����ϱ�
int main()
{
    PrintHeading();
    int i;
    initialize();

    printf("------------\t\t----------\n");
    printf("Index in ST\t\tIdentifier\n");
    printf("------------\t\t----------\n");

    while (EOFflag) {
        errr = noerror;

        get_identifier(); //overst, noidentifier Ȯ��
        SkipSeparators(); //illsp Ȯ��
        SkipStartWithNumber(); //illid Ȯ��

        if (errr == noerror)
        {
            ReadID(); //overfl Ȯ��
            ST[nfree++] = '\0';
            ComputeHS(nid, nfree);
            LookupHS(nid, hashcode);

            if (!flag) {
                printf("%-11d\t\t", nid);
                i = nid;
                while (i < nfree - 1) printf("%c", ST[i++]);
                printf("...(entered)\n");
                ADDHT(hashcode);
            }
            else {
                printf("%-11d\t\t", sameid);
                i = nid;
                while (i < nfree - 1) printf("%c", ST[i++]);
                printf("��(already exists)\n");
                nfree = nid;
            }
        }

    }

    printf("\n\n==================================================\n");
    PrintHStable();
}