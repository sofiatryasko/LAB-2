#ifndef ADDITIONAL_H
#define ADDITIONAL_H
#include <QString>
#include <QFile>
#include <QTreeWidgetItem>
#include <QVector>
#include <QDir>
bool isNotice (QTreeWidgetItem* object);
QString findRoute (QTreeWidgetItem* object);
QString findRoute (QTreeWidgetItem* object);
QTreeWidgetItem* currObject (QTreeWidgetItem *object, bool getObject);
bool isStringAllowed(QString name);

bool isStringAllowed(QString name)
{
    QString changedName = name;
    QString list[] = {".", "/", "\\", "?", "*", ":", "|", "\"", "\'", "<", ">"};
    for (QString element: list)
        name.replace(element, "");

    return (name == changedName);
}

QTreeWidgetItem* currObject (QTreeWidgetItem *object, bool getObject) {
    static QTreeWidgetItem *current;
    if (!getObject) {current = object;}
    else return current;
}

QString findRoute (QTreeWidgetItem* object) {
    QString path = QDir::current().absolutePath()+"/notes/";
    QTreeWidgetItem* current = object->parent();
    QVector<QString> contexts;

    while (current != nullptr) {
        contexts.push_front(current->text(0)+"/");
        current = current->parent();
    }

    for (auto item_type : contexts) {
        path += item_type;
    }
    return path;
}

bool isNotice (QTreeWidgetItem* object) {
    QString route = findRoute(object);
    return QFile::exists(route+object->text(0)+".txt")||QFile::exists(route+object->text(0)+".archive");
}




#endif // ADDITIONAL_H
