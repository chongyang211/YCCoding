//
// Created by 杨充 on 2025/8/3.
//

#include "FileSpeechUtils.h"

using namespace std;

void saveRecord(vector<int> victory) {
    std::ofstream ofs;
    //用输出的方式打开文件  -- 写文件
    ofs.open("speech.txt",ios::out | ios::app);
    //将每个人数据写入到文件中
    for (vector<int>::iterator it = victory.begin(); it != victory.end(); it++) {
        //遍历数据然后写到流中
        // ofs << *it << "号选手，分数是：" << speaker[*it].score[1] << endl;
    }
    ofs << endl;
    //关闭文件
    ofs.close();
    cout << "记录已经保存" << endl;
}
