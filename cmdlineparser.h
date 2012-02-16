#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include <QVariant>
#include <QStringList>

class CmdLineParser
{
public:
    CmdLineParser(const QStringList &arguments);

    // cherche "--key=*", s'il n'existe pas retourne defaultValue
    // si defaultValue est une nombre et que la valeur n'est pas convertible, retourne defaultValue
    QVariant get(const QByteArray &key, const QVariant &defaultValue);

    // cherche dans les agruments : "--key" ou "--key=*", s'il trouve retourne true.
    bool isSet(const QByteArray &key);

private:
    QStringList _args;
};

#endif // CMDLINEPARSER_H
