#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include <mypushbutton.h>
#include <QTimer>
#include <QSound> //多媒体模块下 的音效 头文件

#pragma execution_character_set("utf-8")


MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //配置主场景
    //设置固定大小
    setFixedSize(320,588);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币主场景");
    //退出按钮的实现
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    //准备开始按钮的音效
    QSound *startSound = new QSound(":/res/TapButtonSound.wav",this);

    //开始按钮实现
    MyPushButton *startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move((this->width()- startBtn->width())/2,this->height() * 0.7);

    //实例化选择关卡的场景
    chooseSence = new ChooseLevelScene;

    //监听选择关卡的返回按钮的信号
    connect(chooseSence,&ChooseLevelScene::chooseSceneBack,[=](){
        this->setGeometry(chooseSence->geometry());
        chooseSence->hide(); //将选择的关卡场景 隐藏掉
        this->show(); //重新显示主场景
    });

    connect(startBtn,&MyPushButton::clicked,[=](){
        //播放开始的音效资源
        startSound->play();
        //startSound->setLoops(10);  播放几次 改成-1可以实现无限循环

        startBtn->zoom(0,10);
        startBtn->zoom(10,0);


        //延时进入到选择关卡的场景中
        QTimer::singleShot(500,this,[=](){
            //设置chooseSence场景的位置
            chooseSence->setGeometry(this->geometry());
            //自身隐藏
            this->hide();
            //显示选择关卡场景
            chooseSence->show();

        });
    });


}
void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //画背景上图标
    pix.load(":/res/Title.png");
    //缩放图片
   // pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    //绘制标题
    painter.drawPixmap(10,30,pix.width()*0.5,pix.height()*0.5,pix);
}

MainScene::~MainScene()
{
    delete ui;
}

