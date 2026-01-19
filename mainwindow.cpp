#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);

    ui->spinBox->setRange(0, 999999999);


    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);


    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::calculateResult);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::calculateResult);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getCurrency_clicked()
{
    QString baseCurrency = ui->lineEdit->text().trimmed().toUpper();
    if(baseCurrency.isEmpty()) return;

    QString url = QString("https://open.er-api.com/v6/latest/%1").arg(baseCurrency);
    manager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::onNetworkReply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        qint64 unixTime = obj["time_last_update_unix"].toVariant().toLongLong();
        ui->lineEdit_2->setText(QDateTime::fromSecsSinceEpoch(unixTime).toString("dd/MM/yyyy HH:mm:ss"));

        QJsonObject ratesObj = obj["rates"].toObject();
        ratesMap.clear();
        ui->comboBox->clear();

        for (const QString &key : ratesObj.keys()) {
            double rate = ratesObj[key].toDouble();
            ratesMap.insert(key, rate);
            ui->comboBox->addItem(key);
        }

        int idrIndex = ui->comboBox->findText("IDR");
        if (idrIndex != -1) ui->comboBox->setCurrentIndex(idrIndex);

        calculateResult();
    }
    reply->deleteLater();
}

void MainWindow::calculateResult()
{
    QString target = ui->comboBox->currentText();

    if (ratesMap.contains(target)) {
        double rate = ratesMap.value(target);
        int amount = ui->spinBox->value();
        double total = amount * rate;

        ui->lineEdit_3->setText(QString::number(total, 'f', 2));
    }
}
