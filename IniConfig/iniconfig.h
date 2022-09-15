#ifndef INICONFIG_H
#define INICONFIG_H

#include <QString>

class IniConfigPrivate;
class IniConfig
{
public:
    IniConfig();
    virtual ~IniConfig();

public:
    virtual void setFilePath(const QString &filePath) = 0;

protected:
    IniConfig(IniConfigPrivate& d);

    IniConfigPrivate* d_ptr;

};

#endif // INICONFIG_H
