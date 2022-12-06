#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QPoint>
#include <QDir>
#include <QString>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class notes; }
QT_END_NAMESPACE

class notes : public QMainWindow
{
    Q_OBJECT

public:
    notes(QWidget *parent = nullptr);
    ~notes();

private:
    Ui::notes *ui;

private slots:

    void saveNote();
    void saveEditText();
    void saveEditTitle();
    void saveCategory();

    void changeRestoredColors (QTreeWidgetItem* parent);
    void sortSubList (QStringList subdirs, QDir route);
    void loadAll (QDir route, QTreeWidgetItem* parent, bool sortByTime);
    void loadArchived(QDir route, QTreeWidgetItem* parent, bool inArchived, bool sortByTime);

    void ProvideContextMenu(const QPoint &pos);
    void on_actNewNote_clicked();
    void on_actNewType_clicked();
    void on_actDelete_clicked();
    void on_actEditText_clicked();
    void on_actEditName_clicked();
    void on_actMove_clicked();
    void on_actRestore_clicked();

    void on_actLoad_all_clicked();
    void on_actExit_clicked();
    void on_actSort_clicked();

    void on_archive_clicked();
    void on_addNew_clicked();
    void on_treeNotes_itemClicked(QTreeWidgetItem *object, int column);
    void on_saveChanges_clicked();



protected:
    void loadNote(QTreeWidgetItem *object, int column);
    void save_catogery();
    bool isCategory = false;
    bool isNote = false;
    bool isEditText = false;
    bool isEditTitle = false;
    bool isArchiveMode = false;


};
#endif // NOTES_H
