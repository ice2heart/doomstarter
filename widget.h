#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void runDoom() const;
    void changeNick(const QString &text);
    void selectedMainWad(const QString &text);
    void pk3State(int isChecked, const QString &pk3);

private:
    Ui::Widget *ui;
    QSettings mSettings;
    QString mMainWad;
    QString mWadDirPath;
    QStringList mPk3s;
};

#endif // WIDGET_H
