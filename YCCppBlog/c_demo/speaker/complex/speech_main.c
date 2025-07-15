#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

// ====================== 常量定义 ======================
#define MAX_PLAYERS 12
#define PLAYER_ID_START 10001
#define GROUP_SIZE 6
#define FINALISTS 3
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 256
#define RECORDS_DIR "records"
#define HISTORY_FILE "history.log"
#define CONFIG_FILE "config.cfg"
#define MAX_RECORDS 1000
#define MAX_HISTORY_SIZE 100
#define SCORE_RANGE_MIN 70.0
#define SCORE_RANGE_MAX 95.0
#define JUDGES_COUNT 5
#define DELAY_BASE 800000  // 800毫秒

// ====================== 结构体定义 ======================

// 选手结构体
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int age;
    char department[30];    // 院系
    double score;
    bool eliminated;
    int ranking;
} Player;

// 比赛记录结构体
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int first_place;
    char first_name[MAX_NAME_LEN];
    double first_score;
    int second_place;
    char second_name[MAX_NAME_LEN];
    double second_score;
    int third_place;
    char third_name[MAX_NAME_LEN];
    double third_score;
} Record;

// 历史操作结构体
typedef struct {
    time_t timestamp;
    char operation[50];
    char details[100];
} HistoryEntry;

// 比赛系统结构体
typedef struct {
    Player players[MAX_PLAYERS];
    Record records[MAX_RECORDS];
    HistoryEntry history[MAX_HISTORY_SIZE];
    int record_count;
    int history_count;
    int total_contests;
    bool debug_mode;
    bool animations_enabled;
    bool sound_enabled;
    char record_dir[256];
} SpeechContestSystem;

// ====================== 全局变量 ======================
const char *DEPARTMENTS[] = {
    "计算机学院", "电子信息学院", "机械工程学院",
    "经济管理学院", "外国语学院", "生物医学院",
    "艺术学院", "法学院", "建筑学院"
};

const char *FIRST_NAMES[] = {
    "张", "王", "李", "赵", "陈", "刘",
    "杨", "黄", "周", "吴", "郑", "孙"
};

const char *LAST_NAMES[] = {
    "伟", "芳", "娜", "秀英", "敏", "静",
    "丽", "强", "磊", "军", "洋", "艳",
    "超", "勇", "杰", "婷", "雪", "颖"
};

// ====================== 函数声明 ======================

// 系统初始化
void init_system(SpeechContestSystem *system);
void generate_players(Player *players);
void load_config(SpeechContestSystem *system);
void save_config(SpeechContestSystem *system);
void ensure_dir_exists(const char *path);

// 比赛流程
void start_contest(SpeechContestSystem *system);
void first_round(SpeechContestSystem *system, Player **winners);
void second_round(SpeechContestSystem *system, Player *finalists);
void group_competition(SpeechContestSystem *system, Player *group, int size);
void calculate_scores(SpeechContestSystem *system, Player *player);
void display_results(Player *players, int count, const char *title, bool show_details);
void announce_winner(Player *player, int rank);

// 记录管理
void load_records(SpeechContestSystem *system);
void save_records(SpeechContestSystem *system);
void save_to_csv(SpeechContestSystem *system);
void view_records(SpeechContestSystem *system);
void clear_records(SpeechContestSystem *system);
void add_record(SpeechContestSystem *system, Player *finalists);
void view_statistics(SpeechContestSystem *system);
void archive_records(SpeechContestSystem *system);

// 历史记录
void log_history(SpeechContestSystem *system, const char *operation, const char *details);
void view_history(SpeechContestSystem *system);

// 配置管理
void settings_menu(SpeechContestSystem *system);
void display_settings(SpeechContestSystem *system);
void toggle_setting(int *setting, const char *setting_name);

// 工具函数
void shuffle_players(Player *players, int n);
void sort_players(Player *players, int n);
void sort_records(Record *records, int n);
void display_player_table(Player *players, int n, const char *title, bool show_details);
void display_record_table(Record *records, int n);
void display_finalists_table(Player *finalists);
void press_any_key();
void clear_screen();
void print_header(const char *title);
void print_footer(const char *message);
void print_line(char c, int length);
void animate_text(const char *text, int delay);
void play_sound(const char *sound_type);
void delay_ms(long milliseconds);
char *get_current_datetime_str();
void validate_system(SpeechContestSystem *system);
void backup_system(SpeechContestSystem *system);
void export_data_csv(SpeechContestSystem *system);

// 验证函数
int validate_player_id(int id);
int validate_player_name(const char *name);
int validate_player_age(int age);
int validate_player_department(const char *department);

// 菜单系统
void main_menu(SpeechContestSystem *system);
void display_main_menu();
void record_menu(SpeechContestSystem *system);

// ====================== 函数实现 ======================

// 确保目录存在
void ensure_dir_exists(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
    }
}

// 加载配置
void load_config(SpeechContestSystem *system) {
    FILE *file = fopen(CONFIG_FILE, "r");
    if (!file) {
        // 默认配置
        system->debug_mode = false;
        system->animations_enabled = true;
        system->sound_enabled = true;
        strcpy(system->record_dir, RECORDS_DIR);
        system->total_contests = 0;
        return;
    }

    fscanf(file, "debug_mode=%d\n", (int *)&system->debug_mode);
    fscanf(file, "animations=%d\n", (int *)&system->animations_enabled);
    fscanf(file, "sound=%d\n", (int *)&system->sound_enabled);
    fscanf(file, "total_contests=%d\n", &system->total_contests);
    fscanf(file, "record_dir=%255s\n", system->record_dir);

    fclose(file);
}

// 保存配置
void save_config(SpeechContestSystem *system) {
    FILE *file = fopen(CONFIG_FILE, "w");
    if (!file) {
        perror("无法保存配置");
        return;
    }

    fprintf(file, "debug_mode=%d\n", system->debug_mode);
    fprintf(file, "animations=%d\n", system->animations_enabled);
    fprintf(file, "sound=%d\n", system->sound_enabled);
    fprintf(file, "total_contests=%d\n", system->total_contests);
    fprintf(file, "record_dir=%s\n", system->record_dir);

    fclose(file);
}

// 初始化比赛系统
void init_system(SpeechContestSystem *system) {
    srand((unsigned int)time(NULL));
    memset(system, 0, sizeof(SpeechContestSystem));

    // 确保记录目录存在
    ensure_dir_exists(RECORDS_DIR);

    // 加载配置
    load_config(system);

    // 生成选手
    generate_players(system->players);

    // 加载记录
    load_records(system);

    // 添加初始化日志
    log_history(system, "系统初始化", "程序启动");
}

// 生成选手数据
void generate_players(Player *players) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].id = PLAYER_ID_START + i;

        // 生成随机中文名
        const char *first = FIRST_NAMES[rand() % (sizeof(FIRST_NAMES) / sizeof(FIRST_NAMES[0]))];
        const char *last = LAST_NAMES[rand() % (sizeof(LAST_NAMES) / sizeof(LAST_NAMES[0]))];
        snprintf(players[i].name, MAX_NAME_LEN, "%s%s", first, last);

        // 随机年龄 (18-22岁)
        players[i].age = 18 + rand() % 5;

        // 随机院系
        strcpy(players[i].department, DEPARTMENTS[rand() % (sizeof(DEPARTMENTS) / sizeof(DEPARTMENTS[0]))]);

        players[i].score = 0.0;
        players[i].eliminated = false;
        players[i].ranking = 0;
    }
}

// 日志历史
void log_history(SpeechContestSystem *system, const char *operation, const char *details) {
    if (system->history_count >= MAX_HISTORY_SIZE) {
        // 移除最旧的记录
        for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
            system->history[i] = system->history[i + 1];
        }
        system->history_count = MAX_HISTORY_SIZE - 1;
    }

    time(&system->history[system->history_count].timestamp);
    strncpy(system->history[system->history_count].operation, operation, 50);
    strncpy(system->history[system->history_count].details, details, 100);
    system->history_count++;

    // 保存到日志文件
    FILE *log = fopen(HISTORY_FILE, "a");
    if (log) {
        char time_str[50];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

        fprintf(log, "[%s] %s: %s\n", time_str, operation, details);
        fclose(log);
    }
}

// 加载比赛记录
void load_records(SpeechContestSystem *system) {
    char path[300];
    snprintf(path, sizeof(path), "%s/%s", system->record_dir, "records.dat");

    FILE *file = fopen(path, "rb");
    if (!file) {
        return;
    }

    // 读取记录数量
    fread(&system->record_count, sizeof(int), 1, file);

    // 读取记录
    if (system->record_count > MAX_RECORDS) {
        system->record_count = MAX_RECORDS;
    }

    for (int i = 0; i < system->record_count; i++) {
        fread(&system->records[i], sizeof(Record), 1, file);
    }

    fclose(file);
}

// 保存比赛记录
void save_records(SpeechContestSystem *system) {
    char path[300];
    snprintf(path, sizeof(path), "%s/%s", system->record_dir, "records.dat");

    FILE *file = fopen(path, "wb");
    if (!file) {
        perror("无法保存记录");
        return;
    }

    // 写入记录数量
    fwrite(&system->record_count, sizeof(int), 1, file);

    // 写入记录
    for (int i = 0; i < system->record_count; i++) {
        fwrite(&system->records[i], sizeof(Record), 1, file);
    }

    fclose(file);
}

// 添加到比赛记录
void add_record(SpeechContestSystem *system, Player *finalists) {
    if (system->record_count >= MAX_RECORDS) {
        printf("记录已满，无法添加新记录。正在归档旧记录...\n");
        archive_records(system);
    }

    Record *rec = &system->records[system->record_count];

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    rec->year = t->tm_year + 1900;
    rec->month = t->tm_mon + 1;
    rec->day = t->tm_mday;
    rec->hour = t->tm_hour;
    rec->minute = t->tm_min;

    rec->first_place = finalists[0].id;
    strcpy(rec->first_name, finalists[0].name);
    rec->first_score = finalists[0].score;

    rec->second_place = finalists[1].id;
    strcpy(rec->second_name, finalists[1].name);
    rec->second_score = finalists[1].score;

    rec->third_place = finalists[2].id;
    strcpy(rec->third_name, finalists[2].name);
    rec->third_score = finalists[2].score;

    system->record_count++;

    save_records(system);
    save_to_csv(system);

    log_history(system, "添加比赛记录", "新比赛结果已保存");
}

// 保存为CSV
void save_to_csv(SpeechContestSystem *system) {
    char path[300];
    snprintf(path, sizeof(path), "%s/speech_contest_%d.csv",
             system->record_dir, system->total_contests);

    FILE *file = fopen(path, "w");
    if (!file) {
        perror("无法保存CSV");
        return;
    }

    // CSV标题
    fprintf(file, "年份,月份,日期,时间,冠军ID,冠军姓名,冠军得分,亚军ID,亚军姓名,亚军得分,季军ID,季军姓名,季军得分\n");

    for (int i = 0; i < system->record_count; i++) {
        Record *rec = &system->records[i];
        fprintf(file, "%d,%d,%d,%02d:%02d,%d,%s,%.2f,%d,%s,%.2f,%d,%s,%.2f\n",
                rec->year, rec->month, rec->day, rec->hour, rec->minute,
                rec->first_place, rec->first_name, rec->first_score,
                rec->second_place, rec->second_name, rec->second_score,
                rec->third_place, rec->third_name, rec->third_score);
    }

    fclose(file);
}

// 查看比赛记录
void view_records(SpeechContestSystem *system) {
    clear_screen();
    print_header("往届比赛记录");
    sort_records(system->records, system->record_count);

    if (system->record_count == 0) {
        printf("\n暂无比赛记录\n");
    } else {
        display_record_table(system->records, system->record_count);
    }

    print_footer("");
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

        // 删除所有CSV文件
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "rm -f %s/*.csv", system->record_dir);
        // system(cmd);

        printf("\n所有比赛记录已清空！\n");
        log_history(system, "清空比赛记录", "所有记录已删除");
    } else {
        printf("\n操作已取消\n");
    }

    press_any_key();
}

// 归档记录
void archive_records(SpeechContestSystem *system) {
    char archive_path[300];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(archive_path, sizeof(archive_path), "%s/archive_%04d%02d%02d_%02d%02d.dat",
             system->record_dir, t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);

    FILE *file = fopen(archive_path, "wb");
    if (!file) {
        perror("无法创建归档文件");
        return;
    }

    // 保存当前所有记录
    fwrite(&system->record_count, sizeof(int), 1, file);
    for (int i = 0; i < system->record_count; i++) {
        fwrite(&system->records[i], sizeof(Record), 1, file);
    }
    fclose(file);

    // 重置记录
    system->record_count = 0;
    log_history(system, "记录归档", "记录已归档到备份文件");
}

// 查看统计信息
void view_statistics(SpeechContestSystem *system) {
    clear_screen();
    print_header("比赛统计信息");

    if (system->record_count == 0) {
        printf("暂无统计信息\n");
        press_any_key();
        return;
    }

    printf("总共举办比赛: %d 次\n", system->total_contests);
    printf("总记录条目: %d 条\n\n", system->record_count);

    // 计算每位选手的获奖次数
    int win_count[MAX_PLAYERS] = {0};

    for (int i = 0; i < system->record_count; i++) {
        Record rec = system->records[i];

        for (int j = 0; j < MAX_PLAYERS; j++) {
            if (system->players[j].id == rec.first_place ||
                system->players[j].id == rec.second_place ||
                system->players[j].id == rec.third_place) {
                win_count[j]++;
            }
        }
    }

    // 找出获奖最多的选手
    int max_wins = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (win_count[i] > max_wins) {
            max_wins = win_count[i];
        }
    }

    // 显示获奖最多的选手
    printf("获奖最多选手 (获奖 %d 次):\n", max_wins);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (win_count[i] == max_wins) {
            printf("  %s (%d)\n", system->players[i].name, system->players[i].id);
        }
    }

    // 计算获奖院系分布
    int dept_wins[sizeof(DEPARTMENTS)/sizeof(char*)];
    memset(dept_wins, 0, sizeof(dept_wins));

    for (int i = 0; i < system->record_count; i++) {
        Record rec = system->records[i];

        for (int j = 0; j < MAX_PLAYERS; j++) {
            if (system->players[j].id == rec.first_place ||
                system->players[j].id == rec.second_place ||
                system->players[j].id == rec.third_place) {

                for (size_t k = 0; k < sizeof(DEPARTMENTS)/sizeof(char*); k++) {
                    if (strcmp(system->players[j].department, DEPARTMENTS[k]) == 0) {
                        dept_wins[k]++;
                    }
                }
            }
        }
    }

    // 找出获奖最多的院系
    int max_dept_wins = 0;
    for (size_t i = 0; i < sizeof(DEPARTMENTS)/sizeof(char*); i++) {
        if (dept_wins[i] > max_dept_wins) {
            max_dept_wins = dept_wins[i];
        }
    }

    // 显示获奖最多的院系
    printf("\n表现最佳院系 (获奖 %d 次):\n", max_dept_wins);
    for (size_t i = 0; i < sizeof(DEPARTMENTS)/sizeof(char*); i++) {
        if (dept_wins[i] == max_dept_wins) {
            printf("  %s\n", DEPARTMENTS[i]);
        }
    }

    press_any_key();
}

// 开始比赛
void start_contest(SpeechContestSystem *system) {
    clear_screen();
    print_header("开始新比赛");

    if (system->animations_enabled) {
        animate_text("===== 演讲比赛开始 =====", DELAY_BASE);
    } else {
        printf("===== 演讲比赛开始 =====\n\n");
    }

    system->total_contests++;
    save_config(system);

    // 重置选手状态
    for (int i = 0; i < MAX_PLAYERS; i++) {
        system->players[i].score = 0.0;
        system->players[i].eliminated = false;
        system->players[i].ranking = 0;
    }

    // 显示所有选手
    display_player_table(system->players, MAX_PLAYERS, "所有参赛选手", true);
    press_any_key();

    if (system->animations_enabled) {
        animate_text("===== 第一轮比赛：淘汰赛 =====", DELAY_BASE);
    } else {
        printf("\n===== 第一轮比赛：淘汰赛 =====\n");
    }

    // 第一轮比赛
    Player *winners[FINALISTS * 2]; // 两个小组的前三名
    first_round(system, winners);

    // 准备决赛选手
    Player finalists[FINALISTS * 2];
    for (int i = 0; i < FINALISTS * 2; i++) {
        finalists[i] = *winners[i];
    }

    if (system->animations_enabled) {
        animate_text("===== 第二轮比赛：决赛 =====", DELAY_BASE);
    } else {
        printf("\n===== 第二轮比赛：决赛 =====\n");
    }

    // 第二轮比赛
    second_round(system, finalists);

    // 添加比赛记录
    add_record(system, finalists);
}

// 第一轮比赛
void first_round(SpeechContestSystem *system, Player **winners) {
    // 随机洗牌
    shuffle_players(system->players, MAX_PLAYERS);

    // 分成两个小组
    Player groupA[GROUP_SIZE];
    Player groupB[GROUP_SIZE];

    memcpy(groupA, system->players, sizeof(Player) * GROUP_SIZE);
    memcpy(groupB, system->players + GROUP_SIZE, sizeof(Player) * GROUP_SIZE);

    // 小组A比赛
    if (system->animations_enabled) {
        animate_text("===== 第一组比赛 =====", DELAY_BASE);
    } else {
        printf("\n===== 第一组比赛 =====\n");
    }

    display_player_table(groupA, GROUP_SIZE, "第一组选手", false);
    press_any_key();

    group_competition(system, groupA, GROUP_SIZE);
    sort_players(groupA, GROUP_SIZE);

    printf("\n===== 第一组比赛结果 =====\n");
    display_player_table(groupA, GROUP_SIZE, "第一组比赛结果", true);

    // 标记淘汰选手
    for (int i = FINALISTS; i < GROUP_SIZE; i++) {
        groupA[i].eliminated = true;
        groupA[i].ranking = i+1;
    }

    // 记录晋级选手
    winners[0] = &groupA[0];
    winners[1] = &groupA[1];
    winners[2] = &groupA[2];

    if (system->sound_enabled) {
        play_sound("advance");
    }

    printf("\n晋级选手:\n");
    for (int i = 0; i < FINALISTS; i++) {
        printf("  %d. %s (%.2f分)\n", i+1, groupA[i].name, groupA[i].score);
    }

    press_any_key();

    // 小组B比赛
    if (system->animations_enabled) {
        animate_text("===== 第二组比赛 =====", DELAY_BASE);
    } else {
        printf("\n===== 第二组比赛 =====\n");
    }

    display_player_table(groupB, GROUP_SIZE, "第二组选手", false);
    press_any_key();

    group_competition(system, groupB, GROUP_SIZE);
    sort_players(groupB, GROUP_SIZE);

    printf("\n===== 第二组比赛结果 =====\n");
    display_player_table(groupB, GROUP_SIZE, "第二组比赛结果", true);

    // 标记淘汰选手
    for (int i = FINALISTS; i < GROUP_SIZE; i++) {
        groupB[i].eliminated = true;
        groupB[i].ranking = i+1;
    }

    // 记录晋级选手
    winners[3] = &groupB[0];
    winners[4] = &groupB[1];
    winners[5] = &groupB[2];

    if (system->sound_enabled) {
        play_sound("advance");
    }

    printf("\n晋级选手:\n");
    for (int i = 0; i < FINALISTS; i++) {
        printf("  %d. %s (%.2f分)\n", i+1, groupB[i].name, groupB[i].score);
    }

    press_any_key();

    // 显示所有晋级选手
    clear_screen();
    print_header("第一轮晋级选手");

    Player round_winners[FINALISTS * 2];
    for (int i = 0; i < FINALISTS * 2; i++) {
        round_winners[i] = *winners[i];
    }

    display_player_table(round_winners, FINALISTS * 2, "晋级决赛选手", true);
    press_any_key();
}

// 第二轮比赛
void second_round(SpeechContestSystem *system, Player *finalists) {
    // 随机洗牌
    shuffle_players(finalists, FINALISTS * 2);

    // 显示决赛选手
    display_player_table(finalists, FINALISTS * 2, "决赛选手", true);
    press_any_key();

    // 进行决赛
    group_competition(system, finalists, FINALISTS * 2);
    sort_players(finalists, FINALISTS * 2);

    // 设置排名
    for (int i = 0; i < FINALISTS * 2; i++) {
        finalists[i].ranking = i+1;
        finalists[i].eliminated = (i >= FINALISTS);
    }

    // 显示决赛结果
    printf("\n===== 决赛结果 =====\n");
    display_finalists_table(finalists);

    // 宣布前三名
    printf("\n");
    announce_winner(&finalists[0], 1);
    announce_winner(&finalists[1], 2);
    announce_winner(&finalists[2], 3);

    // 庆祝动画
    if (system->animations_enabled) {
        printf("\n");
        for (int i = 0; i < 3; i++) {
            printf("🎉🎉🎉🎉🎉🎉🎉🎉🎉\n");
            fflush(stdout);
            delay_ms(200);
            clear_screen();
            printf("\n");
            if (i == 0) {
                printf("      🏆 🏆 🏆\n");
            } else if (i == 1) {
                printf("   🏆      🏆      🏆\n");
            } else {
                printf("🏆    🏆    🏆    🏆\n");
            }
            delay_ms(200);
        }
        clear_screen();
        print_header("比赛结束");
    }

    printf("\n");
    announce_winner(&finalists[0], 1);
    announce_winner(&finalists[1], 2);
    announce_winner(&finalists[2], 3);
    printf("\n");

    // 记录比赛历史
    char details[100];
    snprintf(details, sizeof(details), "冠军: %s (%.2f分), 亚军: %s (%.2f分), 季军: %s (%.2f分)",
            finalists[0].name, finalists[0].score,
            finalists[1].name, finalists[1].score,
            finalists[2].name, finalists[2].score);
    log_history(system, "比赛完成", details);

    press_any_key();
}

// 小组比赛
void group_competition(SpeechContestSystem *system, Player *group, int size) {
    if (system->debug_mode) {
        printf("\n[调试] 小组比赛开始 (%d名选手)\n", size);
    }

    for (int i = 0; i < size; i++) {
        if (system->animations_enabled) {
            printf("\n请 %s (%d号) 上台演讲...\n", group[i].name, group[i].id);
            for (int j = 0; j < 5; j++) {
                printf(".");
                fflush(stdout);
                delay_ms(500);
            }
            printf("\n");
        }

        calculate_scores(system, &group[i]);

        if (system->debug_mode) {
            printf("[调试] %s 得分: %.2f\n", group[i].name, group[i].score);
        }
    }
}

// 计算选手得分
void calculate_scores(SpeechContestSystem *system, Player *player) {
    // 模拟评委打分
    double scores[JUDGES_COUNT];

    for (int i = 0; i < JUDGES_COUNT; i++) {
        // 基础分 + 随机波动 + 随机影响
        double base = SCORE_RANGE_MIN + (rand() % (int)((SCORE_RANGE_MAX - SCORE_RANGE_MIN) * 10)) / 10.0;
        double variation = (rand() % 100) / 100.0 - 0.5; // -0.5到+0.5的随机波动
        scores[i] = base + variation;

        // 确保分数在范围内
        if (scores[i] < SCORE_RANGE_MIN) scores[i] = SCORE_RANGE_MIN;
        if (scores[i] > SCORE_RANGE_MAX) scores[i] = SCORE_RANGE_MAX;
    }

    // 排序分数
    for (int i = 0; i < JUDGES_COUNT - 1; i++) {
        for (int j = 0; j < JUDGES_COUNT - i - 1; j++) {
            if (scores[j] > scores[j + 1]) {
                double temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    // 去掉最高分和最低分，计算平均分
    double sum = 0;
    for (int i = 1; i < JUDGES_COUNT - 1; i++) {
        sum += scores[i];
    }

    player->score = sum / (JUDGES_COUNT - 2);
}

// 宣布获胜者
void announce_winner(Player *player, int rank) {
    char *medal = "";
    if (rank == 1) medal = "🏆";
    else if (rank == 2) medal = "🥈";
    else if (rank == 3) medal = "🥉";

    printf("%s第%d名: %s (%d号) 得分: %.2f %s\n",
           medal, rank, player->name, player->id, player->score, medal);
}

// 查看历史记录
void view_history(SpeechContestSystem *system) {
    clear_screen();
    print_header("历史操作记录");

    if (system->history_count == 0) {
        printf("\n暂无历史记录\n");
    } else {
        printf("%-20s %-15s %-s\n", "时间", "操作", "详情");
        print_line('-', 80);

        for (int i = 0; i < system->history_count; i++) {
            char time_str[20];
            struct tm *t = localtime(&system->history[i].timestamp);
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

            printf("%-20s %-15s %-s\n",
                   time_str,
                   system->history[i].operation,
                   system->history[i].details);
        }
    }

    press_any_key();
}

// 设置菜单
void settings_menu(SpeechContestSystem *system) {
    int choice;

    do {
        clear_screen();
        print_header("系统设置");
        display_settings(system);

        printf("\n请选择要更改的设置 (0返回): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                toggle_setting((int*)&system->animations_enabled, "动画效果");
                break;
            case 2:
                toggle_setting((int*)&system->sound_enabled, "音效");
                break;
            case 3:
                toggle_setting((int*)&system->debug_mode, "调试模式");
                break;
            case 4:
                printf("\n输入新的记录目录 (当前: %s): ", system->record_dir);
                scanf("%255s", system->record_dir);
                log_history(system, "修改设置", "更改记录目录");
                printf("\n记录目录已更新!\n");
                press_any_key();
                break;
            case 0:
                return;
            default:
                printf("无效选择!\n");
                press_any_key();
        }

        save_config(system);
    } while (choice != 0);
}

// 显示设置
void display_settings(SpeechContestSystem *system) {
    printf("1. 动画效果: %s\n", system->animations_enabled ? "启用" : "禁用");
    printf("2. 音效: %s\n", system->sound_enabled ? "启用" : "禁用");
    printf("3. 调试模式: %s\n", system->debug_mode ? "启用" : "禁用");
    printf("4. 记录目录: %s\n", system->record_dir);
}

// 切换设置
void toggle_setting(int *setting, const char *setting_name) {
    *setting = !(*setting);
    printf("\n%s已%s!\n", setting_name, *setting ? "启用" : "禁用");
    press_any_key();
}

// 导出数据到CSV
void export_data_csv(SpeechContestSystem *system) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/export_%ld.csv",
             system->record_dir, time(NULL));

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("无法创建导出文件\n");
        return;
    }

    // 标题行
    fprintf(file, "ID,姓名,年龄,院系,得分,是否淘汰,排名\n");

    // 选手数据
    for (int i = 0; i < MAX_PLAYERS; i++) {
        fprintf(file, "%d,%s,%d,%s,%.2f,%s,%d\n",
               system->players[i].id,
               system->players[i].name,
               system->players[i].age,
               system->players[i].department,
               system->players[i].score,
               system->players[i].eliminated ? "是" : "否",
               system->players[i].ranking);
    }

    fclose(file);
    printf("\n数据已导出到: %s\n", filename);

    log_history(system, "数据导出", "导出选手数据到CSV");
    press_any_key();
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

// 记录排序 (按日期时间降序)
void sort_records(Record *records, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // 比较日期时间
            if (records[j].year < records[j+1].year) continue;
            if (records[j].year == records[j+1].year) {
                if (records[j].month < records[j+1].month) continue;
                if (records[j].month == records[j+1].month) {
                    if (records[j].day < records[j+1].day) continue;
                    if (records[j].day == records[j+1].day) {
                        if (records[j].hour < records[j+1].hour) continue;
                        if (records[j].hour == records[j+1].hour) {
                            if (records[j].minute < records[j+1].minute) continue;
                        }
                    }
                }
            }

            Record temp = records[j];
            records[j] = records[j + 1];
            records[j + 1] = temp;
        }
    }
}

// 显示选手表格
void display_player_table(Player *players, int n, const char *title, bool show_details) {
    printf("\n%s (%d人):\n", title, n);
    print_line('-', 100);

    if (show_details) {
        printf("%-8s %-10s %-4s %-12s %-8s %-6s %-4s\n",
               "编号", "姓名", "年龄", "院系", "得分", "状态", "排名");
        print_line('-', 100);

        for (int i = 0; i < n; i++) {
            const char *status = players[i].eliminated ? "淘汰" : "晋级";
            printf("%-8d %-10s %-4d %-12s %-8.2f %-6s %-4d\n",
                   players[i].id, players[i].name, players[i].age,
                   players[i].department, players[i].score, status, players[i].ranking);
        }
    } else {
        printf("%-8s %-10s %-12s\n", "编号", "姓名", "院系");
        print_line('-', 50);

        for (int i = 0; i < n; i++) {
            printf("%-8d %-10s %-12s\n",
                   players[i].id, players[i].name, players[i].department);
        }
    }

    print_line('-', 100);
}

// 显示记录表格
void display_record_table(Record *records, int n) {
    printf("%-12s %-8s %-10s %-10s %-8s %-8s\n",
           "日期", "时间", "冠军", "亚军", "季军", "详情");
    print_line('-', 100);

    for (int i = 0; i < n; i++) {
        char date_str[12];
        snprintf(date_str, sizeof(date_str), "%d-%02d-%02d",
                records[i].year, records[i].month, records[i].day);

        char time_str[6];
        snprintf(time_str, sizeof(time_str), "%02d:%02d",
                records[i].hour, records[i].minute);

        char details[20];
        snprintf(details, sizeof(details), "查看详情 %d", i+1);

        printf("%-12s %-8s %-10s %-10s %-10s %-8s\n",
               date_str, time_str,
               records[i].first_name,
               records[i].second_name,
               records[i].third_name,
               details);
    }

    print_line('-', 100);
}

// 显示决赛选手表格
void display_finalists_table(Player *finalists) {
    printf("\n决赛结果:\n");
    print_line('-', 70);
    printf("%-4s %-10s %-12s %-8s %-10s\n", "排名", "姓名", "院系", "得分", "状态");
    print_line('-', 70);

    for (int i = 0; i < FINALISTS * 2; i++) {
        const char *status = finalists[i].eliminated ? "淘汰" : "胜出";
        const char *medal = "";

        if (i == 0) medal = "🏆";
        else if (i == 1) medal = "🥈";
        else if (i == 2) medal = "🥉";

        printf("%-4s%-2d %-10s %-12s %-8.2f %-10s\n",
               medal, finalists[i].ranking, finalists[i].name,
               finalists[i].department, finalists[i].score, status);
    }

    print_line('-', 70);
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

// 工具函数：打印标题
void print_header(const char *title) {
    printf("\n");
    print_line('=', strlen(title) + 8);
    printf("=== %s ===\n", title);
    print_line('=', strlen(title) + 8);
}

// 工具函数：打印页脚
void print_footer(const char *message) {
    printf("\n");
    print_line('-', strlen(message) + 4);
    printf("- %s -\n", message);
    print_line('-', strlen(message) + 4);
}

// 工具函数：打印分隔线
void print_line(char c, int length) {
    for (int i = 0; i < length; i++) {
        putchar(c);
    }
    putchar('\n');
}

// 工具函数：文字动画
void animate_text(const char *text, int delay) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        putchar(text[i]);
        fflush(stdout);
        usleep(delay);
    }
    printf("\n");
}

// 工具函数：播放音效
void play_sound(const char *sound_type) {
    // 实际实现将使用系统特定的声音API
    // 这里只是模拟
    if (strcmp(sound_type, "advance") == 0) {
        printf("\a");  // 系统铃声
    } else if (strcmp(sound_type, "winner") == 0) {
        printf("\a\a"); // 两次铃声
    }
}

// 工具函数：延迟
void delay_ms(long milliseconds) {
    usleep(milliseconds * 1000);
}

// 记录菜单
void record_menu(SpeechContestSystem *system) {
    int choice;

    do {
        clear_screen();
        print_header("比赛记录");

        printf("1. 查看往届记录\n");
        printf("2. 清空比赛记录\n");
        printf("3. 查看统计数据\n");
        printf("4. 导出数据到CSV\n");
        printf("5. 归档比赛记录\n");
        printf("0. 返回主菜单\n");

        printf("\n请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_records(system);
                break;
            case 2:
                clear_records(system);
                break;
            case 3:
                view_statistics(system);
                break;
            case 4:
                export_data_csv(system);
                break;
            case 5:
                archive_records(system);
                printf("\n比赛记录已归档！\n");
                press_any_key();
                break;
            case 0:
                return;
            default:
                printf("无效选择！\n");
                press_any_key();
        }
    } while (choice != 0);
}

// 主菜单
void main_menu(SpeechContestSystem *system) {
    int choice;

    do {
        clear_screen();
        print_header("校园演讲比赛系统");
        display_main_menu();

        printf("\n请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                start_contest(system);
                break;
            case 2:
                record_menu(system);
                break;
            case 3:
                view_history(system);
                break;
            case 4:
                settings_menu(system);
                break;
            case 5:
                validate_system(system);
                press_any_key();
                break;
            case 6:
                backup_system(system);
                press_any_key();
                break;
            case 0:
                printf("\n感谢使用，再见！\n");
                log_history(system, "系统关闭", "程序正常退出");
                exit(0);
            default:
                printf("\n无效选择，请重新输入！\n");
                press_any_key();
        }
    } while (choice != 0);
}

// 显示主菜单
void display_main_menu() {
    printf("\n主菜单:\n");
    print_line('-', 50);
    printf("1. 开始演讲比赛\n");
    printf("2. 比赛记录管理\n");
    printf("3. 查看操作历史\n");
    printf("4. 系统设置\n");
    printf("5. 验证系统状态\n");
    printf("6. 备份系统数据\n");
    printf("0. 退出比赛程序\n");
    print_line('-', 50);
}

// 验证系统
void validate_system(SpeechContestSystem *system) {
    clear_screen();
    print_header("系统验证");

    int errors = 0;

    // 验证选手数据
    printf("验证选手数据...\n");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!validate_player_id(system->players[i].id)) {
            printf("  错误: 无效选手ID: %d\n", system->players[i].id);
            errors++;
        }

        if (!validate_player_name(system->players[i].name)) {
            printf("  错误: 无效选手姓名: %s\n", system->players[i].name);
            errors++;
        }

        if (!validate_player_age(system->players[i].age)) {
            printf("  错误: 无效选手年龄: %d\n", system->players[i].age);
            errors++;
        }

        if (!validate_player_department(system->players[i].department)) {
            printf("  错误: 无效院系: %s\n", system->players[i].department);
            errors++;
        }
    }

    // 验证记录数据
    printf("\n验证比赛记录...\n");
    for (int i = 0; i < system->record_count; i++) {
        if (system->records[i].year < 2000 || system->records[i].year > 2100) {
            printf("  错误: 无效年份: %d\n", system->records[i].year);
            errors++;
        }
    }

    // 验证目录存在
    printf("\n验证目录权限...\n");
    struct stat st;
    if (stat(system->record_dir, &st) == -1) {
        printf("  错误: 记录目录不存在: %s\n", system->record_dir);
        errors++;
    } else if (!(st.st_mode & S_IWUSR)) {
        printf("  错误: 记录目录不可写: %s\n", system->record_dir);
        errors++;
    }

    printf("\n验证完成! 共发现 %d 个错误。\n", errors);
}

// 备份系统数据
void backup_system(SpeechContestSystem *system) {
    char backup_dir[256];
    snprintf(backup_dir, sizeof(backup_dir), "%s/backup_%ld",
             system->record_dir, time(NULL));

    // 创建备份目录
    mkdir(backup_dir, 0700);

    // 复制记录文件
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "cp %s/* %s/", system->record_dir, backup_dir);
    // system(cmd);

    // 创建系统状态快照
    char snapshot_path[256];
    snprintf(snapshot_path, sizeof(snapshot_path), "%s/system_snapshot.dat", backup_dir);

    FILE *snapshot = fopen(snapshot_path, "wb");
    if (snapshot) {
        fwrite(system, sizeof(SpeechContestSystem), 1, snapshot);
        fclose(snapshot);
    }

    printf("\n系统备份已创建在: %s\n", backup_dir);
    log_history(system, "系统备份", "创建完整系统备份");
}

// 验证选手ID
int validate_player_id(int id) {
    return id >= PLAYER_ID_START && id < PLAYER_ID_START + MAX_PLAYERS;
}

// 验证选手姓名
int validate_player_name(const char *name) {
    return strlen(name) > 1 && strlen(name) < MAX_NAME_LEN;
}

// 验证选手年龄
int validate_player_age(int age) {
    return age >= 16 && age <= 25;
}

// 验证院系
int validate_player_department(const char *department) {
    for (size_t i = 0; i < sizeof(DEPARTMENTS)/sizeof(char*); i++) {
        if (strcmp(department, DEPARTMENTS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// ====================== 主函数 ======================
int main() {
    SpeechContestSystem system;
    init_system(&system);
    main_menu(&system);
    return 0;
}