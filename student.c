#include "student.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ��ʼ��ϵͳ
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
    printf("Ӣ��ɼ�: %.1f\n", stu.scores[0]);
    printf("������ɼ�: %.1f\n", stu.scores[1]);
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