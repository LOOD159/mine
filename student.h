#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 20
#define MAX_SUBJECTS 2  // 英语和计算机两门课程
#define USER_FILE "data/users.txt"
#define DATA_FILE "data/students.txt"

typedef struct {
    int id;                     // 学号
    char name[MAX_NAME_LEN];    // 姓名
    int age;                    // 年龄
    float scores[MAX_SUBJECTS]; // 各科成绩
    float total;                // 总成绩
    float average;              // 平均分
} Student;

typedef struct {
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
} User;

typedef struct {
    Student students[MAX_STUDENTS];
    int count;                  // 当前学生数量
    int loggedIn;               // 登录状态
    char currentUser[MAX_NAME_LEN]; // 当前用户
} System;

// 系统初始化
void initSystem(System *sys);

// 用户相关
int registerUser(System *sys, const char *username, const char *password);
int login(System *sys, const char *username, const char *password);
void logout(System *sys);

// 学生信息操作
int addStudent(System *sys, Student stu);
int deleteStudent(System *sys, int id);
int updateStudent(System *sys, int id, Student newStu);
Student* findStudent(System *sys, int id);

// 排序功能
void sortByTotalScore(System *sys);
void sortBySubjectScore(System *sys, int subjectIndex);

// 统计与显示
int getStudentCount(System *sys);
void displayStudent(Student stu);
void displayAllStudents(System *sys);

// 文件操作
int saveToFile(System *sys, const char *filename);
int loadFromFile(System *sys, const char *filename);
int saveUsers(User *users, int count);
int loadUsers(User *users, int *count);

#endif
