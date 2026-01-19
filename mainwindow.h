#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_getCurrency_clicked();
    void onNetworkReply(QNetworkReply* reply);
    void calculateResult();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QMap<QString, double> ratesMap;
};
#endif // MAINWINDOW_H
