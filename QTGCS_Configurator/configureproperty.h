#ifndef CONFIGUREPROPERTY_H
#define CONFIGUREPROPERTY_H

#include <QString>
#include <QDebug>

class ConfigureProperty
{
public:
    ConfigureProperty();
    QString productType;
    QString productVersion;
    QString GCSAppPath;
    QString CFGAppPath;
    QString MMAppPath;
    QString cfgFilePath;
    QString ResourcePath;
    QString mapImagePath;
    QString mapIndexPath;
    QString xbeeAddrDir;
    QString xbeeAddrPath;
    QString logFilePath;
    QString mapType;
    QString zoomLevel;
    QString mapKey;
    QString originLat;
    QString originLon;
};

#endif // CONFIGUREPROPERTY_H
