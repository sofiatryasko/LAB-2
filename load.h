#ifndef LOAD_H
#define LOAD_H

#include "notes.h"
#include "additional.h"
#include "ui_notes.h"
#include <QDir>
#include <QTreeWidgetItem>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QMessageBox>
#include <QIODevice>
#include <QTextStream>

void loadAll (QDir, QTreeWidgetItem*, bool);
void loadArchived(QDir, QTreeWidgetItem*, bool, bool);
void loadNote(QTreeWidgetItem*, int);

void notes::loadNote(QTreeWidgetItem *object, int column) {
    ui->textNote->clear();
    ui->noteName->clear();

    if (!isNotice(object)) {
        object->setSelected(false);
        return;
    }

    ui->noteName->setEnabled(true);
    ui->textNote->setEnabled(true);
    ui->noteName->setReadOnly(true);
    ui->textNote->setReadOnly(true);
    QString title = object->text(column);

    QString route = findRoute(object);
    QFileInfo check_file(route+title+".txt");

    QFile note;
   if (check_file.exists()) {
     note.setFileName(route+title+".txt");
   } else {
      note.setFileName(route+title+".archive");
   }

    ui->noteName->setText(title);

    if (!note.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0,"error", "this note cannot be open!");
    } else {
        QTextStream in(&note);
        while (!in.atEnd()) {
            ui->textNote->append(in.readLine());
        }
    }
}


void notes::loadAll (QDir route, QTreeWidgetItem* parent = nullptr, bool sortByTime = false) {
    if (QFile::exists(route.absolutePath()+"/.ignore")) return;

    QString pathStr = route.absolutePath()+"/";
    pathStr.replace(QDir::current().absolutePath()+"/notes/", "");
    QString currentFolder = "";

    while(!pathStr.isEmpty()) {
        for (int pos=0; pathStr[pos]!='/'; pos++){
            currentFolder+= pathStr[pos];
        }
        pathStr.replace(currentFolder+"/", "");
        if (!pathStr.isEmpty()) {
            currentFolder.clear();
        }
    }

    QTreeWidgetItem *newObject = nullptr;
    if (!currentFolder.isEmpty()) {
        if (parent == nullptr) {
            newObject = new QTreeWidgetItem(ui->treeNotes);
            ui->treeNotes->addTopLevelItem(newObject);
            newObject->setText(0,currentFolder);
            newObject->setTextColor(0, "white");
        } else {
            newObject = new QTreeWidgetItem(parent);
            newObject->setText(0,currentFolder);
            newObject->setTextColor(0, "white");
        }
    }

    QStringList subdirs = route.entryList();
    if (sortByTime) {
        sortSubList(subdirs,route);
    }
    foreach (QString curr, subdirs) {

        QString nextPath = route.absolutePath();
        nextPath += "/"+curr;
        QFileInfo check_file(nextPath);

       if (check_file.exists() && curr!="." && curr!="..") {
           if (check_file.isFile()) {
               QString checkArc = curr;
               if (curr.replace(".archive", "") == checkArc) {
                   QTreeWidgetItem *childItem = new QTreeWidgetItem(newObject);
                   childItem->setText(0,curr.replace(".txt", ""));
               }
           } else {
               loadAll(nextPath, newObject, sortByTime);
           }
       }
    }
}

void notes::loadArchived(QDir route, QTreeWidgetItem* parent, bool archivedFolder = false, bool sortByTime = false) {
    if (QFile::exists(route.absolutePath()+"/.ignore")) {
        archivedFolder = true;
    }

    QString pathStr = route.absolutePath()+"/";
    pathStr.replace(QDir::current().absolutePath()+"/notes/", "");
    QString currentFolder = "";

    while(!pathStr.isEmpty()) {
        for (int pos=0; pathStr[pos]!='/'; pos++){
            currentFolder+= pathStr[pos];
        }
        pathStr.replace(currentFolder+"/", "");
        if (!pathStr.isEmpty()) {
            currentFolder.clear();
        }
    }

    QTreeWidgetItem *newItem = nullptr;
    QColor typeColor = "white";
    if (archivedFolder) {
        typeColor.setRgb(180,116,104);
    }
    if (!currentFolder.isEmpty()) {
        if (parent == nullptr) {
            newItem = new QTreeWidgetItem(ui->treeNotes);
            ui->treeNotes->addTopLevelItem(newItem);
        } else {
            newItem = new QTreeWidgetItem(parent);
        }
    }

    QStringList subdirs = route.entryList();
    if (sortByTime) {sortSubList(subdirs,route);}
    foreach (QString curr, subdirs) {
        QString nextPath = route.absolutePath();
        nextPath += "/"+curr;
        QFileInfo check_file(nextPath);

       if (check_file.exists() && curr!="." && curr!="..") {
           if (check_file.isFile()) {
               if (curr == ".ignore") {continue;}
               QString checkArc = curr;
               QTreeWidgetItem *childItem = new QTreeWidgetItem(newItem);
               if (curr.replace(".archive", "") == checkArc) {
                   childItem->setText(0,curr.replace(".txt", ""));
                   if (archivedFolder) {
                       childItem->setTextColor(0,"darkgrey");
                   }
               } else {
                   childItem->setText(0,curr);
                   childItem->setTextColor(0,"darkgrey");
               }
           } else {
               loadArchived(nextPath, newItem, archivedFolder);
           }
       }
    }
}


#endif // LOAD_H
