#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QMessageBox>

#include <configureproperty.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString appPath;
    QString mmAppPath;
    QString currentWorkingPath;
    QString cfgFilePath;
    QString mmCfgFilePath;
    ConfigureProperty *cfgp;
    QByteArray readJsonFile(const QString &filename);
    void writeJsonFile(const QString &filename, QString outString);
    void readConfigFile();
    void writeConfigFile();
    void setUI();
    void dirAssert();

private slots:
    void on_saveCFGButton_clicked();

    void on_cancelButton_clicked();

    void on_mainPathBrowseButton_clicked();

    void on_cfgAppBrowseButton_clicked();

    void on_resourceBrowseButton_clicked();

    void on_autoSetButton_clicked();

    void on_mmBrowseButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
