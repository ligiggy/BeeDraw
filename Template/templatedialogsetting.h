#ifndef TEMPLATEDIALOGSETTING_H
#define TEMPLATEDIALOGSETTING_H

#include "iniconfig.h"
#include "qstring.h"
#include "qrect.h"

class TemplateDialogSettingPrivate;
class TemplateDialogSetting
    : public IniConfig
{
public:
    TemplateDialogSetting();

public:
    void setFilePath(const QString& filePath);

public:
    void read();
    void write();


public:
    QString getTemplateFilePath();
    void setTemplateFilePath(const QString &filePath);

private:
    Q_DECLARE_PRIVATE(TemplateDialogSetting)
};

#endif // TEMPLATEDIALOGSETTING_H
