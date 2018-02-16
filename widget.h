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

private:
    Ui::Widget *ui;
    QSettings mSettings;
    QString mMainWad;
    QString mWadDirPath;
};

#endif // WIDGET_H
