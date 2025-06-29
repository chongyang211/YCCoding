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
    int choice = 0; //用来存储用户的选项
    while (true){
        sm.showMenu();
        cout << "请输入您的选择： " << endl;
        cin >> choice; // 接受用户的选项
        switch (choice){
            case 1:  //开始比赛
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






