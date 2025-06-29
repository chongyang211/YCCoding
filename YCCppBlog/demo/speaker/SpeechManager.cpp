//
// Created by 杨充 on 2025/6/28.
//

#include "SpeechManager.h"

SpeechManager::SpeechManager(){
    //初始化属性
    this->initSpeech();
    //创建选手
    this->createSpeaker();
}

SpeechManager::~SpeechManager(){

}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // 使用 ANSI Escape Codes 清空屏幕
    std::cout << "清空屏幕" << std::endl;
}

void pause() {
    //替代 system("pause")
    //system("pause");
    std::cout << "点击enter键，暂停";
    std::cin.get(); // 等待用户输入字符
}

//暂停和清空屏幕
void pauseAndCls() {
    pause();    //暂停
    clearScreen(); // 清空屏幕
}

int main() {
    SpeechManager sm;
    for (map<int, Speaker>::iterator it = sm.speaker.begin(); it != sm.speaker.end(); it++) {
        cout << "选手编号：" << it->first
                << " 姓名： " << it->second.name
                << " 成绩： " << it->second.score[0] << endl;
    }
    int choice = 0; //用来存储用户的选项
    while (true){
        sm.showMenu();
        cout << "请输入您的选择： " << endl;
        cin >> choice; // 接受用户的选项
        switch (choice){
            case 1:  //开始比赛
                sm.startSpeech();
                break;
            case 2:  //查看记录
                break;
            case 3:  //清空记录
                break;
            case 0:  //退出系统
                sm.exitSystem();
                break;
            default:
                system("cls"); //清屏
            break;
        }
    }
    pause();
    return 0;
}

void SpeechManager::showMenu() {
    cout << "********************************************" << endl;
    cout << "*************  欢迎参加演讲比赛 ************" << endl;
    cout << "*************  1.开始演讲比赛  *************" << endl;
    cout << "*************  2.查看往届记录  *************" << endl;
    cout << "*************  3.清空比赛记录  *************" << endl;
    cout << "*************  0.退出比赛程序  *************" << endl;
    cout << "********************************************" << endl;
    cout << endl;
}

void SpeechManager::exitSystem() {
    cout << "欢迎下次使用" << endl;
    pause();
    exit(0);
}

void SpeechManager::initSpeech() {
    //容器保证为空
    this->v1.clear();
    this->v2.clear();
    this->victory.clear();
    this->speaker.clear();
    //初始化比赛轮次
    this->index = 1;
}

void SpeechManager::createSpeaker() {
    //创建选手后，将选手编号，并且放到容器中
    string nameSeed = "ABCDEFGHIJKL";
    for (int i=0 ; i<nameSeed.size() ; i++) {
        string name = "选手";
        name += nameSeed[i];
        Speaker sp;
        sp.name = name;
        for (int i=0 ; i<2 ; i++) {
            sp.score[i] = 0;
        }
        //12名选手编号
        this->v1.push_back(i+10001);
        //选手编号 以及对应的选手 存放到map容器中
        this->speaker.insert(make_pair(i+10001,sp));
    }
}

void SpeechManager::startSpeech() {
    //第一轮比赛
    //1、抽签
    this->speechDraw();
    //2、比赛
    this->speechContest();
    //3、显示晋级结果
    //第二轮比赛
    //1、抽签
    //2、比赛
    //3、显示最终结果
    //4、保存分数
}

void SpeechManager::speechDraw() {
    cout << "第 << " << this->index << " >> 轮比赛选手正在抽签，打乱用户顺序"<<endl;
    cout << "---------------------" << endl;
    cout << "抽签后演讲顺序如下：" << endl;
    //random_shuffle这个已经不用了
    // 创建随机数引擎
    random_device rd;
    mt19937 g(rd());
    if (this->index == 1) {
        shuffle(v1.begin(), v1.end(),g);
        for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++){
            cout << *it << " ";
        }
        cout << endl;
    } else {
        shuffle(v2.begin(), v2.end(),g);
        for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++){
            cout << *it << " ";
        }
        cout << endl;
    }
    cout << "---------------------" << endl;
    pause();
    cout << endl;
}

void SpeechManager::speechContest() {
    cout << "------------- 第" << this->index << "轮正式比赛开始：------------- " << endl;
    //临时容器，保存key分数 value 选手编号
    multimap<double, int, greater<int> > groupScore;
    //记录人员数，6个为1组
    int num = 0;
    //比赛的人员容器
    vector<int> srcVector;
    if (this->index == 1) {
        srcVector = v1;
    } else {
        srcVector = v2;
    }
    //遍历所有参赛选手
    for (vector<int>::iterator it = srcVector.begin(); it != srcVector.end(); it++) {
        num++;
        //评委打分
        deque<double> d;
        for (int i = 0; i < 10; i++) {
            // 600 ~ 1000
            double score = (rand() % 401 + 600) / 10.f;
            //cout << score << " ";
            d.push_back(score);
        }
        //排序
        sort(d.begin(), d.end(), greater<double>());
        //去掉最高分
        d.pop_front();
        //去掉最低分
        d.pop_back();
        //获取总分
        double sum = accumulate(d.begin(), d.end(), 0.0f);
        //获取平均分
        double avg = sum / (double) d.size();
        //每个人平均分
        //cout << "编号： " << *it  << " 选手： " << this->m_Speaker[*it].m_Name << " 获取平均分为： " << avg << endl;  //打印分数
        this->speaker[*it].score[this->index - 1] = avg;
        //6个人一组，用临时容器保存
        groupScore.insert(make_pair(avg, *it));
        if (num % 6 == 0) {
            cout << "第" << num / 6 << "小组比赛名次：" << endl;
            for (multimap<double, int, greater<int> >::iterator it = groupScore.begin(); it != groupScore.end(); it++) {
                cout << "编号: " << it->second
                        << " 姓名： " << this->speaker[it->second].name
                        << " 成绩： " << this->speaker[it->second].score[this->index - 1] << endl;
            }
            int count = 0;
            //取前三名
            for (multimap<double, int, greater<int> >::iterator it = groupScore.begin();
                 it != groupScore.end() && count < 3; it++, count++) {
                if (this->index == 1) {
                    v2.push_back((*it).second);
                } else {
                    victory.push_back((*it).second);
                }
            }
            groupScore.clear();
            cout << endl;
        }
    }
    cout << "------------- 第" << this->index << "轮比赛完毕  ------------- " << endl;
    pause();
}







