﻿#include "../transfer/transferringwidget.h"
#include "zipfileprocesswidget.h"
#include "zipfileprocessresultwidget.h"
#include "../type_defines.h"
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QTextBrowser>
#include <QMovie>
#include <QStackedWidget>

#include <gui/connect/choosewidget.h>
#include <utils/transferhepler.h>
#include <utils/optionsmanager.h>
#pragma execution_character_set("utf-8")

zipFileProcessWidget::zipFileProcessWidget(QWidget *parent) : QFrame(parent)
{
    initUI();
}

zipFileProcessWidget::~zipFileProcessWidget() { }

void zipFileProcessWidget::updateProcess(const QString &content, int processbar, int estimatedtime)
{
    if (OptionsManager::instance()->getUserOption(Options::kTransferMethod)[0]
        == TransferMethod::kNetworkTransmission) {
        return;
    }
    // Transfer success or failure to go to the next page
    if (processbar == 100 || processbar == -1) {
        nextPage();
        return;
    }
    changeFileLabel(content);
    changeTimeLabel(estimatedtime);
    changeProgressBarLabel(processbar);
}

void zipFileProcessWidget::changeFileLabel(const QString &path)
{
    fileLabel->setText(QString("正在打包 %1").arg(path));
}

void zipFileProcessWidget::changeTimeLabel(const int &time)
{
    if (time > 60) {
        int textTime = time / 60;
        timeLabel->setText(QString("预计迁移时间还剩 %1分钟").arg(QString::number(textTime)));
    }else{
         timeLabel->setText(QString("预计迁移时间还剩 %1秒").arg(QString::number(time)));
    }
}

void zipFileProcessWidget::changeProgressBarLabel(const int &processbar)
{
    progressLabel->setProgress(processbar);
}

void zipFileProcessWidget::initUI()
{
    setStyleSheet("background-color: white; border-radius: 10px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->addSpacing(30);

    QLabel *iconLabel = new QLabel(this);
    iconLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    QMovie *iconmovie = new QMovie(this);
    iconmovie->setFileName(":/icon/GIF/light/compress.gif");
    iconmovie->setScaledSize(QSize(200, 160));
    iconmovie->setSpeed(80);
    iconmovie->start();
    iconLabel->setMovie(iconmovie);

    QLabel *titileLabel = new QLabel("正在创建备份文件…", this);
    titileLabel->setFixedHeight(50);
    QFont font;
    font.setPointSize(16);
    font.setWeight(QFont::DemiBold);
    titileLabel->setFont(font);
    titileLabel->setAlignment(Qt::AlignHCenter);

    progressLabel = new ProgressBarLabel(this);
    progressLabel->setAlignment(Qt::AlignCenter);
    progressLabel->setProgress(0);
    progressLabel->setFixedSize(280, 8);
    QHBoxLayout *progressLayout = new QHBoxLayout(this);
    progressLayout->addWidget(progressLabel, Qt::AlignCenter);

    timeLabel = new QLabel(this);
    timeLabel->setText(QString("预计完成时间还剩 %1分钟").arg(QString::number(0)));
    timeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QFont timefont;
    font.setPointSize(7);
    timeLabel->setFont(timefont);

    fileLabel = new QLabel(this);
    fileLabel->setFixedSize(220, 15);
    fileLabel->setStyleSheet(".QLabel {"
                             "opacity: 1;"
                             "color: rgba(82,106,127,1);"
                             " font-family: \"SourceHanSansSC-Normal\";"
                             " font-size: 12px;"
                             "font-weight: 400;"
                             "font-style: normal;"
                             "letter-spacing: 0px;"
                             "text-align: left;"
                             "}");
    fileLabel->setTextFormat(Qt::PlainText);
    fileLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    fileLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QHBoxLayout *fileLabelLayout = new QHBoxLayout();
    fileLabelLayout->setAlignment(Qt::AlignCenter);
    fileLabelLayout->addWidget(fileLabel);

    IndexLabel *indelabel = new IndexLabel(3, this);
    indelabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *indexLayout = new QHBoxLayout();
    indexLayout->addWidget(indelabel, Qt::AlignCenter);

    mainLayout->setAlignment(Qt::AlignHCenter);

    mainLayout->addSpacing(50);
    mainLayout->addWidget(iconLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(titileLabel);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(progressLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(timeLabel);
    mainLayout->addLayout(fileLabelLayout);
    mainLayout->addLayout(indexLayout);

    QObject::connect(TransferHelper::instance(), &TransferHelper::transferContent, this,
                     &zipFileProcessWidget::updateProcess);
}

void zipFileProcessWidget::nextPage()
{
    QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parent());
    if (stackedWidget) {
        stackedWidget->setCurrentIndex(PageName::zipfileprocessresultwidget);
    } else {
        qWarning() << "Jump to next page failed, qobject_cast<QStackedWidget *>(this->parent()) = "
                      "nullptr";
    }
}
