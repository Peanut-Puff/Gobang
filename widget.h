#ifndef WIDGET_H
#define WIDGET_H

#include<QWidget>
#include<QMouseEvent>
#include<QPainter>
#include<QPushButton>
#include<QMessageBox>
#include<chessai.h>
#include<QStack>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
//用于存放坐标的结构体，用作记录落点的栈的成员类型
struct xy{
    int x;
    int y;
};
//实现窗口的类，继承自QWidget
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);//构造函数
    ~Widget();//析构函数
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;//鼠标单击释放时执行
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动时执行
    void paintEvent(QPaintEvent *event) override;//绘制棋盘、棋子、光标
    void init();//初始化
    int check();//检查游戏是否结束
private:
    Ui::Widget *ui;
    QPoint chessboard[15][15];//棋盘各点的坐标
    chessAI ai;//下棋的ai
    QStack<xy> black;//黑子落点记录
    QStack<xy> white;//白子落点记录
    int col;//列（经过计算换算的对应棋盘数组列下标）
    int row;//行（经过计算换算的对应棋盘数组行下标）
    int turn;//回合
private slots:
    void restartButtonPush();//按下重新开始按钮
    void backButtonPush();//按下悔棋按钮
};
#endif // WIDGET_H
