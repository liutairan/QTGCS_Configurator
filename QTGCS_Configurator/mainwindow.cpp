#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    removeToolBar(ui->mainToolBar);
    appPath = "";
    mmAppPath = "";
    currentWorkingPath = "";
    cfgFilePath = "";
    mmCfgFilePath = "";
    cfgp = new ConfigureProperty();
    // Product type is the system type, for example,
    //    macos, winrt, etc.
    //    This term is useful because the paths are
    //    different on each system.
    cfgp->productType = QSysInfo::productType();
    cfgp->productVersion = QSysInfo::productVersion();
    cfgp->CFGAppPath = QCoreApplication::applicationDirPath();

    // In MacOS, the application should end with .app, but the above
    //    function gives the path to the excutable file, which is
    //    the right path, but I prefer the one with .app.
    //    In this way, users are unlikely to chose a wrong path if
    //    not familiar with Mac application system.
    QString tempEnd = "/Contents/MacOS";
    if (cfgp->CFGAppPath.endsWith(tempEnd))
    {
        int tempIndex = cfgp->CFGAppPath.indexOf(tempEnd);
        if (tempIndex > 0)
        {
            cfgp->CFGAppPath.replace(tempIndex, tempEnd.length(), "");
        }
    }
    ui->cfgPathEdit->setText(cfgp->CFGAppPath);
    QStringList mapTypeList;
    mapTypeList << "hybrid" << "satellite" << "terrain" << "roadmap";
    ui->mapTypeComboBox->addItems(mapTypeList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray MainWindow::readJsonFile(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        f.close();
        return QString().toUtf8();
    } else {
        QTextStream in(&f);
        QByteArray retValue = in.readAll().toUtf8();
        f.close();
        return retValue;
    }
}

void MainWindow::writeJsonFile(const QString &filename, QString outString)
{
    QFile f(filename);
    if (!f.open(QFile::WriteOnly | QFile::Text)) {
        f.close();
    } else {
        QTextStream out(&f);
        out << outString;
        f.close();
    }
}

void MainWindow::readConfigFile()
{
    QByteArray var = readJsonFile(cfgp->cfgFilePath);
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(var);
    QJsonObject cfgJsonObj = doc.object();
    cfgp->ResourcePath = cfgJsonObj["Resource"].toString();
    cfgp->mapImagePath = cfgJsonObj["Map Resource"].toString();
    cfgp->mapIndexPath = cfgJsonObj["Map Index"].toString();
    cfgp->xbeeAddrPath = cfgJsonObj["XBEE Address"].toString();
    cfgp->logFilePath = cfgJsonObj["Log"].toString();

    cfgp->zoomLevel = cfgJsonObj["Zoom Level"].toString();
    cfgp->mapKey = cfgJsonObj["Map Key"].toString();
}

void MainWindow::writeConfigFile()
{
    // Compose configure file
    QJsonObject cfgJsonObj;
    cfgJsonObj.insert("GCS", cfgp->GCSAppPath);
    cfgJsonObj.insert("CFG", cfgp->CFGAppPath);
    cfgJsonObj.insert("Map Manager", cfgp->MMAppPath);

    cfgJsonObj.insert("Resource", cfgp->ResourcePath);
    cfgJsonObj.insert("Map Resource", cfgp->mapImagePath);
    cfgJsonObj.insert("Map Index", cfgp->mapIndexPath);
    cfgJsonObj.insert("XBEE Address", cfgp->xbeeAddrPath);
    cfgJsonObj.insert("Log", cfgp->logFilePath);

    cfgJsonObj.insert("Map Type", cfgp->mapType);
    cfgJsonObj.insert("Zoom Level", cfgp->zoomLevel);
    cfgJsonObj.insert("Map Key", cfgp->mapKey);
    QJsonDocument cfgJsonDoc;
    cfgJsonDoc.setObject(cfgJsonObj);

    // Write to configure file
    writeJsonFile(cfgp->cfgFilePath, cfgJsonDoc.toJson());
    writeJsonFile(cfgp->mmCfgFilePath, cfgJsonDoc.toJson());
    //
}

void MainWindow::dirAssert()
{
    if(QDir(cfgp->ResourcePath).exists())
    {
        ;
    }
    else
    {
        QDir().mkdir(cfgp->ResourcePath);
    }

    if(QDir(cfgp->mapImagePath).exists())
    {
        ;
    }
    else
    {
        QDir().mkdir(cfgp->mapImagePath);
    }

    if(QDir(cfgp->xbeeAddrDir).exists())
    {
        ;
    }
    else
    {
        QDir().mkdir(cfgp->xbeeAddrPath);
    }

    if(QDir(cfgp->logFilePath).exists())
    {
        ;
    }
    else
    {
        QDir().mkdir(cfgp->logFilePath);
    }
}

void MainWindow::on_saveCFGButton_clicked()
{
    cfgp->mapType = ui->mapTypeComboBox->currentText();
    dirAssert();
    writeConfigFile();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this, "Saved", "Configuration file saved.",
                                    QMessageBox::Ok);
    QApplication::quit();
}

void MainWindow::setUI()
{
    ui->resourcePathEdit->setText(cfgp->ResourcePath);
    ui->mapResourceEdit->setText(cfgp->mapImagePath);
    ui->mapIndexPathEdit->setText(cfgp->mapIndexPath);
    ui->xbeeAddrEdit->setText(cfgp->xbeeAddrPath);
    ui->logPathEdit->setText(cfgp->logFilePath);

    ui->zoomLevelEdit->setText(cfgp->zoomLevel);
    ui->mapKeyEdit->setText(cfgp->mapKey);
}

void MainWindow::on_cancelButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_mainPathBrowseButton_clicked()
{
    if (cfgp->productType == "osx")
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Choose App File", "", "App Package (*.app);;All files (*.*)");
        if (fileName.length() > 0)
        {
            if (fileName.endsWith("QTGCS.app"))
            {
                //qDebug() << fileName;
                appPath = fileName;
                cfgp->GCSAppPath = fileName;
                ui->mainPathEdit->setText(cfgp->GCSAppPath);
                cfgFilePath = fileName + "/Contents/MacOS/config.json";
                cfgp->cfgFilePath = cfgFilePath;
                readConfigFile();
                setUI();
            }
            else
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::critical(this, "Error", "Please choose the valid application.",
                                                QMessageBox::Ok);
            }
        }
    }
    else if (cfgp->productType == "macos")
    {;}
    else if (cfgp->productType == "ios")
    {;}
    else if (cfgp->productType == "tvos")
    {;}
    else if (cfgp->productType == "watchos")
    {;}
    else if (cfgp->productType == "darwin")
    {;}
    else if (cfgp->productType == "android")
    {;}
    else if (cfgp->productType == "debian")
    {;}
    else if (cfgp->productType == "winrt")
    {;}
    else if (cfgp->productType == "windows")
    {;}
    else if (cfgp->productType == "unknown")
    {;}
    else
    {;}
    /*
    QString fileName = QFileDialog::getOpenFileName(this, "Choose App File", "", "App Package (*.app);;All files (*.*)");
    if (fileName.length() > 0)
    {
        qDebug() << fileName;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Cannot open file";
        }
    }*/
}

void MainWindow::on_cfgAppBrowseButton_clicked()
{

}

void MainWindow::on_resourceBrowseButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose Resource Directory", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dirPath.length() > 0)
    {
        if (true)
        {
            //qDebug() << dirPath;
            cfgp->ResourcePath = dirPath + "/";
            ui->resourcePathEdit->setText(cfgp->ResourcePath);
            cfgp->mapImagePath = dirPath + "/mapscache/";
            ui->mapResourceEdit->setText(cfgp->mapImagePath);
            cfgp->mapIndexPath = dirPath + "/mapcache.txt";
            ui->mapIndexPathEdit->setText(cfgp->mapIndexPath);
        }
        else
        {
            // Not used now, check back later.
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, "Error", "Please choose the valid application.",
                                            QMessageBox::Ok);
        }
    }
}

void MainWindow::on_autoSetButton_clicked()
{
    cfgp->mapImagePath = cfgp->ResourcePath + "mapscache/";
    cfgp->mapIndexPath = cfgp->ResourcePath + "mapcache.txt";
    cfgp->xbeeAddrDir = cfgp->ResourcePath + "xbeeaddr/";
    cfgp->xbeeAddrPath = cfgp->xbeeAddrDir + "xbeeaddrlist.txt";
    cfgp->logFilePath = cfgp->ResourcePath + "log/";
    setUI();
}

void MainWindow::on_mmBrowseButton_clicked()
{
    if (cfgp->productType == "osx")
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Choose App File", "", "App Package (*.app);;All files (*.*)");
        if (fileName.length() > 0)
        {
            if (fileName.endsWith("QTGCS_MapManager.app"))
            {
                //qDebug() << fileName;
                mmAppPath = fileName;
                cfgp->MMAppPath = fileName;
                ui->mmPathEdit->setText(cfgp->MMAppPath);
                mmCfgFilePath = fileName + "/Contents/MacOS/config.json";
                cfgp->mmCfgFilePath = mmCfgFilePath;
                //cfgp->cfgFilePath = cfgFilePath;
                //readConfigFile();
                //setUI();
            }
            else
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::critical(this, "Error", "Please choose the valid application.",
                                                QMessageBox::Ok);
            }
        }
    }
    else if (cfgp->productType == "macos")
    {;}
    else if (cfgp->productType == "ios")
    {;}
    else if (cfgp->productType == "tvos")
    {;}
    else if (cfgp->productType == "watchos")
    {;}
    else if (cfgp->productType == "darwin")
    {;}
    else if (cfgp->productType == "android")
    {;}
    else if (cfgp->productType == "debian")
    {;}
    else if (cfgp->productType == "winrt")
    {;}
    else if (cfgp->productType == "windows")
    {;}
    else if (cfgp->productType == "unknown")
    {;}
    else
    {;}
}
