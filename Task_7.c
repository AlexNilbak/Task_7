#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  char Name[256];
  int Group;
  double Rating;
} Student;

int Get_number(FILE*);
int Read_list(FILE*, int, Student**);
int Write_list(FILE*, int*, int, Student**);
int Check_rating(int*, int, const double, Student**);
int Check_student(int*, int, int, Student**, Student**);
int Autotest(void);

int Get_number (FILE* input) {
    int i = 0;
    char string[256];
    while (fgets(string, sizeof(string), input) != NULL){
        i++;
    }
    rewind(input);
    return i;
}

int Read_list(FILE* data, int number, Student** list){
    Student* array;
    int group;
    char name[256];
    double rating;
    array = (Student*)malloc(number*sizeof(Student));
    for (int i= 0 ; i < number; i++){
        fscanf(data, "%s", &name);
        if (name[0]<'A' || name[0]>'Z'){
            printf("Wrong format\n");
            exit(1);
        }
        strcpy(array[i].Name, name);
        fscanf(data, "%s", &name);
        if (name[0]<'A' || name[0]>'Z'){
            printf("Wrong format\n");
            exit(1);
        }
        strcat(array[i].Name, " ");
        strcat(array[i].Name, name);

        fscanf(data, "%d", &group);
        if (group<100 || group>1000){
            printf("Wrong format\n");
            exit(2);
        }
        array[i].Group = group;

        fscanf(data, "%lf", &rating);
        if (rating>10.0){
            printf("Wrong format\n");
            exit(3);
        }
        array[i].Rating = rating;
    }
    for (int i = 0; i < number; i++){
        list[i] = &array[i];
    }
    return 0;
}


int Write_list(FILE* res, int* check, int number, Student** list){
    for (int i=0; i<number; i++){
        if (check[i]!=0){
            fprintf(res, "%s ", list[i]->Name);
            fprintf(res, "%d ", list[i]->Group);
            fprintf(res, "%lf", list[i]->Rating);
            fprintf(res, "\n");
        }
    }
    return 0;
}

int Check_rating(int* check, int number, const double R, Student** list){
    for (int i=0; i<number; i++){
        if(list[i]->Rating >= R){
            check[i]++;
        }
    }
    return 0;
}

int Check_student(int* check, int number1, int number2, Student** list1, Student** list2){
    int flag;
    int flajok;
    for (int i=0; i<number2; i++){
        for (int j=0; j<number1; j++){
            flag=0;
            if(strlen(list1[j]->Name) != strlen(list2[i]->Name)){
                    flag++;
            }
            else{
                flajok=0;
                for(int h=0; h<strlen(list1[j]->Name); h++){
                    if(list1[j]->Name[h] == list2[i]->Name[h]){
                        flajok++;
                    }
                }
                if(flajok!=strlen(list1[j]->Name)){
                    flag++;
                }

            }
            if(list1[j]->Group != list2[i]->Group){
                    flag++;
            }
            if((list1[j]->Rating < list2[i]->Rating) || (list1[j]->Rating > list2[i]->Rating)){
                    flag++;
            }
            if(flag==0){
                check[i]=0;
                j=number1;
            }
            else{
                check[i]++;
            }
        }
    }
    return 0;
}

int main(void){
    Student** list1;
    Student** list2;
    int number1;
    int number2;
    int p;
    int* check;
    FILE* students;
    FILE* deduct;
    FILE* res;
    double R=4.0;
    students = fopen("students.txt", "r");
    if (!students){
        printf("Cannot open file 'students.txt'\n");
        return -1;
    }
    deduct = fopen("deduct.txt", "r");
    if (!deduct){
        printf("Cannot open file 'deduct.txt'\n");
        return -1;
    }
    res = fopen("res.txt", "w");


    number1=Get_number(students);
    if (number1==0){
        printf("There are no students\n");
        return -1;
    }
    list1=(Student**)malloc(number1*sizeof(Student*));
    p=Read_list(students, number1, list1);

    number2=Get_number(deduct);
    if (number2==0){
        check=(int*)malloc(number1*sizeof(int));
        for (int i=0; i<number1; i++){
        check[i]=0;
        }
        p=Check_rating(check, number1, R, list1);
        p=0;
        for (int i=0; i<number1; i++){
            if (check[i]==0){
                check[i]++;
                p++;
            }
            else{
                check[i]=0;
            }
        }
        if(p>0){
            printf("Some students should be deducted...\n");
            Write_list(res, check, number1, list1);
            fclose(students);
            fclose(deduct);
            fclose(res);
            return 0;
        }
        else{
            printf("All the data is correct\n");
            fclose(students);
            fclose(deduct);
            fclose(res);
            return 0;
        }
    }
    list2=(Student**)malloc(number2*sizeof(Student*));
    p=Read_list(deduct, number2, list2);

    check=(int*)malloc(number2*sizeof(int));
    for (int i=0; i<number2; i++){
        check[i]=0;
    }

    p=Check_student(check, number1, number2, list1, list2);

    p=Check_rating(check, number2, R, list2);

    p=0;
    for (int i=0; i<number2; i++){
        if (check[i]>0){
            p++;
        }
    }
    if(p>0){
        printf("Some data is wrong...\n");
        Write_list(res, check, number2, list2);
    }
    else{
        printf("All the data is correct\n");
    }
    fclose(students);
    fclose(deduct);
    fclose(res);
    return 0;
}
