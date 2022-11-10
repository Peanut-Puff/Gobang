#include "chessai.h"

chessAI::chessAI()
{
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++){
            pieces[i][j]=P_NONE;
            scores[i][j]=0;
        }
}

/* judge_type参数说明:
 * d1,d2={-1,0,1),控制方向
 * l(r)chess[0]-某侧相邻连续同色棋子数
 * l(r)empty[0]-离中心位置最近的一侧空位数
 * l(r)chess[1]-某侧的第二段连续同色棋子
 * l(r)empty[1]-第二段连续空位
 * l(r)chess[2]-某侧的第三段连续同色棋子
*/
int chessAI::judge_type(int x,int y,int d1,int d2){
    int piece=pieces[x][y];
    int lchess[3]={0},rchess[3]={0},lempty[2]={0},rempty[2]={0};
    //某侧
    int i=x-d1,j=y-d2;
    for(;(d1==0||i!=x-5*d1)&&(d2==0||j!=y-5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==piece;i-=d1,j-=d2)
        lchess[0]++;
    for(;(d1==0||i!=x-5*d1)&&(d2==0||j!=y-5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==P_NONE;i-=d1,j-=d2)
        lempty[0]++;
    for(;(d1==0||i!=x-5*d1)&&(d2==0||j!=y-5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==piece;i-=d1,j-=d2)
        lchess[1]++;
    for(;(d1==0||i!=x-5*d1)&&(d2==0||j!=y-5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==P_NONE;i-=d1,j-=d2)
        lempty[1]++;
    for(;(d1==0||i!=x-5*d1)&&(d2==0||j!=y-5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==piece;i-=d1,j-=d2)
        lchess[2]++;
    //另一侧
    i=x+d1;
    j=y+d2;
    for(;(d1==0||i!=x+5*d1)&&(d2==0||j!=y+5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==piece;i+=d1,j+=d2)
        rchess[0]++;
    for(;(d1==0||i!=x+5*d1)&&(d2==0||j!=y+5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==P_NONE;i+=d1,j+=d2)
        rempty[0]++;
    for(;(d1==0||i!=x+5*d1)&&(d2==0||j!=y+5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==piece;i+=d1,j+=d2)
        rchess[1]++;
    for(;(d1==0||i!=x+5*d1)&&(d2==0||j!=y+5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==P_NONE;i+=d1,j+=d2)
        rempty[1]++;
    for(;(d1==0||i!=x+5*d1)&&(d2==0||j!=y+5*d2)&&i>=0&&i<15&&j>=0&&j<15&&pieces[i][j]==piece;i+=d1,j+=d2)
        rchess[2]++;
    //判断棋型
    int mid=lchess[0]+rchess[0]+1;
    if(mid>=5)//连五
        return L5;
    else if(mid==4){
        if(lempty[0]&&rempty[0])
            return H4;
        else if(lempty[0]||rempty[0])
            return C4;
        else
            return 0;//死4
    }
    else if(mid==3){
        if((lempty[0]==1&&lchess[1]>=1)||(rempty[0]==1&&rchess[1]>=1))
            return C4;
        else if(lempty[0]+rempty[0]>=3&&lempty[0]&&rempty[0])
            return H3;
        else if(rempty[0]+lempty[0]>=2)
            return M3;
        else
            return 0;
    }
    else if(mid==2){
        if((lempty[0]==1&&lchess[1]>=2)||(rempty[0]==1&&rchess[1]>=2))
            return C4;
        else if((lempty[0]==1&&lchess[1]&&lempty[1]&&rempty[0])||(rempty[0]==1&&rchess[1]&&rempty[1]&&lempty[0]))
            return H3;
        else if((lchess[1]&&((lempty[0]==1&&(lempty[1]||rempty[0]))||lempty[0]==2))||(rchess[1]&&((rempty[0]==1&&(rempty[1]||lempty[0]))||rempty[0]==2)))
            return M3;
        else if(lempty[0]+rempty[0]>=4&&lempty[0]&&rempty[0])
            return H2;
        else if((lempty[0]||rempty[0])&&lempty[0]+rempty[0]>=3)
            return M2;
        else
            return 0;
    }
    else{
        if((lempty[0]==1&&lchess[1]>=3)||(rempty[0]==1&&rchess[1]>=3))
            return C4;
        else if((lempty[0]==1&&lchess[1]==2&&lempty[1]&&rempty[0])||(rempty[0]==1&&rchess[1]==2&&rempty[1]&&lempty[0]))
            return H3;
        else if((lchess[1]==2&&((lempty[0]==1&&(lempty[1]||rempty[0]))||lempty[0]==2))||(rchess[1]==2&&((rempty[0]==1&&(rempty[1]||lempty[0]))||rempty[0]==2))||lchess[2]||rchess[2])
            return M3;
        else if((lchess[1]==1&&lempty[1]&&rempty[0]&&((lempty[0]==1&&lempty[1]+rempty[0]>=3)||lempty[0]==2))||(rchess[1]==1&&rempty[1]&&lempty[0]&&((rempty[0]==1&&rempty[1]+lempty[0]>=3)||rempty[0]==2)))
            return H2;
        else if((lchess[1]==1&&(lempty[0]==3&&((lempty[1]||rempty[0])&&((lempty[0]==1&&lempty[1]+rempty[0]>=2)||lempty[0]==2))))||(rchess[1]==1&&(rempty[0]==3&&((rempty[1]||lempty[0])&&((rempty[0]==1&&rempty[1]+lempty[0]>=2)||rempty[0]==2)))))
            return M2;
        else
            return 0;
    }
}

int chessAI::evaluate(point& p){
    int sum=0;
    int values[15][15];
    memcpy(values,scores,sizeof(scores));
    int dx[4]={0,1,-1,1};
    int dy[4]={1,0,1,1};
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++){
            if(pieces[i][j]!=P_NONE){
                if(values[i][j])
                    values[i][j]=0;
                for(int k=0;k<4;k++){
                    int score=judge_type(i,j,dx[k],dy[k]);
                    if(p.statu!=-1){
                        if(pieces[i][j]!=pieces[p.x][p.y]&&score==L5)
                            p.statu=-L5;
                        else if(pieces[i][j]==pieces[p.x][p.y]&&score==L5&&p.statu!=-L5)
                            p.statu=L5;
                        else if(pieces[i][j]!=pieces[p.x][p.y]&&(score==H4||score==C4)&&p.statu!=L5&&p.statu!=-L5)
                            p.statu=-H4;
                        else if(pieces[i][j]==pieces[p.x][p.y]&&score==H4&&p.statu!=L5&&p.statu!=-L5&&p.statu!=-H4)
                            p.statu=H4;
                    }
                    if(pieces[i][j]==P_BLACK){
                        if(score==L5)
                            values[i][j]-=score;
                        else if(score==H4)
                            values[i][j]-=score;
                        else if(score==C4)
                            values[i][j]-=100*score;
                        else if(score==H3)
                            values[i][j]-=100*score;
                        else if(score==H2)
                            values[i][j]-=score;
                        else
                            values[i][j]-=score;
                    }
                    else
                        values[i][j]+=score;
                }
            }
        }
    for(int i=0;i<=14;i++)
        for(int j=0;j<=14;j++)
            sum+=values[i][j];
    return sum;
}

void chessAI::evaluate(int rowl,int rowr,int coll,int colr){
    int dx[4]={0,1,-1,1};
    int dy[4]={1,0,1,1};
    if(rowl==0&&rowr==14&&coll==0&&colr==14){//计算整个棋盘
        for(int i=0;i<=14;i++)
            for(int j=0;j<=14;j++){
                if(pieces[i][j]!=P_NONE){
                    if(scores[i][j])
                        scores[i][j]=0;
                    for(int k=0;k<4;k++){
                        int score=judge_type(i,j,dx[k],dy[k]);
                        if(pieces[i][j]==P_BLACK){//把棋型分开，为了便于更改权重
                            if(score==L5)
                                scores[i][j]-=score;
                            else if(score==H4)
                                scores[i][j]-=score;
                            else if(score==C4)
                                scores[i][j]-=100*score;
                            else if(score==H3)
                                scores[i][j]-=100*score;
                            else if(score==H2)
                                scores[i][j]-=score;
                            else
                                scores[i][j]-=score;
                        }
                        else
                            scores[i][j]+=score;
                    }
                }
                else
                    scores[i][j]=0;
            }
    }
    else{//部分更新
        int centerx=rowl+4;
        int centery=coll+4;
        for(int t=0;t<4;t++)
            for(int i=centerx-4*dx[t],j=centery-4*dy[t];(dx[t]==0||i!=centerx+5*dx[t])&&(dy[t]==0||j!=centery+5*dy[t]);i+=dx[t],j+=dy[t]){
                if(pieces[i][j]!=P_NONE){
                    if(i>=0&&i<=14&&j>=0&&j<=14&&!(t&&i==centerx&&j==centery)){
                        if(scores[i][j])
                            scores[i][j]=0;
                        for(int k=0;k<4;k++){
                            int score=judge_type(i,j,dx[k],dy[k]);
                            if(pieces[i][j]==P_BLACK){
                                if(score==L5)
                                    scores[i][j]-=score;
                                else if(score==H4)
                                    scores[i][j]-=score;
                                else if(score==C4)
                                    scores[i][j]-=100*score;
                                else if(score==H3)
                                    scores[i][j]-=100*score;
                                else if(score==H2)
                                    scores[i][j]-=score;
                                else
                                    scores[i][j]-=score;
                            }
                            else
                                scores[i][j]+=score;
                        }
                    }
                }
                else
                    scores[i][j]=0;
            }
    }
}

void chessAI::getpos(int turn,struct QVector<point>& pos){
    int visit[15][15];
    point p;
    for(int i=0;i<POS_NUM;i++){
        pos[i].x=-1;
        pos[i].y=-1;
        pos[i].score=(turn==T_WHITE)?INT_MIN:INT_MAX;
        pos[i].statu=INT_MIN;
    }
    memset(visit,false,sizeof(visit));
    //寻找有子位置周边的空点
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
            if(pieces[i][j]!=P_NONE)
                for(int a=i-2;a<=i+2;a++)
                    for(int b=j-2;b<=j+2;b++)
                        if(a>=0&&a<15&&b>=0&&b<15&&pieces[a][b]==P_NONE)
                            visit[a][b]=true;
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
            if(visit[i][j]){
                pieces[i][j]=(turn==T_WHITE)?P_WHITE:P_BLACK;
                p.x=i;
                p.y=j;
                p.statu=1;
                p.score=evaluate(p);
                if(p.statu==L5){
                    for(int i=0;i<POS_NUM;i++)
                        pos[i].statu=0;
                    pos[0]=p;
                    pieces[i][j]=P_NONE;
                    return;
                }
                for(int k=0;k<POS_NUM;k++)
                    if(pos[k].score==INT_MIN||pos[k].score==INT_MAX){
                        pos[k]=p;
                        break;
                    }
                    else if(turn==T_WHITE&&((p.statu==pos[k].statu&&p.score>pos[k].score)||p.statu>pos[k].statu)){
                        for(int t=POS_NUM-1;t>k;t--)
                            pos[t]=pos[t-1];
                        pos[k]=p;
                        break;
                    }
                    else if(turn==T_BLACK&&((p.statu==pos[k].statu&&p.score<pos[k].score)||p.statu>pos[k].statu)){
                        for(int t=POS_NUM-1;t>k;t--)
                            pos[t]=pos[t-1];
                        pos[k]=p;
                        break;
                    }//排序的规则是statu特殊标记优先，相等情况下考虑score
                pieces[i][j]=P_NONE;
            }
}

int chessAI::nextstep(int turn,int depth,int alpha,int beta)
{
    if(!depth){
        evaluate();
    }
    //计算评估值、剪枝
    QVector<point> pos(POS_NUM);
    int flag=0;
    getpos(turn,pos);//找到可能落子的地方（有子位置+-2），然后评估这些位置，找出最优的十个继续扩展
    if(turn==T_WHITE){
        for(int i=0;i<POS_NUM&&pos[i].statu;i++){
            point p=pos[i];
            if(p.statu==-L5||p.statu==-H4)
                continue;
            flag=1;
            pieces[p.x][p.y]=P_WHITE;
            evaluate(p.x-4,p.x+4,p.y-4,p.y+4);//落子
            int a=nextstep(turn^1,depth+1,alpha,beta);//下一层
            pieces[p.x][p.y]=P_NONE;
            evaluate(p.x-4,p.x+4,p.y-4,p.y+4);//还原
            if(a>alpha){
                alpha=a;
                if(!depth)
                    best_point=p;//更新最佳落点
            }
            if(beta<=alpha)
                break;//剪枝
        }
        if(!flag){//必输了但还是要下
            if(!depth)
                best_point=pos[0];//更新最佳落点
            return INT_MIN+1;
        }
        return alpha;
    }
    else{
        for(int i=0;i<POS_NUM&&pos[i].statu;i++){
            point p=pos[i];
            if(p.statu==-L5||p.statu==-H4)
                continue;
            flag=1;
            int b;
            if(depth==DEPTH_LIMIT-1)
                b=pos[0].score;//最后一层，直接返回评估值最优的点
            else{
                pieces[p.x][p.y]=P_BLACK;
                evaluate(p.x-4,p.x+4,p.y-4,p.y+4);//落子
                b=nextstep(turn^1,depth+1,alpha,beta);//下一层
                pieces[p.x][p.y]=P_NONE;
                evaluate(p.x-4,p.x+4,p.y-4,p.y+4);//还原
            }
            if(b<beta)
                beta=b;
            if(beta<=alpha)
                break;//剪枝
        }
        if(!flag)
            return INT_MAX-1;
        return beta;
    }

}
