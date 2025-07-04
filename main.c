#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  // ����getch()
#define MAX_STUDENTS 100
#define MAX_NAME_LEN 20
#define MAX_SUBJECTS 4  // ������ѧ��Ӣ�ĺͼ�������ſγ�
#define USER_FILE "data/users.txt"
#define DATA_FILE "data/students.txt"

typedef struct {
    int id;                     // ѧ��
    char name[MAX_NAME_LEN];    // ����
    int age;                    // ����
    float scores[MAX_SUBJECTS]; // ���Ƴɼ�
    float total;                // �ܳɼ�
    float average;              // ƽ����
} Student;

typedef struct {
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
} User;

typedef struct {
    Student students[MAX_STUDENTS];
    int count;                  // ��ǰѧ������
    int loggedIn;               // ��¼״̬
    char currentUser[MAX_NAME_LEN]; // ��ǰ�û�
} System;
void initSystem(System *sys) {
    sys->count = 0;
    sys->loggedIn = 0;
    memset(sys->currentUser, 0, MAX_NAME_LEN);
}

// �û�ע��
int registerUser(System *sys, const char *username, const char *password) {
    User users[MAX_STUDENTS];
    int userCount = 0;
    
    // ���������û�
    loadUsers(users, &userCount);
    
    // ����û����Ƿ��Ѵ���
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0) {
            return 0;
        }
    }
    
    // ������û�
    if(userCount < MAX_STUDENTS) {
        strcpy(users[userCount].username, username);
        strcpy(users[userCount].password, password);
        userCount++;
        
        // �����û�����
        if(saveUsers(users, userCount)) {
            return 1;
        }
    }
    return 0;
}

// �û���¼
int login(System *sys, const char *username, const char *password) {
    User users[MAX_STUDENTS];
    int userCount = 0;
    
    loadUsers(users, &userCount);
    
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0 && 
           strcmp(users[i].password, password) == 0) {
            sys->loggedIn = 1;
            strcpy(sys->currentUser, username);
            return 1;
        }
    }
    return 0;
}

// �û�ע��
void logout(System *sys) {
    sys->loggedIn = 0;
    memset(sys->currentUser, 0, MAX_NAME_LEN);
}

// ���ѧ��
int addStudent(System *sys, Student stu) {
    if(sys->count >= MAX_STUDENTS) return 0;
    
    // ���ѧ���Ƿ��Ѵ���
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == stu.id) return 0;
    }
    
    // �����ֺܷ�ƽ����
    stu.total = 0;
    for(int i = 0; i < MAX_SUBJECTS; i++) {
        stu.total += stu.scores[i];
    }
    stu.average = stu.total / MAX_SUBJECTS;
    
    sys->students[sys->count++] = stu;
    return 1;
}

// ɾ��ѧ��
int deleteStudent(System *sys, int id) {
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == id) {
            // �����һ��Ԫ���Ƶ���ǰλ��
            sys->students[i] = sys->students[sys->count-1];
            sys->count--;
            return 1;
        }
    }
    return 0;
}

// ����ѧ����Ϣ
int updateStudent(System *sys, int id, Student newStu) {
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == id) {
            // ����ԭѧ��
            newStu.id = id;
            
            // ���¼����ֺܷ�ƽ����
            newStu.total = 0;
            for(int j = 0; j < MAX_SUBJECTS; j++) {
                newStu.total += newStu.scores[j];
            }
            newStu.average = newStu.total / MAX_SUBJECTS;
            
            sys->students[i] = newStu;
            return 1;
        }
    }
    return 0;
}

// ����ѧ��
Student* findStudent(System *sys, int id) {
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == id) {
            return &sys->students[i];
        }
    }
    return NULL;
}

// ���ܳɼ�����
void sortByTotalScore(System *sys) {
    for(int i = 0; i < sys->count - 1; i++) {
        for(int j = 0; j < sys->count - 1 - i; j++) {
            if(sys->students[j].total < sys->students[j+1].total) {
                Student temp = sys->students[j];
                sys->students[j] = sys->students[j+1];
                sys->students[j+1] = temp;
            }
        }
    }
}

// �����Ƴɼ�����
void sortBySubjectScore(System *sys, int subjectIndex) {
    if(subjectIndex < 0 || subjectIndex >= MAX_SUBJECTS) return;
    
    for(int i = 0; i < sys->count - 1; i++) {
        for(int j = 0; j < sys->count - 1 - i; j++) {
            if(sys->students[j].scores[subjectIndex] < sys->students[j+1].scores[subjectIndex]) {
                Student temp = sys->students[j];
                sys->students[j] = sys->students[j+1];
                sys->students[j+1] = temp;
            }
        }
    }
}

// ��ȡѧ������
int getStudentCount(System *sys) {
    return sys->count;
}

// ��ʾѧ����Ϣ
void displayStudent(Student stu) {
    printf("\nѧ��: %d\n", stu.id);
    printf("����: %s\n", stu.name);
    printf("����: %d\n", stu.age);
    printf("���ĳɼ�: %.1f\n", stu.scores[0]);
    printf("��ѧ�ɼ�: %.1f\n", stu.scores[1]);
    printf("������ѧ��Ӣ�ĳɼ�: %.1f\n", stu.scores[2]);
    printf("������ɼ�: %.1f\n", stu.scores[3]);
    printf("�ܳɼ�: %.1f\n", stu.total);
    printf("ƽ����: %.1f\n", stu.average);
    printf("----------------------------\n");
}

// ��ʾ����ѧ��
void displayAllStudents(System *sys) {
    if(sys->count == 0) {
        printf("\nû��ѧ����¼!\n");
        return;
    }
    
    printf("\n===== ����ѧ����Ϣ (%d��) =====\n", sys->count);
    for(int i = 0; i < sys->count; i++) {
        displayStudent(sys->students[i]);
    }
}

// ����ѧ�����ݵ��ļ�
int saveToFile(System *sys, const char *filename) {
    FILE *file = fopen(filename, "w");
    if(!file) return 0;
    
    for(int i = 0; i < sys->count; i++) {
        fprintf(file, "%d %s %d", sys->students[i].id, sys->students[i].name, sys->students[i].age);
        
        for(int j = 0; j < MAX_SUBJECTS; j++) {
            fprintf(file, " %.1f", sys->students[i].scores[j]);
        }
        
        fprintf(file, "\n");
    }
    
    fclose(file);
    return 1;
}

// ���ļ�����ѧ������
int loadFromFile(System *sys, const char *filename) {
    FILE *file = fopen(filename, "r");
    if(!file) return 0;
    
    sys->count = 0;
    while(!feof(file) && sys->count < MAX_STUDENTS) {
        Student stu;
        if(fscanf(file, "%d %s %d", &stu.id, stu.name, &stu.age) != 3) break;
        
        for(int j = 0; j < MAX_SUBJECTS; j++) {
            if(fscanf(file, "%f", &stu.scores[j]) != 1) {
                stu.scores[j] = 0;
            }
        }
        
        // �����ֺܷ�ƽ����
        stu.total = 0;
        for(int j = 0; j < MAX_SUBJECTS; j++) {
            stu.total += stu.scores[j];
        }
        stu.average = stu.total / MAX_SUBJECTS;
        
        sys->students[sys->count++] = stu;
    }
    
    fclose(file);
    return 1;
}

// �����û�����
int saveUsers(User *users, int count) {
    FILE *file = fopen(USER_FILE, "w");
    if(!file) return 0;
    
    for(int i = 0; i < count; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    
    fclose(file);
    return 1;
}

// �����û�����
int loadUsers(User *users, int *count) {
    FILE *file = fopen(USER_FILE, "r");
    if(!file) return 0;
    
    *count = 0;
    while(!feof(file) && *count < MAX_STUDENTS) {
        if(fscanf(file, "%s %s", users[*count].username, users[*count].password) == 2) {
            (*count)++;
        }
    }
    
    fclose(file);
    return 1;
}
// �˵�����
void showMainMenu();
void showUserMenu();
void showAddStudentMenu();
void showSearchMenu();
void showSortMenu();

// ��������
void clearInputBuffer();
void pressAnyKeyToContinue();

int main() {
    System sys;
    initSystem(&sys);
    loadFromFile(&sys, DATA_FILE); // ����ʱ��������
    
    int choice;
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
    
    while(1) {
        if(!sys.loggedIn) {
            showMainMenu();
            scanf("%d", &choice);
            clearInputBuffer();
            
            switch(choice) {
                case 1: // ע��
                    printf("\n=== �û�ע�� ===\n");
                    printf("�������û���: ");
                    scanf("%s", username);
                    printf("����������: ");
                    scanf("%s", password);
                    clearInputBuffer();
                    
                    if(registerUser(&sys, username, password)) {
                        printf("\nע��ɹ�!\n");
                    } else {
                        printf("\nע��ʧ��! �û��������Ѵ��ڡ�\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                    
                case 2: // ��¼
                    printf("\n=== �û���¼ ===\n");
                    printf("�������û���: ");
                    scanf("%s", username);
                    printf("����������: ");
                    scanf("%s", password);
                    clearInputBuffer();
                    
                    if(login(&sys, username, password)) {
                        printf("\n��¼�ɹ�! ��ӭ, %s!\n", username);
                    } else {
                        printf("\n��¼ʧ��! �û������������\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                    
                case 3: // �˳�
                    saveToFile(&sys, DATA_FILE); // �˳�ǰ����
                    printf("\n��лʹ��ѧ����Ϣ����ϵͳ!\n");
                    return 0;
                    
                default:
                    printf("\n��Чѡ��! ���������롣\n");
                    pressAnyKeyToContinue();
            }
        } else {
            showUserMenu();
            scanf("%d", &choice);
            clearInputBuffer();
            
            switch(choice) {
                case 1: { // ���ѧ��
                    Student newStu;
                    printf("\n=== ���ѧ�� ===\n");
                    
                    printf("������ѧ��: ");
                    scanf("%d", &newStu.id);
                    printf("����������: ");
                    scanf("%s", newStu.name);
                    printf("����������: ");
                    scanf("%d", &newStu.age);
                    printf("���������ĳɼ�: ");
                    scanf("%f", &newStu.scores[0]);
                    printf("��������ѧ�ɼ�: ");
                    scanf("%f", &newStu.scores[1]);
                    printf("������������ѧ��Ӣ�ĳɼ�: ");
                    scanf("%f", &newStu.scores[2]);
                    printf("�����������ɼ�: ");
                    scanf("%f", &newStu.scores[3]);
                    clearInputBuffer();
                    
                    if(addStudent(&sys, newStu)) {
                        printf("\n���ѧ���ɹ�!\n");
                        displayStudent(newStu);
                    } else {
                        printf("\n���ʧ��! ѧ�ſ����Ѵ��ڻ��Ѵﵽ���ѧ������\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 2: { // ɾ��ѧ��
                    int id;
                    printf("\n=== ɾ��ѧ�� ===\n");
                    printf("������Ҫɾ����ѧ��ѧ��: ");
                    scanf("%d", &id);
                    clearInputBuffer();
                    
                    if(deleteStudent(&sys, id)) {
                        printf("\nɾ���ɹ�!\n");
                    } else {
                        printf("\nɾ��ʧ��! δ�ҵ���ѧ�ŵ�ѧ����\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 3: { // �޸�ѧ����Ϣ
                    int id;
                    Student newStu;
                    printf("\n=== �޸�ѧ����Ϣ ===\n");
                    printf("������Ҫ�޸ĵ�ѧ��ѧ��: ");
                    scanf("%d", &id);
                    clearInputBuffer();
                    
                    Student* stu = findStudent(&sys, id);
                    if(stu == NULL) {
                        printf("\nδ�ҵ���ѧ�ŵ�ѧ��!\n");
                        pressAnyKeyToContinue();
                        break;
                    }
                    
                    printf("\n��ǰѧ����Ϣ:\n");
                    displayStudent(*stu);
                    
                    printf("\n�������µ�����(��ǰ: %s): ", stu->name);
                    fgets(newStu.name, MAX_NAME_LEN, stdin);
                    newStu.name[strcspn(newStu.name, "\n")] = 0;
                    if(strlen(newStu.name) == 0) {
                        strcpy(newStu.name, stu->name);
                    }
                    
                    printf("�������µ�����(��ǰ: %d): ", stu->age);
                    char ageStr[10];
                    fgets(ageStr, 10, stdin);
                    if(strlen(ageStr) > 1) {
                        newStu.age = atoi(ageStr);
                    } else {
                        newStu.age = stu->age;
                    }
                    
                    printf("�������µ�������ѧ��Ӣ�ĳɼ�(��ǰ: %.1f): ", stu->scores[0]);
                    char score1Str[10];
                    fgets(score1Str, 10, stdin);
                    if(strlen(score1Str) > 1) {
                        newStu.scores[0] = atof(score1Str);
                    } else {
                        newStu.scores[0] = stu->scores[0];
                    }
                    
                    printf("�������µļ�����ɼ�(��ǰ: %.1f): ", stu->scores[1]);
                    char score2Str[10];
                    fgets(score2Str, 10, stdin);
                    if(strlen(score2Str) > 1) {
                        newStu.scores[1] = atof(score2Str);
                    } else {
                        newStu.scores[1] = stu->scores[1];
                    }
                    
                    if(updateStudent(&sys, id, newStu)) {
                        printf("\n�޸ĳɹ�!\n");
                        displayStudent(newStu);
                    } else {
                        printf("\n�޸�ʧ��!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 4: { // ��ѯѧ��
                    int id;
                    printf("\n=== ��ѯѧ�� ===\n");
                    printf("������Ҫ��ѯ��ѧ��ѧ��: ");
                    scanf("%d", &id);
                    clearInputBuffer();
                    
                    Student* stu = findStudent(&sys, id);
                    if(stu != NULL) {
                        printf("\n��ѯ���:\n");
                        displayStudent(*stu);
                    } else {
                        printf("\nδ�ҵ���ѧ�ŵ�ѧ��!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 5: { // �ɼ�����
                    int sortChoice;
                    showSortMenu();
                    scanf("%d", &sortChoice);
                    clearInputBuffer();
                    
                    switch(sortChoice) {
                        case 1:
                            sortByTotalScore(&sys);
                            printf("\n�Ѱ��ܳɼ�����!\n");
                            break;
                        case 2:
                            sortBySubjectScore(&sys, 0);
                            printf("\n�Ѱ�������ѧ��Ӣ�ĳɼ�����!\n");
                            break;
                        case 3:
                            sortBySubjectScore(&sys, 1);
                            printf("\n�Ѱ�������ɼ�����!\n");
                            break;
                        default:
                            printf("\n��Чѡ��!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 6: // ͳ������
                    printf("\n��ǰϵͳ�й��� %d ��ѧ����\n", getStudentCount(&sys));
                    pressAnyKeyToContinue();
                    break;
                    
                case 7: // ��ʾ����ѧ��
                    displayAllStudents(&sys);
                    pressAnyKeyToContinue();
                    break;
                    
                case 8: // ��������
                    if(saveToFile(&sys, DATA_FILE)) {
                        printf("\n���ݱ���ɹ�!\n");
                    } else {
                        printf("\n���ݱ���ʧ��!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                    
                case 9: // �˳���¼
                    logout(&sys);
                    printf("\n���˳���¼!\n");
                    pressAnyKeyToContinue();
                    break;
                    
                default:
                    printf("\n��Чѡ��! ���������롣\n");
                    pressAnyKeyToContinue();
            }
        }
    }
    
    return 0;
}

void showMainMenu() {
    printf("\n=== ѧ����Ϣ����ϵͳ ===\n");
    printf("1. ע��\n");
    printf("2. ��¼\n");
    printf("3. �˳�\n");
    printf("��ѡ��: ");
}

void showUserMenu() {
    printf("\n=== ���˵� ===\n");
    printf("1. ���ѧ��\n");
    printf("2. ɾ��ѧ��\n");
    printf("3. �޸�ѧ����Ϣ\n");
    printf("4. ��ѯѧ��\n");
    printf("5. �ɼ�����\n");
    printf("6. ͳ������\n");
    printf("7. ��ʾ����ѧ��\n");
    printf("8. ��������\n");
    printf("9. �˳���¼\n");
    printf("��ѡ��: ");
}

void showSortMenu() {
    printf("\n=== ����ʽ ===\n");
    printf("1. ���ܳɼ�����\n");
    printf("2. ��������ѧ��Ӣ�ĳɼ�����\n");
    printf("3. ��������ɼ�����\n");
    printf("��ѡ��: ");
}

void clearInputBuffer() {
    while(getchar() != '\n');
}

void pressAnyKeyToContinue() {
    printf("\n�����������...");
    getch();
    printf("\n");
}