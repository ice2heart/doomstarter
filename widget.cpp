#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QRadioButton>
#include <QCheckBox>


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
    mPk3s = mSettings.value("pk3s", QStringList()).toStringList();
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
    QStringList pk3s(wadDir.entryList(QStringList("*.pk3")));
    for (const auto &pk3: pk3s) {
        QCheckBox *cb = new QCheckBox(pk3, this);
        if (mPk3s.contains(pk3, Qt::CaseInsensitive)) {
            cb->setChecked(true);
        }
        connect(cb, &QCheckBox::toggled, [pk3, this](bool checked){this->pk3State(checked, pk3);});

        ui->verticalLayout_3->addWidget(cb);
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

    QStringList wadsList = this->mPk3s;
    for (auto &wad: wadsList){
        args.append("-file");
        args.append(wadsDir.filePath(wad));
    }
    //connect
    args.append("+connect");
    args.append(mSettings.value("server", QString("ice2heart.com")).toString());
    //nick
    args.append("+Name");
    args.append(ui->lineEditNick->text());
    qDebug()<<args;
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

void Widget::pk3State(int isChecked, const QString &pk3)
{
    if (isChecked){
        this->mPk3s.append(pk3);

    } else {
        this->mPk3s.removeAll(pk3);

    }
    mSettings.setValue("pk3s", this->mPk3s);
}
