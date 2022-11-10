#ifndef CHESSAI_H
#define CHESSAI_H

#include<limits.h>
#include<QPoint>
#include<QVector>
#include<string.h>

//棋子类型（黑白空）
#define P_NONE 2
#define P_BLACK 1
#define P_WHITE 0
//回合（黑白）
#define T_BLACK 0
#define T_WHITE 1
//结果（下满棋盘平局、黑胜、白胜、进行中）
#define R_NOWINNER 2
#define R_BLACK 1
#define R_WHITE 0
#define R_CONTINUE 3
//搜索深度
#define DEPTH_LIMIT 6
//选点个数
#define POS_NUM 15
//棋型
#define L5 20000//连五
#define H4 2400//活四
#define C4 300//冲四
#define H3 400//活三
#define M3 5//眠三
#define H2 8//活二
#define M2 1//眠二
struct point{
    int x;
    int y;
    int score;
    int statu;
};//记录某点坐标、得分、状态的结构体
class chessAI//实现ai下棋的类
{
public:
    chessAI();//构造函数
    int pieces[15][15];//棋子类型（黑白空）
    int scores[15][15];//各点得分
    point best_point;//最终落点
    int nextstep(int turn=T_WHITE,int depth=0,int alpha=INT_MIN,int beta=INT_MAX);//极大极小搜索确定下一步
    int evaluate(point &p);//评估当前局面（选点用），不改变scores数组
    void evaluate(int rowl=0,int rowr=14,int coll=0,int colr=14);//评估当前局面（模拟用），改变scores数组
    void getpos(int turn,QVector<point>& pos);//选点
    int judge_type(int x,int y,int d1,int d2);//判断棋型
};

#endif // CHESSAI_H
