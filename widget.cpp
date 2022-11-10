#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(this->ui->btn_restart,SIGNAL(clicked(bool)),this,SLOT(restartButtonPush()));
    connect(this->ui->btn_back,SIGNAL(clicked(bool)),this,SLOT(backButtonPush()));
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++){
            chessboard[i][j].setX(20+30*j);
            chessboard[i][j].setY(20+30*i);
        }
    setMouseTracking(true);
    init();
}

void Widget::init(){
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++){
            ai.pieces[i][j]=P_NONE;
            ai.scores[i][j]=0;
        }
    row=-1;
    col=-1;
    turn=T_BLACK;
    while(!black.empty())
        black.pop();
    while(!white.empty())
        white.pop();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    //chessboard
    for(int i=0;i<15;i++){
        painter.drawLine(chessboard[i][0],chessboard[i][14]);
        painter.drawLine(chessboard[0][i],chessboard[14][i]);
    }
    //curser position
    if(turn==T_BLACK)
        painter.setBrush(Qt::black);
    else
        painter.setBrush(Qt::white);
    if(row!=-1&&col!=-1)
        painter.drawRect(chessboard[row][col].x()-4,chessboard[row][col].y()-4,8,8);
    //pieces
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
            if(ai.pieces[i][j]!=P_NONE){
                if(ai.pieces[i][j]==P_BLACK)
                    painter.setBrush(Qt::black);
                else
                    painter.setBrush((Qt::white));
                painter.drawEllipse(chessboard[i][j],10,10);
            }
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    if(event->x()>=5&&event->x()<455&&event->y()>=5&&event->y()<455){
        setCursor(Qt::BlankCursor);
        col=(event->x()-5)/30;
        row=(event->y()-5)/30;
        if(ai.pieces[row][col]!=P_NONE)
            setCursor(Qt::ForbiddenCursor);
    }
    else
        setCursor(Qt::ArrowCursor);
    update();
}

int Widget::check(){
    bool flag;
    int piece;
    bool full=true;
    int res;
    int cnt;
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            flag=false;
            piece=ai.pieces[i][j];
            if(piece==P_NONE){
                full=false;
                continue;
            }
            int k;
            for(cnt=0,k=-4;k<5;k++)
                if(i+k<0)
                    continue;
                else if(i+k>=15||(i+k<15&&ai.pieces[i+k][j]!=piece))
                    break;
                else
                    cnt++;
            if(cnt>=5)
                flag=true;
            if(!flag){
                for(cnt=0,k=-4;k<5;k++)
                    if(j+k<0)
                        continue;
                    else if(j+k>=15||(j+k<15&&ai.pieces[i][j+k]!=piece))
                        break;
                    else
                        cnt++;
                if(cnt>=5)
                    flag=true;
                if(!flag){
                    for(cnt=0,k=-4;k<5;k++)
                        if(i+k<0||j+k<0)
                            continue;
                        else if(i+k>=15||j+k>=15||(i+k<15&&j+k<15&&ai.pieces[i+k][j+k]!=piece))
                            break;
                        else
                            cnt++;
                    if(cnt>=5)
                        flag=true;
                    if(!flag){
                        for(cnt=0,k=-4;k<5;k++)
                            if(i+k<0||j-k<0)
                                continue;
                            else if(i+k>=15||j-k>=15||(i+k<15&&j-k<15&&ai.pieces[i+k][j-k]!=piece))
                                break;
                            else
                                cnt++;
                        if(cnt>=5)
                            flag=true;
                    }
                }
            }
            if(flag)
                break;
        }
        if(flag)
            break;
    }
    if(flag)
        if(piece==P_BLACK)
            res=R_BLACK;
        else
            res=R_WHITE;
    else
        if(full)
            res=R_NOWINNER;
        else
            res=R_CONTINUE;
    if(res!=R_CONTINUE){
         QMessageBox msg;
         msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
         msg.button(QMessageBox::Yes)->setText("再来一局");
         msg.button(QMessageBox::No)->setText("结束游戏");
         if(res==R_NOWINNER)
             msg.setText("平局！");
         else if(res==R_BLACK)
             msg.setText("黑棋获胜！");
         else
             msg.setText("白棋获胜！");
         int choose=msg.exec();
         if(choose==QMessageBox::No){
             QApplication *app;
             app->exit();
         }
         else
             init();
         update();
    }
    return res;
}

void Widget::mouseReleaseEvent(QMouseEvent *event){
    xy tmp;
    if(ai.pieces[row][col]==P_NONE){
        //record
        tmp.x=row;
        tmp.y=col;
        black.push(tmp);
        //turn change
        if(turn==T_BLACK){
            turn=T_WHITE;
            ai.pieces[row][col]=P_BLACK;
        }
        else{
            turn=T_BLACK;
            ai.pieces[row][col]=P_WHITE;
        }
        //check result
        if(check()==R_CONTINUE){
            //alpharun
            ai.nextstep();
            ai.pieces[ai.best_point.x][ai.best_point.y]=P_WHITE;
            ai.evaluate();
            turn=turn^1;
            tmp.x=ai.best_point.x;
            tmp.y=ai.best_point.y;
            white.push(tmp);
            //check result
            check();
        }
        update();
    }
}

void Widget::restartButtonPush(){
    init();
    update();
}

void Widget::backButtonPush(){
    if(!black.empty()&&!white.empty()){
        xy tmp=white.pop();
        ai.pieces[tmp.x][tmp.y]=P_NONE;
        turn=turn^1;
        tmp=black.pop();
        ai.pieces[tmp.x][tmp.y]=P_NONE;
        turn=turn^1;
        ai.evaluate();
        update();
    }
}
