#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void showDialoguBoxApropos();
    void showDialoguSelectFile();
    void showDialoguSaveAs();
    void showDialoguSave();
    void updateStatTitle();
    void verifierDocumentSaved();

private:
    Ui::Widget *ui;
    QVBoxLayout *mainLayout, *text_layout, *operation_layout, *autres_actions_layout;
    QGroupBox *zone_de_texte, *operations, *autres_actions;
    QTextEdit *textEdit;
    QPushButton *btn_ouvrir, *btn_enregistrer, *btn_enregistrer_sous, *btn_apropos, *btn_quitter;

    QFile current_file;
    QString nomApplication;
    QString contenu_fichier;
    QString statChar;

    void insertFileContain();
    QString extractFileName(QString link);
};
#endif // WIDGET_H
