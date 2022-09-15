#ifndef INICONFIG_P_H
#define INICONFIG_P_H

#include <QSettings>

class IniConfigPrivate
{
public:
    QString strFilePath = "";
    QString strSectionName = "";

    template<class T>
    void setValue(const QString &key, const T &value)
    {
        if(!settings)
        {
            return;
        }

        settings->setValue(strSectionName + key, value);
    }


    template<class T>
    T getValue(const QString &key, const T &defValue)
    {
        if(!settings)
        {
            return defValue;
        }

        auto var = settings->value(strSectionName + key);

        if(var.canConvert<T>())
        {
            return var.value<T>();
        }
        else
        {
            setValue(key, defValue);
            return defValue;
        }
    }

    void newSetting(const QString& strFileName)
    {
        settings = new QSettings(strFileName, QSettings::IniFormat);
    }

    void flushSetting()
    {
        if(settings)
        {
            settings->sync();
        }
    }

    void removeSection(const QString &sectionName)
    {
        if(!settings)
        {
            return;
        }
        settings->remove(sectionName);
    }

    bool containsGroup(const QString& sectionName)
    {
        if(!settings)
        {
            return false;
        }
        return settings->childGroups().contains(sectionName);
    }

private:
    QSettings* settings = nullptr;

};


#endif // INICONFIG_P_H
