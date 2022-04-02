#include "Parse.h"
#include <QCoreApplication>

bool GetGfwCode(const QString& filePath, QString& res)
{
    res.clear();

    if (!QFile::exists(filePath))
    {
        return false;
    }

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly))
    {
        return false;
    }

    QByteArray code = file.readAll();
    file.close();

    if (code.size() < 10)
    {
        return false;
    }

    QString cGfw = QString::fromUtf8(code);
    cGfw.remove("\r");
    cGfw.remove("\n");
    cGfw.remove(" ");
    cGfw.remove("\b");

    code = QByteArray::fromBase64(cGfw.toUtf8());
    res = QString::fromUtf8(code);
    return true;
}

QStringList GetUrlList(const QString& inList)
{
    QStringList cc = inList.split("\n", QString::SkipEmptyParts);
    for (int i = 0; i < cc.size(); ++i)
    {
        cc[i].remove("\r");
        cc[i].remove("\n");
    }

    return cc;
}

void FilterMulti(QStringList& urls, QString& url)
{
    if (url.size() <= 4)
    {
        return;
    }
    for (QStringList::iterator i = urls.begin(); i != urls.end(); ++i)
    {
        QString cUrl = *i;

        if (url.contains(cUrl))
        {
            url.clear();
            break;
        }

        if (cUrl.lastIndexOf(url) != -1)
        {
            if (url.size() < cUrl.size())
            {
                urls.erase(i);
                urls.append(url);
                break;
            }
        }
    }

    if (url.size() > 4)
    {
        urls.append(url);
    }
}

void GetClearUrl(QString& string)
{
    if (!string.contains("."))
    {
        string.clear();
        return;
    }

    while ((string.startsWith(".") || string.startsWith("|")) && string.size())
    {
        string = string.remove(0, 1);
    }

    if (string.startsWith("http://"))
    {
        string = string.remove(0, 7);
    }
    else if (string.startsWith("https://"))
    {
        string = string.remove(0, 8);
    }

    string = string.split("/").first();
}

int ClearUrl(QString& string, QStringList& whileMulti, QStringList& blackMulti)//0 white 1 black
{
    int type = -1;
    if (string.startsWith(".") || string.startsWith("|"))
    {
        GetClearUrl(string);
        FilterMulti(whileMulti, string);
        if (string.size() > 4)
        {
            type = 0;
        }

        return type;
    }

    if (string.startsWith("@"))
    {
        while (string.startsWith("@") && string.size())
        {
            string = string.remove(0, 1);
        }

        GetClearUrl(string);
        FilterMulti(blackMulti, string);
        if (string.size() > 4)
        {
            type = 1;
        }
        return type;
    }

    return type;
}

QJsonDocument ToRule(const QStringList& inList)
{
    QJsonDocument doc;
    QJsonObject obj;

    QStringList whileMulti;
    QStringList blackMulti;

    QJsonArray whitePatterns;
    QJsonArray blackPatterns;

    for (int i = 0; i < inList.size(); ++i)
    {
        QString url = inList[i];

        int type = ClearUrl(url, whileMulti, blackMulti);
        Q_UNUSED(type);
    }

    for (int i = 0; i < whileMulti.size(); ++i)
    {
        QString url = whileMulti[i];
        QJsonObject oo;
        oo.insert("title", url);
        oo.insert("pattern", QString("*%1*").arg(url));
        oo.insert("type", 1);
        oo.insert("protocols", 1);
        oo.insert("active", true);
        whitePatterns.append(oo);

    }

    for (int i = 0; i < blackMulti.size(); ++i)
    {
        QString url = blackMulti[i];
        QJsonObject oo;
        oo.insert("title", url);
        oo.insert("pattern", QString("*%1*").arg(url));
        oo.insert("type", 1);
        oo.insert("protocols", 1);
        oo.insert("active", true);
        blackPatterns.append(oo);
    }

    obj.insert("whitePatterns", whitePatterns);
    obj.insert("blackPatterns", blackPatterns);

    doc.setObject(obj);
    return doc;
}

bool SaveFileToJson(const QJsonDocument& doc, QString& filePath)
{
    filePath = filePath.isEmpty() ? QString("%1/%2.json").arg(qApp->applicationDirPath(), QDateTime::currentDateTime().toString("yyyyMMddhhmmss")) : filePath;
    if (QFile::exists(filePath))
    {
        if (!QFile::remove(filePath))
        {
            return false;
        }
    }

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly))
    {
        return false;
    }

    QByteArray docJson = doc.toJson(QJsonDocument::Indented);
    if (file.write(docJson) != docJson.size())
    {
        file.close();
        return false;
    }

    file.flush();
    file.close();
    return true;
}

bool Convert(const QString& gfwPath, QString& errorCode)
{
    QString urlContent;
    if (!GetGfwCode(gfwPath, urlContent))
    {
        errorCode = QString("Cannot Parse File \"%1\"").arg(gfwPath);
        return false;
    }

    QFile filetemp(qApp->applicationDirPath() + "/decordGFW.abp");
    filetemp.open(QFile::WriteOnly);
    filetemp.write(urlContent.toUtf8());
    filetemp.flush();
    filetemp.close();

    QFile addones(":/RES/add_ones.txt");
    addones.open(QFile::ReadOnly);

    urlContent += "\n";
    urlContent += addones.readAll();
    addones.close();

    QStringList urls = GetUrlList(urlContent);
    QJsonDocument jsonDoc = ToRule(urls);

    QString saveUrl;
    if (!SaveFileToJson(jsonDoc, saveUrl))
    {
        errorCode = QString("Cannot Save File \"%1\"").arg(saveUrl);
        return false;
    }

    errorCode = QString("Success \"%1\"").arg(saveUrl);
#ifdef Q_OS_WINDOWS
    QStringList args;
    saveUrl.replace("/", "\\");
    args.append(QString("/select,\"%1\"").arg(saveUrl));

    QString fullExe = QString("explorer %1").arg(QString("/select,\"%1\"").arg(saveUrl));
    QProcess::execute(fullExe);
    //qDebug() << fullExe.toStdString().c_str();

#endif

    return true;

}
