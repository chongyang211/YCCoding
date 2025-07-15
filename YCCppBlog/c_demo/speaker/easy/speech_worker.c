//
// Created by 杨充 on 2025/7/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

// ====================== 常量定义 ======================
#define MAX_PLAYERS 12
#define PLAYER_ID_START 10001
#define GROUP_SIZE 6
#define FINALISTS 3
#define MAX_NAME_LEN 50
#define RECORD_FILE "speech_records.csv"
#define BUFFER_SIZE 1024

// ====================== 结构体定义 ======================

// 选手结构体
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    double score;
    bool eliminated;
} Player;

// 比赛记录结构体
typedef struct {
    int year;
    int first_place;
    int second_place;
    int third_place;
} Record;

// 比赛系统结构体
typedef struct {
    Player players[MAX_PLAYERS];
    Record records[100];
    int record_count;
} SpeechContestSystem;

// ====================== 函数声明 ======================

// 系统初始化
void init_system(SpeechContestSystem *system);
void generate_players(Player *players);

// 比赛流程
void start_contest(SpeechContestSystem *system);
void first_round(Player *players, Player **winners);
void second_round(Player *finalists);
void group_competition(Player *group, int size);
void calculate_scores(Player *player);

// 记录管理
void load_records(SpeechContestSystem *system);
void save_records(SpeechContestSystem *system);
void view_records(SpeechContestSystem *system);
void clear_records(SpeechContestSystem *system);
void add_record(SpeechContestSystem *system, int first, int second, int third);

// 工具函数
void shuffle_players(Player *players, int n);
void sort_players(Player *players, int n);
void display_players(Player *players, int n, const char *title);
void display_finalists(Player *finalists);
void press_any_key();
void clear_screen();
int get_current_year();
void print_header(const char *title);
void print_line(char c, int length);

// 菜单系统
void main_menu(SpeechContestSystem *system);
void display_menu();

// ====================== 函数实现 ======================

// 初始化比赛系统
void init_system(SpeechContestSystem *system) {
    srand((unsigned int)time(NULL));
    memset(system, 0, sizeof(SpeechContestSystem));
    generate_players(system->players);
    load_records(system);
}

// 生成选手数据
void generate_players(Player *players) {
    const char *first_names[] = {"张", "王", "李", "赵", "陈", "刘", "杨", "黄", "周", "吴", "郑", "孙"};
    const char *last_names[] = {"伟", "芳", "娜", "秀英", "敏", "静", "丽", "强", "磊", "军", "洋", "艳"};

    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].id = PLAYER_ID_START + i;

        // 生成随机中文名
        const char *first = first_names[rand() % (sizeof(first_names) / sizeof(first_names[0]))];
        const char *last = last_names[rand() % (sizeof(last_names) / sizeof(last_names[0]))];
        snprintf(players[i].name, MAX_NAME_LEN, "%s%s", first, last);

        players[i].score = 0.0;
        players[i].eliminated = false;
    }
}

// 加载比赛记录
void load_records(SpeechContestSystem *system) {
    FILE *file = fopen(RECORD_FILE, "r");
    if (!file) return;

    char buffer[BUFFER_SIZE];
    system->record_count = 0;

    while (fgets(buffer, BUFFER_SIZE, file) && system->record_count < 100) {
        int year, first, second, third;
        if (sscanf(buffer, "%d,%d,%d,%d", &year, &first, &second, &third) == 4) {
            system->records[system->record_count].year = year;
            system->records[system->record_count].first_place = first;
            system->records[system->record_count].second_place = second;
            system->records[system->record_count].third_place = third;
            system->record_count++;
        }
    }

    fclose(file);
}

// 保存比赛记录
void save_records(SpeechContestSystem *system) {
    FILE *file = fopen(RECORD_FILE, "w");
    if (!file) {
        perror("无法保存记录");
        return;
    }

    for (int i = 0; i < system->record_count; i++) {
        fprintf(file, "%d,%d,%d,%d\n",
                system->records[i].year,
                system->records[i].first_place,
                system->records[i].second_place,
                system->records[i].third_place);
    }

    fclose(file);
}

// 添加比赛记录
void add_record(SpeechContestSystem *system, int first, int second, int third) {
    if (system->record_count >= 100) {
        printf("记录已满，无法添加新记录\n");
        return;
    }

    system->records[system->record_count].year = get_current_year();
    system->records[system->record_count].first_place = first;
    system->records[system->record_count].second_place = second;
    system->records[system->record_count].third_place = third;
    system->record_count++;

    save_records(system);
}

// 查看比赛记录
void view_records(SpeechContestSystem *system) {
    clear_screen();
    print_header("往届比赛记录");

    if (system->record_count == 0) {
        printf("\n暂无比赛记录\n");
    } else {
        printf("%-8s %-8s %-8s %-8s\n", "年份", "冠军", "亚军", "季军");
        print_line('-', 40);

        for (int i = 0; i < system->record_count; i++) {
            printf("%-8d %-8d %-8d %-8d\n",
                   system->records[i].year,
                   system->records[i].first_place,
                   system->records[i].second_place,
                   system->records[i].third_place);
        }
    }

    press_any_key();
}

// 清空比赛记录
void clear_records(SpeechContestSystem *system) {
    clear_screen();
    print_header("清空比赛记录");

    printf("确定要清空所有比赛记录吗？(y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (tolower(choice) == 'y') {
        system->record_count = 0;
        save_records(system);
        printf("\n所有比赛记录已清空！\n");
    } else {
        printf("\n操作已取消\n");
    }

    press_any_key();
}

// 开始比赛
void start_contest(SpeechContestSystem *system) {
    clear_screen();
    print_header("开始新比赛");

    // 重置选手状态
    for (int i = 0; i < MAX_PLAYERS; i++) {
        system->players[i].score = 0.0;
        system->players[i].eliminated = false;
    }

    // 显示所有选手
    display_players(system->players, MAX_PLAYERS, "所有参赛选手");
    press_any_key();

    // 第一轮比赛
    Player *winners[2 * FINALISTS]; // 两个小组的前三名
    first_round(system->players, winners);

    // 准备决赛选手
    Player finalists[FINALISTS * 2];
    for (int i = 0; i < FINALISTS * 2; i++) {
        finalists[i] = *winners[i];
    }

    // 第二轮比赛
    second_round(finalists);

    // 添加比赛记录
    add_record(system, finalists[0].id, finalists[1].id, finalists[2].id);
}

// 第一轮比赛
void first_round(Player *players, Player **winners) {
    clear_screen();
    print_header("第一轮比赛 - 淘汰赛");

    // 随机洗牌
    shuffle_players(players, MAX_PLAYERS);

    // 分成两个小组
    Player groupA[GROUP_SIZE];
    Player groupB[GROUP_SIZE];

    memcpy(groupA, players, sizeof(Player) * GROUP_SIZE);
    memcpy(groupB, players + GROUP_SIZE, sizeof(Player) * GROUP_SIZE);

    // 小组A比赛
    printf("\n===== 第一组比赛 =====\n");
    display_players(groupA, GROUP_SIZE, "第一组选手");
    press_any_key();

    group_competition(groupA, GROUP_SIZE);
    sort_players(groupA, GROUP_SIZE);

    printf("\n===== 第一组比赛结果 =====\n");
    display_players(groupA, GROUP_SIZE, "第一组比赛结果");

    // 记录晋级选手
    winners[0] = &groupA[0];
    winners[1] = &groupA[1];
    winners[2] = &groupA[2];

    printf("\n晋级选手: %s, %s, %s\n",
           groupA[0].name, groupA[1].name, groupA[2].name);
    press_any_key();

    // 小组B比赛
    clear_screen();
    printf("\n===== 第二组比赛 =====\n");
    display_players(groupB, GROUP_SIZE, "第二组选手");
    press_any_key();

    group_competition(groupB, GROUP_SIZE);
    sort_players(groupB, GROUP_SIZE);

    printf("\n===== 第二组比赛结果 =====\n");
    display_players(groupB, GROUP_SIZE, "第二组比赛结果");

    // 记录晋级选手
    winners[3] = &groupB[0];
    winners[4] = &groupB[1];
    winners[5] = &groupB[2];

    printf("\n晋级选手: %s, %s, %s\n",
           groupB[0].name, groupB[1].name, groupB[2].name);
    press_any_key();

    // 显示所有晋级选手
    clear_screen();
    print_header("第一轮晋级选手");

    Player round_winners[FINALISTS * 2];
    for (int i = 0; i < FINALISTS * 2; i++) {
        round_winners[i] = *winners[i];
    }

    display_players(round_winners, FINALISTS * 2, "晋级决赛选手");
    press_any_key();
}

// 第二轮比赛
void second_round(Player *finalists) {
    clear_screen();
    print_header("第二轮比赛 - 决赛");

    // 随机洗牌
    shuffle_players(finalists, FINALISTS * 2);

    // 显示决赛选手
    display_players(finalists, FINALISTS * 2, "决赛选手");
    press_any_key();

    // 进行决赛
    group_competition(finalists, FINALISTS * 2);
    sort_players(finalists, FINALISTS * 2);

    // 标记淘汰选手
    for (int i = FINALISTS; i < FINALISTS * 2; i++) {
        finalists[i].eliminated = true;
    }

    // 显示决赛结果
    printf("\n===== 决赛结果 =====\n");
    display_finalists(finalists);

    // 显示前三名
    printf("\n🏆 冠军: %s (%.2f分)\n", finalists[0].name, finalists[0].score);
    printf("🥈 亚军: %s (%.2f分)\n", finalists[1].name, finalists[1].score);
    printf("🥉 季军: %s (%.2f分)\n", finalists[2].name, finalists[2].score);

    press_any_key();
}

// 小组比赛
void group_competition(Player *group, int size) {
    for (int i = 0; i < size; i++) {
        calculate_scores(&group[i]);
    }
}

// 计算选手得分
void calculate_scores(Player *player) {
    // 模拟评委打分 (5位评委)
    double scores[5];

    for (int i = 0; i < 5; i++) {
        // 基础分 + 随机波动
        scores[i] = 80.0 + (rand() % 200) / 10.0;
    }

    // 去掉最高分和最低分
    double min = scores[0], max = scores[0], sum = 0;
    for (int i = 0; i < 5; i++) {
        if (scores[i] < min) min = scores[i];
        if (scores[i] > max) max = scores[i];
        sum += scores[i];
    }

    player->score = (sum - min - max) / 3.0;
}

// 随机洗牌
void shuffle_players(Player *players, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() % (n - i);
        Player temp = players[i];
        players[i] = players[j];
        players[j] = temp;
    }
}

// 选手排序 (按分数降序)
void sort_players(Player *players, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (players[j].score < players[j + 1].score) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }
}

// 显示选手列表
void display_players(Player *players, int n, const char *title) {
    printf("\n%s:\n", title);
    print_line('-', 60);
    printf("%-8s %-10s %-8s %-10s\n", "编号", "姓名", "得分", "状态");
    print_line('-', 60);

    for (int i = 0; i < n; i++) {
        const char *status = players[i].eliminated ? "淘汰" : "晋级";
        printf("%-8d %-10s %-8.2f %-10s\n",
               players[i].id, players[i].name, players[i].score, status);
    }
}

// 显示决赛选手
void display_finalists(Player *finalists) {
    printf("\n决赛结果:\n");
    print_line('-', 60);
    printf("%-8s %-10s %-8s %-10s\n", "排名", "姓名", "得分", "状态");
    print_line('-', 60);

    const char *ranks[] = {"🥇", "🥈", "🥉", "4", "5", "6"};

    for (int i = 0; i < FINALISTS * 2; i++) {
        const char *status = finalists[i].eliminated ? "淘汰" : "胜出";
        printf("%-8s %-10s %-8.2f %-10s\n",
               ranks[i], finalists[i].name, finalists[i].score, status);
    }
}

// 工具函数：按任意键继续
void press_any_key() {
    printf("\n按任意键继续...");
    getchar();
    while (getchar() != '\n');
}

// 工具函数：清屏
void clear_screen() {
    system("clear");
}

// 工具函数：获取当前年份
int get_current_year() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return tm->tm_year + 1900;
}

// 工具函数：打印标题
void print_header(const char *title) {
    printf("\n");
    print_line('=', strlen(title) + 8);
    printf("=== %s ===\n", title);
    print_line('=', strlen(title) + 8);
}

// 工具函数：打印分隔线
void print_line(char c, int length) {
    for (int i = 0; i < length; i++) {
        putchar(c);
    }
    putchar('\n');
}

// 主菜单
void main_menu(SpeechContestSystem *system) {
    int choice;

    do {
        clear_screen();
        print_header("校园演讲比赛系统");

        display_menu();

        printf("\n请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                start_contest(system);
                break;
            case 2:
                view_records(system);
                break;
            case 3:
                clear_records(system);
                break;
            case 4:
                printf("\n感谢使用，再见！\n");
                exit(0);
            default:
                printf("\n无效选择，请重新输入！\n");
                press_any_key();
        }
    } while (choice != 4);
}

// 显示菜单
void display_menu() {
    printf("\n主菜单:\n");
    print_line('-', 40);
    printf("1. 开始演讲比赛\n");
    printf("2. 查看往届记录\n");
    printf("3. 清空比赛记录\n");
    printf("4. 退出比赛程序\n");
    print_line('-', 40);
}

// ====================== 主函数 ======================
int main() {
    SpeechContestSystem system;
    init_system(&system);
    main_menu(&system);
    return 0;
}