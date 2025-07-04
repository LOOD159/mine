#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 20
#define MAX_SUBJECTS 2  // Ӣ��ͼ�������ſγ�
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

// ϵͳ��ʼ��
void initSystem(System *sys);

// �û����
int registerUser(System *sys, const char *username, const char *password);
int login(System *sys, const char *username, const char *password);
void logout(System *sys);

// ѧ����Ϣ����
int addStudent(System *sys, Student stu);
int deleteStudent(System *sys, int id);
int updateStudent(System *sys, int id, Student newStu);
Student* findStudent(System *sys, int id);

// ������
void sortByTotalScore(System *sys);
void sortBySubjectScore(System *sys, int subjectIndex);

// ͳ������ʾ
int getStudentCount(System *sys);
void displayStudent(Student stu);
void displayAllStudents(System *sys);

// �ļ�����
int saveToFile(System *sys, const char *filename);
int loadFromFile(System *sys, const char *filename);
int saveUsers(User *users, int count);
int loadUsers(User *users, int *count);

#endif
