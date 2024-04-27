#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtScript/QScriptEngine>
#include <QPushButton>
#include <QTextEdit>
#include <QTextBrowser>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNumberButtonClicked();
    void onOperatorButtonClicked();
    void onEqualsButtonClicked();
    void onCommaButtonClicked();
    void onClearButtonClicked();
    void onDeleteButtonClicked();
    void onChangeSignButtonClicked();
    void updateDisplay2();
    void onPercenButtonClicked();


private:
    Ui::MainWindow *ui;
    QTextEdit *display1;
    QString replacement;

    QScriptEngine engine;

};

#endif // MAINWINDOW_H
