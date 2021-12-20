#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>
#include "include/HCNetSDK.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    long play(HWND hWnd);
    bool initSDK();
    bool uninitSDK();
public slots:
    void OnClickedLogin();
    void OnClickedLogout();
    void OnClickedPreview();
    void OnClickedStopPreview();
    void OnClickedCapture();
private:
    Ui::Widget *ui;
    long userID;
    long previewID;
};

#endif // WIDGET_H
