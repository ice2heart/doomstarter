#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QRadioButton>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    mSettings(qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat),
    mMainWad(),
    mWadDirPath()
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    QString nick = mSettings.value("nick", QString("WhopWhop")).toString();
    ui->lineEditNick->setText(nick);

    mWadDirPath = mSettings.value("wadDir", QString("wads")).toString();
    QString selectedWad = mSettings.value("mainWad", QString("doom.wad")).toString();
    mMainWad = selectedWad;
    QDir wadDir(mWadDirPath);
    QStringList wads(wadDir.entryList(QStringList("*.wad")));
    for (const auto &wad: wads){
        QRadioButton *rb = new QRadioButton(wad, this);
        if (wad == selectedWad) {
            rb->setChecked(true);
        }
        connect(rb, &QRadioButton::clicked, [wad, this](){this->selectedMainWad(wad);});
        ui->verticalLayout_2->addWidget(rb);
    }

    connect(ui->pushButtonRun, SIGNAL(clicked()), this, SLOT(runDoom()));
    connect(ui->lineEditNick, SIGNAL(textChanged(QString)), this, SLOT(changeNick(QString)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::runDoom() const
{
    QStringList args;
    QDir wadsDir(mWadDirPath);
    //main wad
    args.append("-iwad");
    args.append(wadsDir.filePath(mMainWad));
    //wads
    args.append("-file");
    QString wads = mSettings.value("wads", QString("bd21test-jan02.pk3 skulltag_actors.pk3")).toString();
    QStringList wadsList = wads.split(" ");
    for (auto &wad: wadsList){
        wad = wadsDir.filePath(wad);
    }
    args.append(wadsList);
    //connect
    args.append("+connect");
    args.append(mSettings.value("server", QString("ice2heart.com")).toString());
    //nick
    args.append("+Name");
    args.append(ui->lineEditNick->text());
    QString doomApp = mSettings.value("doomBin", QString("zandronum.exe")).toString();

    QProcess::startDetached(QDir(qApp->applicationDirPath()).filePath(doomApp), args);
    qApp->exit(0);
}

void Widget::changeNick(const QString &text)
{
    mSettings.setValue("nick", text);
}

void Widget::selectedMainWad(const QString &text)
{
    mSettings.setValue("mainWad", text);
    mMainWad = text;
}
