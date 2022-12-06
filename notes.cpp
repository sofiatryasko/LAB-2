#include "notes.h"
#include "ui_notes.h"
#include "popUpMenu.h"
#include "additional.h"
#include "load.h"
#include "save.h"
#include "menu.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QAction>
#include <QVector>
#include <QDir>


notes::notes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::notes)
{
    ui->setupUi(this);
    this->setStyleSheet("color: white; background-color: lightpink;");
    this->setWindowTitle("ଘ(੭ˊᵕˋ)੭* ੈ✩‧₊˚notepad");
    ui->saveChanges->hide();
    ui->treeNotes->setContextMenuPolicy(Qt::CustomContextMenu);

    QDir notesDir (QDir::current().absolutePath() + "/notes");
    if (!notesDir.exists()) {
        QDir().mkdir(QDir::current().absolutePath() + "/notes");
        QDir().mkdir(QDir::current().absolutePath() + "/notes"+"/All");
    } else {
        QDir AllDir (QDir::current().absolutePath() + "/notes"+"/All");
        if (!AllDir.exists()) {
            QDir().mkdir(QDir::current().absolutePath() + "/notes"+"/All");
        }
    }

    connect(ui->treeNotes,&QTreeWidget::customContextMenuRequested,this,&notes::ProvideContextMenu);
    connect(ui->actionLoad_all, SIGNAL(triggered()), this, SLOT(on_actLoad_all_clicked()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(on_actExit_clicked()));
    connect(ui->actionShow_by_time, SIGNAL(triggered()), this, SLOT(on_actSort_clicked()));
}

notes::~notes()
{
    delete ui;
}

void notes::on_archive_clicked()
{
    ui->treeNotes->clear();
    ui->noteName->clear();
    ui->textNote->clear();
    if (notes::isArchiveMode) {
       loadAll(QDir::current().absolutePath()+"/notes/", nullptr);
       ui->archive->setText("Show with archived");
       ui->addNew->setEnabled(true);
    } else {
       loadArchived(QDir::current().absolutePath()+"/notes/", nullptr, false);
       ui->archive->setText("Hide archived");
       ui->addNew->setEnabled(false);
    }
    notes::isArchiveMode = !notes::isArchiveMode;

}

void notes::on_saveChanges_clicked()
{
    if (notes::isCategory) {
        saveCategory();
    } else if (notes::isNote) {
        saveNote();
    } else if (notes::isEditText) {
        saveEditText();
    } else if (notes::isEditTitle) {
        saveEditTitle();
    }

    ui->textNote->clear();
    ui->noteName->clear();
    ui->noteName->setEnabled(false);
    ui->textNote->setEnabled(false);
    ui->saveChanges->hide();
}

void notes::on_addNew_clicked()
{
    ui->textNote->clear();
    ui->noteName->clear();
    ui->noteName->setEnabled(true);
    ui->textNote->setEnabled(false);
    ui->noteName->setReadOnly(false);
    ui->textNote->setReadOnly(false);
    notes::isCategory = true;
    currObject(nullptr,false);
    ui->saveChanges->show();
}

void notes::on_treeNotes_itemClicked(QTreeWidgetItem *item, int column)
{
    loadNote(item, column);
}

