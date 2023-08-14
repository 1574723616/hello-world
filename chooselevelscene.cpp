#include "chooselevelscene.h"
#include <QPainter>
#include <QMenuBar>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QSound>

#pragma execution_character_set("utf-8")

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景
    this->setFixedSize(320,588);
    //设置图标
     this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("选择关卡场景");

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

    //选择关卡的音效
    QSound *startSound = new QSound(":/res/TapButtonSound.wav",this);

    //点击返回按钮的音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);

    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=](){
        //播放点击返回按钮的音效
        backSound->play();
        //告诉主场景 我返回了 主场景监听ChooseLevelScene的返回按钮
        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    //创建选择关卡的按钮
    for(int i=0;i<20;i++)
    {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25+i%4*70,130+i/4*70);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            //播放选择关卡的音效资源
            startSound->play();

            QString str = QString("您选择的是第 %1 关").arg(i+1);
            qDebug()<<str;

            //进入到游戏场景
            this->hide(); //将选关场景隐藏掉
            play = new PlayScene(i+1); //创建游戏场景

            //设置游戏场景的初始位置
            play->setGeometry(this->geometry());
            play->show(); //显示游戏场景


            connect(play,&PlayScene::chooseSceneBack,[=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play = NULL;
            });
        });

        QLabel *lable = new QLabel(this);
        lable->setFixedSize(menuBtn->width(),menuBtn->height());
        lable->setText(QString::number(i+1));
        lable->move(25+i%4*70,130+i/4*70);

        //设置lable上的文字对齐方式 水平居中 和垂直居中
        lable->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        //设置让鼠标进行穿透 51号属性
        lable->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //画背景上图标
    pix.load(":/res/Title.png");

    //绘制标题
    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix.width(),pix.height(),pix);
}

