#ifndef SAVE_H
#define SAVE_H

#include "notes.h"
#include "ui_notes.h"
#include "additional.h"
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QMessageBox>
#include <QTreeWidgetItem>


void saveNote();
void saveEditText();
void saveEditTitle();
void saveCategory();


void notes::saveNote() {
    QTreeWidgetItem* parent = currObject(nullptr,true);
    QString title = ui->noteName->text();
   if (title.isEmpty()) {
       QMessageBox::warning(0, "warning", "please, fill the title");
   }
   else {
       if (isStringAllowed(title)) {
           QTreeWidgetItem *Object = new QTreeWidgetItem(parent);
           QString route = findRoute(Object);
           Object->setText(0,title);

           QFile note(route+title+".txt");
           if (!note.open(QIODevice::WriteOnly | QIODevice::Text)) {
               QMessageBox::critical(0,"critical", "this note cannot be opened");
           } else {
           QTextStream out(&note);
           out << "\n ";
           out << ui->textNote->toPlainText();
           note.close();
           notes::isNote = false;
           }
       } else {
           QMessageBox::warning(0, "warning", "please, don't use /, \\, ?, *, :, |, \", \', <, >, . in the title");
       }
   }
}

void notes::saveEditText() {
    QString title = ui->noteName->text();
    QString route = findRoute(currObject(nullptr,true));

    QFile note (route+title+".txt");
    if (!note.open(QIODevice::Append | QIODevice::Text)) QMessageBox::critical(0,"critical", "this note cannot be opened");
    else {
        QTextStream out(&note);
        out<< ui->textNote->toPlainText();
        note.close();
    }

   notes::isEditText = false;
}

void notes::saveEditTitle() {
   QTreeWidgetItem* Object = currObject(nullptr,true);
   QString title = ui->noteName->text();
   if (title.isEmpty()) QMessageBox::warning(0,"warning", "please, fill the title!");
    else {
       if (isStringAllowed(title)) {
           QString route = findRoute(Object);
           if (isNotice(Object))
                QFile::rename(route+Object->text(0)+".txt", route+title+".txt");
            else {
               QDir prevTitle = QDir::current().absolutePath()+"/notes/"+Object->text(0);
               QString nweTitle = QDir::current().absolutePath()+"/notes/"+title;
               prevTitle.rename(QDir::current().absolutePath()+"/notes/"+Object->text(0), nweTitle);
           }
           Object->setText(0, title);
           notes::isEditTitle = false;
       }
       else QMessageBox::warning(0, "warning", "please, don't use /, \\, ?, *, :, |, \", \', <, >, . in the title");

   }
}

void notes::saveCategory() {
    QString title = ui->noteName->text();
    QTreeWidgetItem* parent = currObject(nullptr,true);

   if (title.isEmpty()) {
        QMessageBox::warning(0,"error", "please, fill the title!");
   } else {
        if (isStringAllowed(title)) {
           if (parent == nullptr) {
               QTreeWidgetItem *topLevelObject = new QTreeWidgetItem(ui->treeNotes);
               ui->treeNotes->addTopLevelItem(topLevelObject);
               topLevelObject->setText(0,title);
               topLevelObject->setTextColor(0, "white");
               QDir().mkdir(QDir::current().absolutePath()+"/notes/"+title);
           } else {
               QTreeWidgetItem *Object = new QTreeWidgetItem(parent);

               QString route = findRoute(Object);
               Object->setText(0,title);
               Object->setTextColor(0, "white");

               QDir().mkdir(route+title);
                notes::isCategory = false;
           }
        } else {
            QMessageBox::warning(0, "warning", "please, don't use /, \\, ?, *, :, |, \", \', <, >, . in the title");
        }
   }
}


#endif // SAVE_H
