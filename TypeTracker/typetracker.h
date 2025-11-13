#pragma once

#include <QTime>
#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class TypeTracker;
}
QT_END_NAMESPACE

class BaseTypeTracker
{
public:
    BaseTypeTracker();
    virtual ~BaseTypeTracker();

protected:
    QTime m_counterTime;
    int m_wordIndex;
    int m_error;
    int m_amount;
    float m_chars;
    bool m_pastError;
    QVector<QString> m_words;
    QVector<QString> m_userWords;
};

class TypeTracker : public QMainWindow, public BaseTypeTracker

{
    Q_OBJECT

public:
    explicit TypeTracker(QWidget *parent = nullptr);
    ~TypeTracker();

private slots:
    void onStartButtonClicked();
    void updateCounter();
    void onInputTextChanged(const QString& text);

private:
    void resetSession();

    Ui::TypeTracker *ui;
    QTimer *m_timer;
};
