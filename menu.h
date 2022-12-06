#ifndef MENU_H
#define MENU_H

#include "notes.h"
#include "ui_notes.h"
#include "additional.h"
#include <QFile>
#include <QFileDevice>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QApplication>
#include <QDir>
#include <algorithm>

void notes::on_actExit_clicked() {
    QApplication::quit();
}

void notes::on_actLoad_all_clicked() {
    ui->treeNotes->clear();
    loadAll(QDir::current().absolutePath()+"/notes/", nullptr, false);
}

void notes::sortSubList (QStringList subdirs, QDir path) {
    std::sort(subdirs.begin(), subdirs.end(), [&path](const QString& left, const QString right)
    {
        QFile leftFile (path.absolutePath()+"/"+left);
        QFile rightFile (path.absolutePath()+"/"+right);
        return (leftFile.fileTime(QFileDevice::FileBirthTime)<rightFile.fileTime(QFileDevice::FileBirthTime));
    });
}

void notes::on_actSort_clicked() {
    ui->treeNotes->clear();
    ui->noteName->clear();
    ui->textNote->clear();

    if (notes::isArchiveMode) {
        loadArchived(QDir::current().absolutePath()+"/notes/", nullptr, false, true);

    } else {
        loadAll(QDir::current().absolutePath()+"/notes/", nullptr, true);
    }
}

#endif // MENU_H
