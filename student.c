#include "student.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// 初始化系统
void initSystem(System *sys) {
    sys->count = 0;
    sys->loggedIn = 0;
    memset(sys->currentUser, 0, MAX_NAME_LEN);
}

// 用户注册
int registerUser(System *sys, const char *username, const char *password) {
    User users[MAX_STUDENTS];
    int userCount = 0;
    
    // 加载已有用户
    loadUsers(users, &userCount);
    
    // 检查用户名是否已存在
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0) {
            return 0;
        }
    }
    
    // 添加新用户
    if(userCount < MAX_STUDENTS) {
        strcpy(users[userCount].username, username);
        strcpy(users[userCount].password, password);
        userCount++;
        
        // 保存用户数据
        if(saveUsers(users, userCount)) {
            return 1;
        }
    }
    return 0;
}

// 用户登录
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

// 用户注销
void logout(System *sys) {
    sys->loggedIn = 0;
    memset(sys->currentUser, 0, MAX_NAME_LEN);
}

// 添加学生
int addStudent(System *sys, Student stu) {
    if(sys->count >= MAX_STUDENTS) return 0;
    
    // 检查学号是否已存在
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == stu.id) return 0;
    }
    
    // 计算总分和平均分
    stu.total = 0;
    for(int i = 0; i < MAX_SUBJECTS; i++) {
        stu.total += stu.scores[i];
    }
    stu.average = stu.total / MAX_SUBJECTS;
    
    sys->students[sys->count++] = stu;
    return 1;
}

// 删除学生
int deleteStudent(System *sys, int id) {
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == id) {
            // 将最后一个元素移到当前位置
            sys->students[i] = sys->students[sys->count-1];
            sys->count--;
            return 1;
        }
    }
    return 0;
}

// 更新学生信息
int updateStudent(System *sys, int id, Student newStu) {
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == id) {
            // 保留原学号
            newStu.id = id;
            
            // 重新计算总分和平均分
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

// 查找学生
Student* findStudent(System *sys, int id) {
    for(int i = 0; i < sys->count; i++) {
        if(sys->students[i].id == id) {
            return &sys->students[i];
        }
    }
    return NULL;
}

// 按总成绩排序
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

// 按单科成绩排序
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

// 获取学生数量
int getStudentCount(System *sys) {
    return sys->count;
}

// 显示学生信息
void displayStudent(Student stu) {
    printf("\n学号: %d\n", stu.id);
    printf("姓名: %s\n", stu.name);
    printf("年龄: %d\n", stu.age);
    printf("英语成绩: %.1f\n", stu.scores[0]);
    printf("计算机成绩: %.1f\n", stu.scores[1]);
    printf("总成绩: %.1f\n", stu.total);
    printf("平均分: %.1f\n", stu.average);
    printf("----------------------------\n");
}

// 显示所有学生
void displayAllStudents(System *sys) {
    if(sys->count == 0) {
        printf("\n没有学生记录!\n");
        return;
    }
    
    printf("\n===== 所有学生信息 (%d人) =====\n", sys->count);
    for(int i = 0; i < sys->count; i++) {
        displayStudent(sys->students[i]);
    }
}

// 保存学生数据到文件
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

// 从文件加载学生数据
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
        
        // 计算总分和平均分
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

// 保存用户数据
int saveUsers(User *users, int count) {
    FILE *file = fopen(USER_FILE, "w");
    if(!file) return 0;
    
    for(int i = 0; i < count; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    
    fclose(file);
    return 1;
}

// 加载用户数据
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