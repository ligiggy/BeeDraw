#include "templatedialogsetting.h"

#include "qdatastream.h"
#include "qdebug.h"

#include "iniconfig_p.h"



class TemplateDialogSettingPrivate
    : public IniConfigPrivate
{
public:
    const QString strDefaultSectionName = "Template/";

    const QString strTemplateKeyName = "Temp";


public:
    QString m_strTemplateFilePath = "";

};


TemplateDialogSetting::TemplateDialogSetting()
    : IniConfig(* new TemplateDialogSettingPrivate)
{

}

void TemplateDialogSetting::setFilePath(const QString &filePath)
{
    Q_D(TemplateDialogSetting);

    d->strFilePath = filePath;
    d->newSetting(filePath);
    d->strSectionName = d->strDefaultSectionName;

    read();
}

void TemplateDialogSetting::read()
{
    Q_D(TemplateDialogSetting);

    QByteArray array = d->getValue(d->strTemplateKeyName, QByteArray());

    if(array.isEmpty())
    {
        return;
    }

    QDataStream in(array);
    in >> d->m_strTemplateFilePath;

}

void TemplateDialogSetting::write()
{
    Q_D(TemplateDialogSetting);

    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);

    out << d->m_strTemplateFilePath;

    d->setValue(d->strTemplateKeyName, array);
}

QString TemplateDialogSetting::getTemplateFilePath()
{
    Q_D(TemplateDialogSetting);
    return d->m_strTemplateFilePath;
}

void TemplateDialogSetting::setTemplateFilePath(const QString &filePath)
{
    Q_D(TemplateDialogSetting);
    d->m_strTemplateFilePath = filePath;
}
