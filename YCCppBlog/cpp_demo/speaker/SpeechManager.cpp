//
// Created by 杨充 on 2025/6/28.
//

#include "SpeechManager.h"

//执行指令：g++ SpeechManager.cpp

SpeechManager::SpeechManager(){
    //初始化属性
    this->initSpeech();
    //创建选手
    this->createSpeaker();
    //获取往届记录
    this->loadRecord();
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
    SpeechManager sm;   //创建对象之后，首先对调用构造方法
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
                sm.showRecord();
                break;
            case 3:  //清空记录
                sm.clearRecord();
                break;
            case 0:  //退出系统
                sm.exitSystem();
                break;
            default:
                clearScreen();
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
    //保证选手容器也为空
    this->speaker.clear();
    //初始化比赛轮次
    this->index = 1;
}

void SpeechManager::createSpeaker() {
    //创建选手后，将选手编号，并且放到容器中。按照A，B，C……L排序
    string nameSeed = "ABCDEFGHIJKL";
    for (int i=0 ; i<nameSeed.size() ; i++) {
        string name = "选手";
        name = name + nameSeed[i];
        //创建选手对象
        Speaker speaker;
        speaker.name = name;
        for (int j=0 ; j<2 ; j++) {
            speaker.score[j] = 0;
        }
        int key = i + 10001;
        //将选手放到v1容器中。从10001开始
        this->v1.push_back(key);
        //将选手放到容器中
        this->speaker.insert(std::make_pair(key, speaker)); // 使用 insert
    }
}

void SpeechManager::startSpeech() {
    //第一轮比赛
    //1、抽签
    this->speechDraw();
    //2、比赛
    // this->speechContest();
    // //3、显示晋级结果
    // this->showScore();
    // //第二轮比赛
    // this->index++;
    // //1、抽签
    // this->speechDraw();
    // //2、比赛
    // this->speechContest();
    // //3、显示最终结果
    // this->showScore();
    // //4、保存分数
    // this->saveRecord();
}

void SpeechManager::speechDraw() {
    cout << "第 << " << this->index << " >> 轮比赛选手正在抽签，打乱用户顺序"<<endl;
    cout << "---------------------" << endl;
    cout << "抽签后演讲顺序如下：" << endl;
    //<algorithm>：提供 std::shuffle。<random>：提供随机数生成器。
    //使用 std::shuffle 打乱 std::vector 的顺序。
    // 创建随机数生成器
    std::random_device rd;  // 随机种子
    std::mt19937 g(rd());   // 使用 Mersenne Twister 算法
    if (this->index == 1) {
        //第一轮
        shuffle(v1.begin(), v1.end(),g);
        //然后打印一下打乱后的次序
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

void SpeechManager::showScore() {
    cout << "---------第" << this->index << "轮晋级选手信息如下：-----------" << endl;
    vector<int> v;
    if (this->index == 1) {
        //如果是第一轮，晋级6人
        v = v2;
    } else {
        //如果是第二轮，晋级3人
        v = victory;
    }
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        cout << "选手编号：" << *it <<
            " 姓名： " << speaker[*it].name <<
                " 得分： " << speaker[*it].score[this->index - 1] << endl;
    }
    clearScreen();
    this->showMenu();
}

void SpeechManager::saveRecord() {
    ofstream ofs;
    //用输出的方式打开文件  -- 写文件
    ofs.open("speech.txt",ios::out | ios::app);
    //将每个人数据写入到文件中
    for (vector<int>::iterator it = victory.begin(); it != victory.end(); it++) {
        //遍历数据然后写到流中
        ofs << *it << "号选手，分数是：" << speaker[*it].score[1] << endl;
    }
    ofs << endl;
    //关闭文件
    ofs.close();
    cout << "记录已经保存" << endl;
}


void SpeechManager::loadRecord() {
    //输入流对象 读取文件
    ifstream ifs("speech.txt", ios::in);
    if (!ifs.is_open()) {
        this->fileIsEmpty = true;
        cout << "文件不存在！" << endl;
        ifs.close();
        return;
    }
    char ch;
    ifs >> ch;
    if (ifs.eof()) {
        cout << "文件为空!" << endl;
        this->fileIsEmpty = true;
        ifs.close();
        return;
    }
    //文件不为空
    this->fileIsEmpty = false;
    ifs.putback(ch); //读取的单个字符放回去
    string data;
    int index = 0;
    while (ifs >> data) {
        //cout << data << endl;
        vector<string> v;
        int pos = -1;
        int start = 0;
        while (true) {
            pos = data.find(",", start); //从0开始查找 ','
            if (pos == -1) {
                break; //找不到break返回
            }
            string tmp = data.substr(start, pos - start); //找到了,进行分割 参数1 起始位置，参数2 截取长度
            v.push_back(tmp);
            start = pos + 1;
        }
        this->record.insert(make_pair(index, v));
        index++;
    }
    ifs.close();
}

void SpeechManager::showRecord() {
    for (int i = 0; i < this->record.size(); i++) {
        cout << "第" << i + 1 << "届 " <<
                "冠军编号：" << this->record[i][0] << " 得分：" << this->record[i][1] << " "
                "亚军编号：" << this->record[i][2] << " 得分：" << this->record[i][3] << " "
                "季军编号：" << this->record[i][4] << " 得分：" << this->record[i][5] << endl;
    }
    pauseAndCls();
}

void SpeechManager::clearRecord() {
    cout << "确认清空？" << endl;
    cout << "1、确认" << endl;
    cout << "2、返回" << endl;
    int select = 0;
    cin >> select;
    if (select == 1) {
        //打开模式 ios::trunc 如果存在删除文件并重新创建
        ofstream ofs("speech.txt", ios::trunc);
        ofs.close();
        //初始化属性
        this->initSpeech();
        //创建选手
        this->createSpeaker();
        //获取往届记录
        this->loadRecord();
        cout << "清空成功！" << endl;
    }
    pauseAndCls();
}







