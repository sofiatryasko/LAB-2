#ifndef POPUPMENU_H
#define POPUPMENU_H
#include "notes.h"
#include "ui_notes.h"
#include "additional.h"

#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QIcon>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTreeWidgetItem>
#include <QString>
#include <QStringList>


void on_actNewType_clicked();
void on_actNewNote_clicked();
void deleteObject(QTreeWidgetItem* object);
void on_actEditText_clicked();
void on_actDelete_clicked();
void on_actEditName_clicked();
void on_actMove_clicked();
void archiveSubFiles (QString route);
void restoreSubFiles (QString route);
void on_actRestore_clicked();
void changeRestoredColors (QTreeWidgetItem* parent);
void ProvideContextMenu(const QPoint &positionition);


void notes::on_actNewNote_clicked() {
    ui->noteName->clear();
    ui->textNote->clear();
    ui->noteName->setEnabled(true);
    ui->textNote->setEnabled(true);
    ui->noteName->setReadOnly(false);
    ui->textNote->setReadOnly(false);
    notes::isNote = true;
    ui->saveChanges->show();
}

void notes::on_actNewType_clicked() {
    ui->noteName->clear();
    ui->textNote->clear();
    ui->noteName->setEnabled(true);
    ui->textNote->setEnabled(false);
    ui->noteName->setReadOnly(false);
    ui->textNote->setReadOnly(true);
    notes::isCategory = true;
    ui->saveChanges->show();
}

void deleteObject(QTreeWidgetItem* object) {
    if (!isNotice(object)) {
        QDir dir(QDir::current().absolutePath()+"/notes/"+"All/"+object->text(0));
        dir.removeRecursively();
    } else {
        QFile::remove(QDir::current().absolutePath()+"/notes/"+"All/"+object->text(0)+".txt");
        QFile::remove(QDir::current().absolutePath()+"/notes/"+"All/"+object->text(0)+".archive");
    }
    delete object;
}

void notes::on_actDelete_clicked() {
    QTreeWidgetItem *object = currObject(nullptr, true);

    ui->textNote->clear();
    ui->noteName->clear();

    if (object->parent() == nullptr && object->text(0)=="All") {
        for (int i = 0; i < object->childCount(); i++)
            deleteObject(object->child(0));
        if (object->childCount()!=0) {deleteObject(object->child(0));}

    } else {

        QString route = findRoute(object);

        if (!isNotice(object)) {
            QDir dir(route+object->text(0));
            dir.removeRecursively();
        }
        else {
            QFile::remove(route+object->text(0)+".txt");
            QFile::remove(route+object->text(0)+".archive");
        }

        delete object;
    }
}

void notes::on_actEditText_clicked() {
    QTreeWidgetItem *object = currObject(nullptr, true);
    notes::loadNote(object,0);

    ui->textNote->setReadOnly(false);
    ui->textNote->setEnabled(true);
    notes::isEditText = true;
    ui->saveChanges->show();
}

void notes::on_actEditName_clicked() {
    QTreeWidgetItem *object = currObject(nullptr, true);
    notes::loadNote(object,0);

    ui->noteName->setReadOnly(false);
    ui->noteName->setEnabled(true);

    ui->textNote->setEnabled(false);

    notes::isEditTitle = true;
    ui->saveChanges->show();

    ui->textNote->setEnabled(false);
}


void notes::on_actMove_clicked() {

    QTreeWidgetItem* object = currObject(nullptr, true);
    QString route = findRoute(object);

    if (isNotice(object))
        QFile::rename(route+object->text(0)+".txt", route+object->text(0)+".archive");
    else {
        QFile ignoreNote (route+object->text(0)+"/.ignore");
        if (!ignoreNote.open(QIODevice::Append | QIODevice::Text)) {
            QMessageBox::critical(0,"critical", "ignore file cannot be created!");
        }
        archiveSubFiles(route+object->text(0));
    }

    delete object;
}

void archiveSubFiles (QString route) {

    QDir dir = route;
    QStringList subdirs = dir.entryList();
    foreach (QString current, subdirs) {
        QString nextRoute = dir.absolutePath();
        QString currArchived = current;
        currArchived.replace(".txt", ".archive");
       route += "/"+current;
        QFileInfo check_file(nextRoute);

       if (check_file.exists() && current!="." && current!="..") {
            if (check_file.isFile())
                QFile::rename(nextRoute, dir.absolutePath()+"/"+currArchived);
            else archiveSubFiles(nextRoute);
       }
    }
}


void restoreSubFiles (QString route) {
    QDir dir = route;
    QStringList subdirs = dir.entryList();
    foreach (QString current, subdirs) {
        QString nextRoute = dir.absolutePath();
        QString currArchived = current;
        currArchived.replace(".archive", ".txt");
        nextRoute += "/"+current;
        QFileInfo check_file(nextRoute);

       if (check_file.exists() && current!="." && current!="..") {
           if (check_file.isFile()) {
            QFile::rename(nextRoute, dir.absolutePath()+"/"+currArchived);
           } else {
               restoreSubFiles(nextRoute);
           }
       }
    }
}

void notes::changeRestoredColors (QTreeWidgetItem* parent) {
    if (isNotice(parent)) {
        parent->setTextColor(0,"black");
    } else {
        parent->setTextColor(0, "white");
        for (int i = 0; i < parent->childCount(); i++)
            changeRestoredColors(parent->child(i));
    }
}

void notes::on_actRestore_clicked() {

    QTreeWidgetItem* object = currObject(nullptr, true);
    QString route = findRoute(object);

    if (isNotice(object)) {
        QFile::rename(route+object->text(0)+".archive", route+object->text(0)+".txt");
        object->setTextColor(0,"black");
    } else {
        QFile::remove(route+object->text(0)+"/.ignore");
        restoreSubFiles(route+object->text(0));
        changeRestoredColors(object);
    }
}


void notes::ProvideContextMenu(const QPoint &position)
{
    QTreeWidget *tree = ui->treeNotes;
    QTreeWidgetItem *object = tree->itemAt(position);
    if (object == nullptr) {return;}

    qDebug()<<position<<object->text(0);

    currObject(object, false);

    QMenu menu(this);

    if (!notes::isArchiveMode) {
    if (!isNotice(object)) {
        QAction *actNewNote = new QAction(QIcon(":/icons/icons/new.ico"), tr("&♡add new note♡"), this); //ADD IMAGES
        connect(actNewNote, SIGNAL(triggered()), this, SLOT(on_actNewNote_clicked()));
        menu.addAction(actNewNote);

        QAction *actNewType = new QAction(QIcon(":/icons/icons/new.ico"), tr("&♡add new context♡"), this);
        connect(actNewType, SIGNAL(triggered()), this, SLOT(on_actNewType_clicked()));
        menu.addAction(actNewType);
    }

    if (currObject(object,true)->text(0)!="All") {

        if (isNotice(object)) {
            QAction *actEditText = new QAction(QIcon(":/icons/icons/edit.ico"), tr("&♡edit note♡"), this);
            connect(actEditText, SIGNAL(triggered()), this, SLOT(on_actEditText_clicked()));
            menu.addAction(actEditText);
        }
    }

    QAction *actEditTitle = new QAction(QIcon(":/icons/icons/edit.ico"), tr("&♡edit Title♡"), this);
    connect(actEditTitle, SIGNAL(triggered()), this, SLOT(on_actEditTitle_clicked()));
    menu.addAction(actEditTitle);

    QAction *actMove = new QAction(QIcon(":/icons/icons/archive.ico"), tr("&♡move to archive♡"), this);
    connect(actMove, SIGNAL(triggered()), this, SLOT(on_actMove_clicked()));
    menu.addAction(actMove);
    } else {
        QAction *actRestore = new QAction(QIcon(":/icons/icons/archive.ico"), tr("&♡restore♡"), this);
        connect(actRestore, SIGNAL(triggered()), this, SLOT(on_actRestore_clicked()));
        menu.addAction(actRestore);
    }

    QAction *actDelete = new QAction(QIcon(":/icons/icons/delete.ico"), tr("&♡delete♡"), this);
    connect(actDelete, SIGNAL(triggered()), this, SLOT(on_actDelete_clicked()));
    menu.addAction(actDelete);

    menu.exec(tree->mapToGlobal(position));
}

#endif // POPUPMENU_H
