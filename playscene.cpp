#include "playscene.h"
#include <QDebug>
#include <QPainter>
#include <QMenuBar>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>


#pragma execution_character_set("utf-8")
//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}

PlayScene::PlayScene(int levelNum){
    QString str = QString("进入了第 %1 关").arg(levelNum);
    qDebug()<<str;
    this->levelIndex = levelNum;


    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(320,588);
    //设置图标
     this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币场景");

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //点击返回按钮的音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //翻金币的音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //胜利按钮音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);


    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    connect(backBtn,&MyPushButton::clicked,[=](){
        //播放点击返回按钮的音效资源
        backSound->play();
        //告诉主场景 我返回了 主场景监听ChooseLevelScene的返回按钮
         qDebug()<<"翻金币的场景中，点击了返回按钮";
        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    //显示当前的关卡数
    QLabel *lab = new QLabel(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    //将字体设置到标签控件中
    lab->setFont(font);
    //lab->setText("Level:"+QString::number(this->levelIndex));
    lab->setText(QString("level:%1").arg(this->levelIndex));
    lab->setGeometry(30,this->height()-50,120,50);


    dataConfig config;
    //初始化每个关卡的二维数组
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
         this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片显示
    QLabel * winLable = new QLabel(this);
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLable->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLable->setPixmap(tmpPix);
    winLable->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());


    //显示金币背景图案
    for(int i = 0;i<4;i++)
    {
        for (int j = 0; j < 4; j++)
        {
            //绘制背景图案
            QPixmap pix = QPixmap(":/res/BoardNode(1).png");
            QLabel *lable = new QLabel(this);
            lable->setGeometry(0,0,pix.width(),pix.height());
            lable->setPixmap(pix);
            lable->move(57+i*50,200+j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = ":/res/Coin0001.png";
            }
            else
            {
                //显示银币
                str = ":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*50,204+j*50);

            //给金币属性赋值
            coin->poxX = i;
            coin->poxY = j;
            coin->flag = gameArray[i][j]; //1就是正面 0就是反面

            //将金币放入到 金币的二维数组里 以便后期的维护
            coinBtn[i][j] = coin;
            //点击金币进行翻转
            connect(coin,&MyCoin::clicked,[=](){
                //播放翻金币的音效资源
                flipSound->play();

                //点击按钮 将所有的按钮先都禁用
                for(int i = 0;i<4;i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        this->coinBtn[i][j]->isWin = true;
                    }
                 }


                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;

                //翻转周围的金币
                QTimer::singleShot(300,this,[=](){
                    if(coin->poxX+1 <= 3)
                    {
                        coinBtn[coin->poxX+1][coin->poxY]->changeFlag();
                        this->gameArray[coin->poxX+1][coin->poxY] = this->gameArray[coin->poxX+1][coin->poxY] == 0 ? 1 : 0;
                    }
                    //周围的左侧翻金币的条件
                    if(coin->poxX-1 >= 0)
                    {
                        coinBtn[coin->poxX-1][coin->poxY]->changeFlag();
                        this->gameArray[coin->poxX-1][coin->poxY] = this->gameArray[coin->poxX-1][coin->poxY] == 0 ? 1 : 0;
                    }
                    //周围的上侧翻金币的条件
                    if(coin->poxY+1 <= 3)
                    {
                        coinBtn[coin->poxX][coin->poxY+1]->changeFlag();
                        this->gameArray[coin->poxX][coin->poxY+1] = this->gameArray[coin->poxX][coin->poxY+1] == 0 ? 1 : 0;
                    }
                    //周围的下侧翻金币的条件
                    if(coin->poxY-1 >= 0)
                    {
                        coinBtn[coin->poxX][coin->poxY-1]->changeFlag();
                        this->gameArray[coin->poxX][coin->poxY-1] = this->gameArray[coin->poxX][coin->poxY-1] == 0 ? 1 : 0;
                    }

                    //翻完周围的金币后 解开所有金币的禁用
                    for(int i = 0;i<4;i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            this->coinBtn[i][j]->isWin = false;
                        }
                     }

                    //判断是否胜利
                    this->isWin =true;
                    for(int i = 0;i<4;i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if(coinBtn[i][j]->flag ==false) //只要有一个反面就银币 就是失败
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                     }
                    if(this->isWin == true)
                    {
                        //播放胜利的音效资源
                        winSound->play();
                        qDebug()<<"胜利了";
                        //将所有按钮的胜利标志改为true 如果再次点击按钮 直接return掉，不做响应
                        for(int i = 0;i<4;i++)
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                coinBtn[i][j]->isWin = true;
                            }
                         }

                        //将胜利的图片移动下来
                        //创建动态对象
                        QPropertyAnimation * animation = new QPropertyAnimation(winLable,"geometry");
                        //设置动画的事件间隔
                        animation->setDuration(1000);
                        //起始位置
                        animation->setStartValue(QRect(winLable->x(),winLable->y(),winLable->width(),winLable->height()));
                        //结束位置
                        animation->setEndValue(QRect(winLable->x(),winLable->y()+114,winLable->width(),winLable->height()));
                        //设置弹跳曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();

                    }

                });

            });

        }
    }


}

void PlayScene::paintEvent(QPaintEvent *)
{
    //创建背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //画背景上图标
    pix.load(":/res/Title.png");

    //绘制标题
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
