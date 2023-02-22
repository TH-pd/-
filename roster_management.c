#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int profile_data_nitems = 0;
int MAX_LEN = 70;
struct date {  // 構造体の宣言部  //
  int y;   // year
  int m;   // month
  int d;   // day of month
};

struct profile {
    int id;             //ID
    char school[70];    //school_name
    struct date birth;        //day of month
    char place[70];     //place
    char *sub;     //sub_data
};

struct profile profile_data_store[10000];

int func_max(int i, int j);            //関数の宣言部//
int func_min(int i, int j);
int subst(char *str, char c1, char c2);
int split(char *str, char *ret[], char sep, int max);
int get_line(char *line, FILE *fp);
struct profile *new_profile(struct profile *profiles, char *line);
void exec_command(char cmd1, char cmd2, char *param);
void parse_line(char *line);
void cmd_quit();
void cmd_check();
void cmd_print(int param);
void cmd_read(char *fn);
void cmd_write(char *fn);
void cmd_find(char *param);
void cmd_sort(int param);
void cmd_complex_print(char *param);
void cmd_del(int i);
void cmd_bin_write(char *fn);
void cmd_bin_read(char *fn);
void output_profile(int i);
void save_profile(int i, FILE *fp);
int com_str(char *str, int i);
int comp_str(char *str1, char *str2, int cnt);
void swap_struct(int a, int b);
void quick_sort(int start, int end, int param);
int call_quick(int s, int e, int front, int back, int param);
int sum_date(int i);
void output_10to16(FILE *fp);
void read_bin(FILE *fp);

int main()
{
    int MAX = 1024;
    char line[MAX + 1];
    while (get_line(line, stdin)) {
        parse_line(line);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int func_max(int i, int j) {
  //2数の大きい方を返す//
    if(i > j) {
        return i;
    }
    return j;
}

int func_min(int i, int j) {
  //2数の小さい方を返す//
    if(i < j) {
        return i;
    }
    return j;
}

int subst(char *str, char c1, char c2)
  //文字列の置換を行う//
{
    int n = 0;
    for( ; *str != '\0'; str++) {
        if(*str == c1) {
            *str = c2;
            n++;
        }
    }
    return n;
}

int split(char *str, char *ret[], char sep, int max)
  //指定された文字で区切る//
{
    int n;
    int c = subst(str, sep, '\0') + 1;
    if(c > max) {
        return c;   // 区切り数が多すぎる場合はエラー //
    }
    for(n = 0; n < c;) {
        ret[n++] = str;  // 文字列をコピーし適切な位置に'\0'を置く //
        while(*str) {
            str++;
        }
        str++;
    }
    return n;
}

int get_line(char *line, FILE *fp)//1行読み込み//
{
    int max = 1024;
    if(fgets(line, max + 1, fp) == NULL) {
        return 0;
    }
    subst(line, '\n', '\0');
    return 1;
}


struct profile *new_profile(struct profile *profiles, char *line) 
//新規データを構造体に格納//
{
    int i, len, n, j;
    char *ret[5];
    char *birth[3];
    // 標準入力の区切り数が異常な場合 //
    if (split(line, ret, ',', 5) != 5) {
        profile_data_nitems;
        printf("Please input 5 elements\n");
        return;
    }
    // 年月日の入力が異常な場合 //
    i = split(ret[2], birth, '-', 3);
    if(i != 3) {
        printf("Please input element 3 int-int-int\n");
        return;
    }
    for(i = 0; i < 3; i++) {
        n = atoi(birth[i]);
        for(j = 0;;) {
            if(n < 10) {
                break;
            }
            n = n / 10;
            j++;
        }
        j++;
        if((atoi(birth[i]) == 0) || (strlen(birth[i]) != j)) {
            printf("Please input element 3 int-int-int\n");
            return;
        }
    }
    
    profiles->id = atoi(ret[0]);
    strcpy(profiles->school, ret[1]);
    profiles->birth.y = atoi(birth[0]);
    profiles->birth.m = atoi(birth[1]);
    profiles->birth.d = atoi(birth[2]);
    strcpy(profiles->place, ret[3]);
    (profiles->sub) = (char *)malloc(sizeof(char) * (strlen(ret[4])) + 1);
    if(strlen(ret[4]) > MAX_LEN) {
        MAX_LEN = strlen(ret[4]);
    }
    strcpy(profiles->sub, ret[4]);
    profile_data_nitems++;
    return profiles;
}

void exec_command(char cmd1, char cmd2, char *param)
//%から始まる入力に対して適切なコマンドを割り当てる//
{
    switch(cmd1) {
    case 'Q':
        cmd_quit();
        break;
    case 'C':
        switch(cmd2) {
        case 'P':
            cmd_complex_print(param);
            break;
        default:
            cmd_check();
        }
        break;
    case 'P':
        cmd_print(atoi(param));
        break;
    case 'R':
        cmd_read(param);
        break;
    case 'W':
        cmd_write(param);
        break;
    case 'F':
        cmd_find(param);
        break;
    case 'S':
        cmd_sort(atoi(param));
        break;
    case 'D':
        cmd_del(atoi(param));
        break;
    case 'B':
        switch(cmd2) {
        case 'W':
            cmd_bin_write(param);
            break;
        case 'R':
            cmd_bin_read(param);
            break;
        }
        break;
    default:
        fprintf(stderr, "%%%c%c command is not defined.\n", cmd1, cmd2);
    }
    return;
}

void parse_line(char *line)
//入力に対して適切な関数を呼び出す//
{
    if(*line == '%') {
        exec_command(line[1], line[2], &line[3]);
    } else {
        new_profile(&profile_data_store[profile_data_nitems], line);
    }
    return;
}

////////////////////////////      cmd        ///////////////////////////////////
void cmd_quit()
//終了//
{
    exit(0);
    return;
}

void cmd_check()
//登録件数の表示//
{
    fprintf(stderr, "%d profile", profile_data_nitems);
    if(profile_data_nitems != 1) {
        fprintf(stderr, "s");
    }
    fprintf(stderr, "\n");
    return;
}

void cmd_print(int param)
  //データの表示//
{
    int i = 0, j = 1;
    if(profile_data_nitems == 0) { // データが0件のとき //
        fprintf(stderr, "Datas are not registered.\n");
        return;
    }
    if ((param > profile_data_nitems) || (param < (-1) * profile_data_nitems)) { //データの件数より大きい入力を受け付けたとき //
        fprintf(stderr, "This program has only %d datas now.\n\n", profile_data_nitems);
        param = 0;
    }
    if(param == 0) {
        param = profile_data_nitems;
    } else if(param < 0) {
        i = profile_data_nitems + param;
        param = profile_data_nitems;
    }
    for(; i < param; i++) {
        output_profile(i);
    }
    return;
}

void cmd_read(char *fn)
  //ファイルからの読み込み//
{
    FILE *fp;
    char data[65536];
    if((fp = fopen(fn, "r")) == NULL) {
//ファイルが見つからなかった場合//
        fprintf(stderr, "ERROR has occured.\n");
        return;
    }
    while(get_line(data, fp)) {
        parse_line(data);
    }
    fclose(fp);
    return;
}

void cmd_write(char *fn)
  //ファイルへの書き出し//
{
    FILE *fp;
    int i = 0;
    if(profile_data_nitems <= 0) {
//データが登録されていなければエラーメッセージを出力//
        fprintf(stderr, "This program has no datas.");
        return;
    }
    if(comp_str(fn, "\0", 0) == 1) {
//ファイル名が入力されていない場合//
        strcpy(fn, "output.csv");
    }
    if((fp = fopen(fn, "w")) == NULL) {
        printf("%s\n", fn);
        fprintf(stderr, "ERROR has occured.\n");
        return;
    }
    for(;i < profile_data_nitems; i++) {
        save_profile(i, fp);
    }
    printf("Saved : %s\n", fn);
    fclose(fp);
    return;
}

void cmd_find(char *param)
  //検索//
{
    int len = strlen(param), cnta, i = 0;
    for(cnta = 0; cnta < profile_data_nitems; cnta++) {
        if(com_str(param, cnta)) {
            output_profile(cnta);
            i++;
        }
    }
    if(i == 0) {
        printf("NONE\n");
    }
    return;
}

void cmd_sort(int param)
  //ソート//
{
    if(profile_data_nitems <= 0) {
        fprintf(stderr, "This program has no datas.");
        return;
    }
    quick_sort(0, profile_data_nitems - 1, param);
    return;
}

void cmd_complex_print(char *param)
  //指定された範囲の表示//
{
    int step, i, j;
    char *parameter[2];
    if(split(param, parameter, ',', 2) != 2) {
        fprintf(stderr, "Please Input 2 elements\n");
        return;
    }
    i = atoi(parameter[0]) - 1;
    j = atoi(parameter[1]) - 1;
    if(i <= j) {
        step = 1;
        i = func_max(0, i);
        j = func_min(j, profile_data_nitems);
    } else {
        step = -1;
        j = func_max(0, j);
        i = func_min(i, profile_data_nitems);
    }
    while(i != j) {
        output_profile(i);
        i += step;
    }
    output_profile(i);
    return;
}

void cmd_del(int i)
  //削除//
{
    if((0 >= i) || (i > profile_data_nitems)) {
        fprintf(stderr, "This program has only %d elements.\n", profile_data_nitems);
        return;
    }
    i--;
    while(i < profile_data_nitems) {
        profile_data_store[i] = profile_data_store[i + 1];
        i++;
    }
    profile_data_nitems--;
    return;
}

void cmd_bin_write(char *fn)
  //バイナリファイルの書き出し//
{
    FILE *fp;
    int i = 0, struct_size = 0;
    if(profile_data_nitems <= 0)
    {
        fprintf(stderr, "This program has no datas.\n");
        return;
    }
    if(comp_str(fn, "\0", 0) == 1) {
        strcpy(fn, "bin_output.dat");
    }
    if((fp = fopen(fn, "wb")) == NULL) {
        printf("%s\n", fn);
        fprintf(stderr, "ERROR has occured.\n");
        return;
    }
    output_10to16(fp);
    printf("Saved : %s\n", fn);
    fclose(fp);
    return;
}

void cmd_bin_read(char *fn)
  //バイナリファイルの読み込み//
{
    FILE *fp;
    char data[65536];
    if((fp = fopen(fn, "rb")) == NULL) {
        fprintf(stderr, "ERROR has occured.\n");
        return;
    }
    read_bin(fp);
    fclose(fp);
    return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void output_profile(int i)
  //データ1件表示//
{
    fprintf(stdout, "Id    : %d\nName  : %s\nBirth : %02d-%02d-%02d\nAddr  : %s\nCom.  : %s\n\n", profile_data_store[i].id, profile_data_store[i].school, profile_data_store[i].birth.y, profile_data_store[i].birth.m, profile_data_store[i].birth.d, profile_data_store[i].place, profile_data_store[i].sub);
    return;
}

int com_str(char *str, int i)
  //部分一致しているものがあれば1，そうでなければ0を返す//
{
    int cnt, cmp;
    char int_str[32 + 1];
    for(cnt = 0; profile_data_store[i].school[cnt] != '\0'; cnt++)
    {
        if(comp_str(str, profile_data_store[i].school, cnt) <= 1)
        {
            return 1;
        }
    }
    for(cnt = 0; profile_data_store[i].place[cnt] != '\0'; cnt++)
    {
        if(comp_str(str, profile_data_store[i].place, cnt) <= 1)
        {
            return 1;
        }
    }
    for(cnt = 0; profile_data_store[i].sub[cnt] != '\0'; cnt++)
    {
        if(comp_str(str, profile_data_store[i].sub, cnt) <= 1)
        {
            return 1;
        }
    }
    sprintf(int_str, "%d", profile_data_store[i].id);
    for(cnt = 0; int_str[cnt] != '\0'; cnt++)
    {
        if(comp_str(str, int_str, cnt) <= 1)
        {
            return 1;
        }
    }
    sprintf(int_str, "%02d-%02d-%02d", profile_data_store[i].birth.y, profile_data_store[i].birth.m, profile_data_store[i].birth.d);
    for(cnt = 0; int_str[cnt] != '\0'; cnt++)
    {
        if(comp_str(str, int_str, cnt) <= 1)
        {
            return 1;
        }
    }
    sprintf(int_str, "%d-%d-%d", profile_data_store[i].birth.y, profile_data_store[i].birth.m, profile_data_store[i].birth.d);
    for(cnt = 0; int_str[cnt] != '\0'; cnt++)
    {
        if(comp_str(str, int_str, cnt) <= 1)
        {
            return 1;
        }
    }
    return 0;
}

int comp_str(char *str1, char *str2, int cnt)
  //文字列の大小関係等を返す//
{
    int cnta, cmp;
    for(cnta = cnt, cmp = 0;; cnta++)
    {
        cmp = 0;
        if(str1[cnta - cnt] == '\0') {
            if(str2[cnta] == '\0') {
                cmp = 1;
            }
            break;
        }
        if (str2[cnta] == '\0') {
            cmp = 3;
            break;
        }
        if(str2[cnta] < str1[cnta - cnt]) {
            cmp = 3;
            break;
            }
        if(str2[cnta] != str1[cnta - cnt]) {
            cmp = 2;
            break;
        }
    }
    return cmp;
}

void swap_struct(int a, int b)
  //構造体の交換//
{
    struct profile *A, *B, c;
    A = &profile_data_store[a];
    B = &profile_data_store[b];
    c = *A;
    *A = *B;
    *B = c;
    return;
}

void quick_sort(int s, int e, int param)
  //クイックソート//
{
    if(s == e) {
        return;
    }
    int base = s + (e - s) / 2, front = s, back = e;
    switch(param) {
        case 1:
            while(1)
            {
                for(; (e > front) && (profile_data_store[front].id < profile_data_store[base].id); front++);
                for(; (s < back) && (profile_data_store[back].id >= profile_data_store[base].id); back--);
                if(call_quick(s, e, front, back, param))
                {
                    break;
                }
            }
            break;

    case 2:
        while(1)
        {
            for(front = s; (front < back) && ((comp_str(profile_data_store[front].school, profile_data_store[base].school, 0) == 2) || (comp_str(profile_data_store[front].school, profile_data_store[base].school, 0) == 0)); front++);
            for(back = e; (front < back) && ((comp_str(profile_data_store[back].school, profile_data_store[base].school, 0) == 1) || (comp_str(profile_data_store[back].school, profile_data_store[base].school, 0) == 3)); back--);
            if(call_quick(s, e, front, back, param))
            {
                break;
            }
        }
        break;

    case 3:
        while(1)
        {
            for(front = s; (front < back) && (sum_date(front) < sum_date(base)); front++);
            for(back = e; (back > front) && (sum_date(back) >= sum_date(base)); back--);
            if(call_quick(s, e, front, back, param))
            {
                break;
            }
        }
        break;

    case 4:
        while(1)
        {
            for(front = s; (front < back) && ((comp_str(profile_data_store[front].place, profile_data_store[base].place, 0) == 2) || (comp_str(profile_data_store[front].place, profile_data_store[base].place, 0) == 0)); front++);
            for(back = e; (front < back) && ((comp_str(profile_data_store[back].place, profile_data_store[base].place, 0) == 1) || (comp_str(profile_data_store[back].place, profile_data_store[base].place, 0) == 3)); back--);
            if(call_quick(s, e, front, back, param))
            {
                break;
            }
        }
        break;

    case 5:
        while(1)
        {
            for(front = s; (front < back) && ((comp_str(profile_data_store[front].sub, profile_data_store[base].sub, 0) == 2) || (comp_str(profile_data_store[front].sub, profile_data_store[base].sub, 0) == 0)); front++);
            for(back = e; (front < back) && ((comp_str(profile_data_store[back].sub, profile_data_store[base].sub, 0) == 1) || (comp_str(profile_data_store[back].sub, profile_data_store[base].sub, 0) == 3)); back--);
           if(call_quick(s, e, front, back, param))
            {
                break;
            }
        }
        break;
    default:
        fprintf(stderr, "This element is undefined.");
    }
    return;
}

int call_quick(int s, int e, int front, int back, int param)
  //交換できれば行う，できなければクイックソート関数を再度呼び出す//
{
    if(back <= front) {
        swap_struct(front, s + (e - s) / 2);
        quick_sort(s, front, param);
        quick_sort(front + 1, e, param);
        return 1;
    } else {
        swap_struct(front, back);
    }
    return 0;
}

int sum_date(int i)
  //ソート用に日付を1つの数字として出力する//
{
    return (profile_data_store[i].birth.y) * 10000 + (profile_data_store[i].birth.m) * 100 + (profile_data_store[i].birth.d);
}

void save_profile(int i, FILE *fp)
  //csv形式で1件書き出し//
{
    fprintf(fp, "%d,%s,%d-%d-%d,%s,%s\n", profile_data_store[i].id, profile_data_store[i].school, profile_data_store[i].birth.y, profile_data_store[i].birth.m, profile_data_store[i].birth.d, profile_data_store[i].place, profile_data_store[i].sub);
    return;
}

void output_10to16(FILE *fp)
{
//バイナリデータの書き出し//
    int i, struct_size, sum;
    for(i = 0; i < profile_data_nitems; i++)
    {
        struct_size = sizeof(profile_data_store[i].id);
        fwrite(&struct_size, sizeof(int), 1, fp);
        fwrite(&profile_data_store[i].id, struct_size, 1, fp);
        struct_size = strlen(profile_data_store[i].school)+1;
        fwrite(&struct_size, sizeof(int), 1, fp);
        fwrite(&profile_data_store[i].school, struct_size, 1, fp);
        sum = sum_date(i);
        struct_size = sizeof(sum);
        fwrite(&struct_size, sizeof(int), 1, fp);
        fwrite(&sum, struct_size, 1, fp);
        struct_size = strlen(profile_data_store[i].place) + 1;
        fwrite(&struct_size, sizeof(int), 1, fp);
        fwrite(&profile_data_store[i].place, struct_size, 1, fp);
        struct_size = strlen(profile_data_store[i].sub) + 1;
        fwrite(&struct_size, sizeof(int), 1, fp);
        fwrite(profile_data_store[i].sub, struct_size, 1, fp);
    }
    return;
}

void read_bin(FILE *fp)
{
//バイナリデータの読み込み//
    int struct_size, sum;
    while(fread(&struct_size, sizeof(int), 1, fp))
    {
        fread(&profile_data_store[profile_data_nitems].id, struct_size, 1, fp);

        fread(&struct_size, struct_size, 1, fp);
        fread(profile_data_store[profile_data_nitems].school, sizeof(char), struct_size / sizeof(char) , fp);
        struct_size = 0;
        fread(&struct_size, sizeof(int), 1, fp);
        fread(&sum, struct_size, 1, fp);
        profile_data_store[profile_data_nitems].birth.y = sum / 10000;
        profile_data_store[profile_data_nitems].birth.m = (sum%10000)/100;
        profile_data_store[profile_data_nitems].birth.d = (sum%10000)%100;

        fread(&struct_size, sizeof(int), 1, fp);
        fread(profile_data_store[profile_data_nitems].place, sizeof(char), struct_size / sizeof(char), fp);
        fread(&struct_size, sizeof(int), 1, fp);
        (profile_data_store[profile_data_nitems].sub) = (char *)malloc(struct_size * sizeof(char) + 1);
        if(struct_size > MAX_LEN) 
        {
            MAX_LEN = struct_size;
        }
        fread(profile_data_store[profile_data_nitems].sub, sizeof(char), struct_size / sizeof(char), fp);
        profile_data_nitems++;
    }
    return;
}
