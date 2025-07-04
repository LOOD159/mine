#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  // 用于getch()
#define MAX_STUDENTS 100
#define MAX_NAME_LEN 20
#define MAX_SUBJECTS 4  // 语文数学和英文和计算机四门课程
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
    printf("语文成绩: %.1f\n", stu.scores[0]);
    printf("数学成绩: %.1f\n", stu.scores[1]);
    printf("语文数学和英文成绩: %.1f\n", stu.scores[2]);
    printf("计算机成绩: %.1f\n", stu.scores[3]);
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
// 菜单函数
void showMainMenu();
void showUserMenu();
void showAddStudentMenu();
void showSearchMenu();
void showSortMenu();

// 辅助函数
void clearInputBuffer();
void pressAnyKeyToContinue();

int main() {
    System sys;
    initSystem(&sys);
    loadFromFile(&sys, DATA_FILE); // 启动时加载数据
    
    int choice;
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
    
    while(1) {
        if(!sys.loggedIn) {
            showMainMenu();
            scanf("%d", &choice);
            clearInputBuffer();
            
            switch(choice) {
                case 1: // 注册
                    printf("\n=== 用户注册 ===\n");
                    printf("请输入用户名: ");
                    scanf("%s", username);
                    printf("请输入密码: ");
                    scanf("%s", password);
                    clearInputBuffer();
                    
                    if(registerUser(&sys, username, password)) {
                        printf("\n注册成功!\n");
                    } else {
                        printf("\n注册失败! 用户名可能已存在。\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                    
                case 2: // 登录
                    printf("\n=== 用户登录 ===\n");
                    printf("请输入用户名: ");
                    scanf("%s", username);
                    printf("请输入密码: ");
                    scanf("%s", password);
                    clearInputBuffer();
                    
                    if(login(&sys, username, password)) {
                        printf("\n登录成功! 欢迎, %s!\n", username);
                    } else {
                        printf("\n登录失败! 用户名或密码错误。\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                    
                case 3: // 退出
                    saveToFile(&sys, DATA_FILE); // 退出前保存
                    printf("\n感谢使用学生信息管理系统!\n");
                    return 0;
                    
                default:
                    printf("\n无效选择! 请重新输入。\n");
                    pressAnyKeyToContinue();
            }
        } else {
            showUserMenu();
            scanf("%d", &choice);
            clearInputBuffer();
            
            switch(choice) {
                case 1: { // 添加学生
                    Student newStu;
                    printf("\n=== 添加学生 ===\n");
                    
                    printf("请输入学号: ");
                    scanf("%d", &newStu.id);
                    printf("请输入姓名: ");
                    scanf("%s", newStu.name);
                    printf("请输入年龄: ");
                    scanf("%d", &newStu.age);
                    printf("请输入语文成绩: ");
                    scanf("%f", &newStu.scores[0]);
                    printf("请输入数学成绩: ");
                    scanf("%f", &newStu.scores[1]);
                    printf("请输入语文数学和英文成绩: ");
                    scanf("%f", &newStu.scores[2]);
                    printf("请输入计算机成绩: ");
                    scanf("%f", &newStu.scores[3]);
                    clearInputBuffer();
                    
                    if(addStudent(&sys, newStu)) {
                        printf("\n添加学生成功!\n");
                        displayStudent(newStu);
                    } else {
                        printf("\n添加失败! 学号可能已存在或已达到最大学生数。\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 2: { // 删除学生
                    int id;
                    printf("\n=== 删除学生 ===\n");
                    printf("请输入要删除的学生学号: ");
                    scanf("%d", &id);
                    clearInputBuffer();
                    
                    if(deleteStudent(&sys, id)) {
                        printf("\n删除成功!\n");
                    } else {
                        printf("\n删除失败! 未找到该学号的学生。\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 3: { // 修改学生信息
                    int id;
                    Student newStu;
                    printf("\n=== 修改学生信息 ===\n");
                    printf("请输入要修改的学生学号: ");
                    scanf("%d", &id);
                    clearInputBuffer();
                    
                    Student* stu = findStudent(&sys, id);
                    if(stu == NULL) {
                        printf("\n未找到该学号的学生!\n");
                        pressAnyKeyToContinue();
                        break;
                    }
                    
                    printf("\n当前学生信息:\n");
                    displayStudent(*stu);
                    
                    printf("\n请输入新的姓名(当前: %s): ", stu->name);
                    fgets(newStu.name, MAX_NAME_LEN, stdin);
                    newStu.name[strcspn(newStu.name, "\n")] = 0;
                    if(strlen(newStu.name) == 0) {
                        strcpy(newStu.name, stu->name);
                    }
                    
                    printf("请输入新的年龄(当前: %d): ", stu->age);
                    char ageStr[10];
                    fgets(ageStr, 10, stdin);
                    if(strlen(ageStr) > 1) {
                        newStu.age = atoi(ageStr);
                    } else {
                        newStu.age = stu->age;
                    }
                    
                    printf("请输入新的语文数学和英文成绩(当前: %.1f): ", stu->scores[0]);
                    char score1Str[10];
                    fgets(score1Str, 10, stdin);
                    if(strlen(score1Str) > 1) {
                        newStu.scores[0] = atof(score1Str);
                    } else {
                        newStu.scores[0] = stu->scores[0];
                    }
                    
                    printf("请输入新的计算机成绩(当前: %.1f): ", stu->scores[1]);
                    char score2Str[10];
                    fgets(score2Str, 10, stdin);
                    if(strlen(score2Str) > 1) {
                        newStu.scores[1] = atof(score2Str);
                    } else {
                        newStu.scores[1] = stu->scores[1];
                    }
                    
                    if(updateStudent(&sys, id, newStu)) {
                        printf("\n修改成功!\n");
                        displayStudent(newStu);
                    } else {
                        printf("\n修改失败!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 4: { // 查询学生
                    int id;
                    printf("\n=== 查询学生 ===\n");
                    printf("请输入要查询的学生学号: ");
                    scanf("%d", &id);
                    clearInputBuffer();
                    
                    Student* stu = findStudent(&sys, id);
                    if(stu != NULL) {
                        printf("\n查询结果:\n");
                        displayStudent(*stu);
                    } else {
                        printf("\n未找到该学号的学生!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 5: { // 成绩排序
                    int sortChoice;
                    showSortMenu();
                    scanf("%d", &sortChoice);
                    clearInputBuffer();
                    
                    switch(sortChoice) {
                        case 1:
                            sortByTotalScore(&sys);
                            printf("\n已按总成绩排序!\n");
                            break;
                        case 2:
                            sortBySubjectScore(&sys, 0);
                            printf("\n已按语文数学和英文成绩排序!\n");
                            break;
                        case 3:
                            sortBySubjectScore(&sys, 1);
                            printf("\n已按计算机成绩排序!\n");
                            break;
                        default:
                            printf("\n无效选择!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                }
                    
                case 6: // 统计人数
                    printf("\n当前系统中共有 %d 名学生。\n", getStudentCount(&sys));
                    pressAnyKeyToContinue();
                    break;
                    
                case 7: // 显示所有学生
                    displayAllStudents(&sys);
                    pressAnyKeyToContinue();
                    break;
                    
                case 8: // 保存数据
                    if(saveToFile(&sys, DATA_FILE)) {
                        printf("\n数据保存成功!\n");
                    } else {
                        printf("\n数据保存失败!\n");
                    }
                    pressAnyKeyToContinue();
                    break;
                    
                case 9: // 退出登录
                    logout(&sys);
                    printf("\n已退出登录!\n");
                    pressAnyKeyToContinue();
                    break;
                    
                default:
                    printf("\n无效选择! 请重新输入。\n");
                    pressAnyKeyToContinue();
            }
        }
    }
    
    return 0;
}

void showMainMenu() {
    printf("\n=== 学生信息管理系统 ===\n");
    printf("1. 注册\n");
    printf("2. 登录\n");
    printf("3. 退出\n");
    printf("请选择: ");
}

void showUserMenu() {
    printf("\n=== 主菜单 ===\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 修改学生信息\n");
    printf("4. 查询学生\n");
    printf("5. 成绩排序\n");
    printf("6. 统计人数\n");
    printf("7. 显示所有学生\n");
    printf("8. 保存数据\n");
    printf("9. 退出登录\n");
    printf("请选择: ");
}

void showSortMenu() {
    printf("\n=== 排序方式 ===\n");
    printf("1. 按总成绩排序\n");
    printf("2. 按语文数学和英文成绩排序\n");
    printf("3. 按计算机成绩排序\n");
    printf("请选择: ");
}

void clearInputBuffer() {
    while(getchar() != '\n');
}

void pressAnyKeyToContinue() {
    printf("\n按任意键继续...");
    getch();
    printf("\n");
}