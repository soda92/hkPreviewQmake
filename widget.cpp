#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    userID=-1;
    previewID=-1;
    connect(ui->LoginButton,&QPushButton::clicked,this,&Widget::OnClickedLogin);
    connect(ui->LogoutButton,&QPushButton::clicked,this,&Widget::OnClickedLogout);
    connect(ui->PreviewButton,&QPushButton::clicked,this,&Widget::OnClickedPreview);
    connect(ui->StopPreviewButton,&QPushButton::clicked,this,&Widget::OnClickedStopPreview);
    connect(ui->CaptureButton,&QPushButton::clicked,this,&Widget::OnClickedCapture);
    if(initSDK())
        qDebug()<<"SDK init Success!"<<endl;
}

Widget::~Widget()
{
    if(uninitSDK())
        qDebug()<<"SDK uninit Success!"<<endl;
    delete ui;
}

long Widget::play(HWND hWnd)
{
    qDebug() << "HWND: " << reinterpret_cast<long long>(hWnd);
//    hWnd = (HWND)0x9096e;
    hWnd = (HWND)271922;
    NET_DVR_CLIENTINFO ClientInfo;
    ClientInfo.hPlayWnd     = hWnd;
    ClientInfo.lChannel     = 1;
    ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
    long IRealPlayHandle= NET_DVR_RealPlay_V30(userID,&ClientInfo,NULL,NULL,TRUE);

    if(IRealPlayHandle <0)
    {
        qDebug() << "NET_DVR_RealPlay_V40 error;error number " <<NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_RealPlay_V40 error;error number "+QString::number(NET_DVR_GetLastError()));
        NET_DVR_Logout(userID);
        userID=-1;
        return -1;
    }

    return IRealPlayHandle;
}

bool Widget::initSDK()
{
    bool isok = NET_DVR_Init();
    if(isok == false)
    {
        qDebug()<< "NET_DVR_Init error;error number is " <<NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_Init error;error number is "+QString::number(NET_DVR_GetLastError()));
        return isok;
    }
    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(5000, 1);
    NET_DVR_SetReconnect(10000, true);
    return isok;
}

bool Widget::uninitSDK()
{
    bool isok = NET_DVR_Cleanup();
    if(isok == false)
    {
        qDebug()<< "NET_DVR_Cleanup error;error number is " <<NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_Cleanup error;error number "+QString::number(NET_DVR_GetLastError()));
        return isok;
    }
    return isok;
}

void Widget::OnClickedLogin()
{
//    NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
//    struLoginInfo.bUseAsynLogin = 0;			 //同步登录方式
//    strcpy_s(struLoginInfo.sDeviceAddress, "192.168.104.72");	 //设备IP地址
//    struLoginInfo.wPort = 8000;					 //设备服务端口
//    strcpy_s(struLoginInfo.sUserName, "admin");	 //设备登录用户名
//    strcpy_s(struLoginInfo.sPassword, "hk123456"); //设备登录密码
//    //strcpy(struLoginInfo.sPassword, "123456"); //设备登录密码

//    //设备信息, 输出参数
//    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };
////    NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
////    char sDVRIP[100]{"192.168.104.72"};
////    char sUserName[100]{"admin"};
////    char sPassword[100]{"hk123456"};
//    userID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);

    NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
    char sDVRIP[100]{"192.168.104.72"};
    char sUserName[100]{"admin"};
    char sPassword[100]{"hk123456"};
    userID=NET_DVR_Login_V30(sDVRIP,8000,sUserName,sPassword,&DeviceInfoTmp);
    if(userID < 0)
    {
        qDebug() << "NET_DVR_Login_V30 error;" << "error number is " << NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_Login_V30 error;error number "+QString::number(NET_DVR_GetLastError()));
        return;
    }
    qDebug()<<"Login Success,userID:" << userID<<endl;
}

void Widget::OnClickedLogout()
{
    if(userID!=-1)
    {
        if(NET_DVR_Logout(userID)==false)
        {
            qDebug() << "NET_DVR_Logout error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_Logout error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        userID=-1;
        qDebug() <<"Logout Success!"<<endl;
    }
}

void Widget::OnClickedPreview()
{
    if(userID==-1)
    {
        qDebug() <<"Need Login!"<<endl;
        QMessageBox::warning(this,"error","Need Login!");
        return;
    }
    HWND hWnd = (HWND)ui->label->winId();
    previewID = play(hWnd);
    if(previewID==-1)
    {
        return;
    }
    qDebug()<<"Preview Success!" << userID<<endl;
}

void Widget::OnClickedStopPreview()
{
    if(previewID!=-1)
    {
        if(NET_DVR_StopRealPlay(previewID)==false)
        {
            qDebug() << "NET_DVR_StopRealPlay error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_StopRealPlay error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        previewID=-1;
        qDebug() <<"Stop Preview Success!"<<endl;
    }
}

void Widget::OnClickedCapture()
{

    if(userID!=-1)
    {
        NET_DVR_JPEGPARA IpJpegPara2;
        IpJpegPara2.wPicQuality=0;
        IpJpegPara2.wPicSize=0xff;

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy-MM-dd_hh-mm-ss_zzz");
        current_date="C:\\Picture\\"+current_date+".jpg";
        char*  fileName;
        QByteArray ba = current_date.toLatin1(); // must
        fileName=ba.data();


        if(NET_DVR_CaptureJPEGPicture(userID,1,&IpJpegPara2,fileName)==false)
        {
            qDebug() << "NET_DVR_CaptureJPEGPicture error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_CaptureJPEGPicture error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        qDebug() <<"Capture Success!"<<endl;
    }
}
