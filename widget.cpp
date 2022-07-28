#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    nomApplication = "Writext";
    statChar = "";
    current_file.setFileName("Nouveau fichier");

    setWindowTitle(current_file.fileName() + statChar + " - " + nomApplication);
    setWindowIcon(QIcon("iconeEditeur.png"));

    zone_de_texte = new QGroupBox("Zone de texte");
    operations = new QGroupBox("Opérations");
    autres_actions = new QGroupBox("Autres actions");

    mainLayout = new QVBoxLayout(this);
    text_layout = new QVBoxLayout;
    operation_layout = new QVBoxLayout;
    autres_actions_layout = new QVBoxLayout;

    textEdit = new QTextEdit;
    btn_ouvrir = new QPushButton("Ouvrir");
    btn_enregistrer = new QPushButton("Enregistrer");
    btn_enregistrer_sous = new QPushButton("Enregistrer sous");
    btn_apropos = new QPushButton("A propos");
    btn_quitter = new QPushButton("Quitter");

    text_layout->addWidget(textEdit);
    zone_de_texte->setLayout(text_layout);

    operation_layout->addWidget(btn_ouvrir);
    operation_layout->addWidget(btn_enregistrer);
    operation_layout->addWidget(btn_enregistrer_sous);
    operations->setLayout(operation_layout);

    autres_actions_layout->addWidget(btn_apropos);
    autres_actions_layout->addWidget(btn_quitter);
    autres_actions->setLayout(autres_actions_layout);

    mainLayout->addWidget(zone_de_texte);
    mainLayout->addWidget(operations);
    mainLayout->addWidget(autres_actions);

    setLayout(mainLayout);

    connect(textEdit, SIGNAL(textChanged()), this, SLOT(updateStatTitle()));
    connect(btn_ouvrir, SIGNAL(clicked(bool)), this, SLOT(showDialoguSelectFile()));
    connect(btn_enregistrer, SIGNAL(clicked(bool)), this, SLOT(showDialoguSave()));
    connect(btn_enregistrer_sous, SIGNAL(clicked(bool)), this, SLOT(showDialoguSaveAs()));
    connect(btn_apropos, SIGNAL(clicked(bool)), this, SLOT(showDialoguBoxApropos()));
    connect(btn_quitter, SIGNAL(clicked(bool)), this, SLOT(verifierDocumentSaved()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::showDialoguBoxApropos()
{
    QMessageBox::about(0, "A propos", "Logiciel créer le <b>28 Janvier 2022</b><br><hr><br>"
                             "<b>Autheur :</b> ZOCLI Patrick<br>"
                             "<b>Version :</b> 1.0<br>"
                                            "<b>Copyright, je ne sais pas quoi mettre, 2021 peut être ;D</b>");
}

void Widget::showDialoguSelectFile()
{
    current_file.setFileName(QFileDialog::getOpenFileName(this, tr("Ouvrir Fichier"), "/home/patrick", tr("Fichier texte (*.txt)")));
    if (current_file.fileName() != "")
    {
        insertFileContain();
    }
    else
        current_file.setFileName("Nouveau fichier");
}

void Widget::showDialoguSaveAs()
{
    QString newFileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder comme"), current_file.fileName(), tr("Fichier texte (*.txt)"));
    if (newFileName != "")
    {
        current_file.setFileName((newFileName.size() > 4 && newFileName.mid(newFileName.size() - 4, 4) == ".txt") ? newFileName : newFileName + QString(".txt"));
        if (!current_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            QMessageBox::warning(0, "Erreur d'ouverture du fichier", "Le fichier n'a pas pu créé");
        else
        {
            QTextStream out(&current_file);
            contenu_fichier = textEdit->toPlainText();
            out << contenu_fichier;
            out.flush();
            current_file.close();
            updateStatTitle();
        }
    }
}

void Widget::showDialoguSave()
{
    if (current_file.fileName() == "Nouveau fichier")
    {
        current_file.setFileName("/home/patrick/\"Nouveau fichier\"");
        showDialoguSaveAs();
    }
    else
    {
        if (!current_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            QMessageBox::warning(0, "Erreur d'ouverture du fichier", "Le fichier n'a pas pu être sauvegarder");
        else
        {
            QTextStream out(&current_file);
            contenu_fichier = textEdit->toPlainText();
            out << contenu_fichier;
            out.flush();
            current_file.close();
            updateStatTitle();
        }
    }
}

void Widget::updateStatTitle()
{
    statChar = QString("*");
    if (textEdit->toPlainText() == contenu_fichier)
        statChar = "";
    setWindowTitle(extractFileName(current_file.fileName()) + statChar + " - " + nomApplication);
}

void Widget::verifierDocumentSaved()
{
    if (contenu_fichier != textEdit->toPlainText())
    {
        int answer = QMessageBox::question(0, "Writext", "Voulez vous enregistrer votre document avant de quitter ?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (answer == QMessageBox::Yes)
        {
            showDialoguSave();
            if (contenu_fichier != textEdit->toPlainText())
            {
                answer = QMessageBox::question(0, "Writext", "Le document n'a toujours pas été sauvergarder. Voulez vous quitter ?", QMessageBox::Yes, QMessageBox::No);
                if (answer == QMessageBox::Yes)
                    this->close();
            }
            else
                this->close();
        }
        else if (answer == QMessageBox::No)
            this->close();
    }
    else
        this->close();
}

void Widget::insertFileContain()
{
    if (!current_file.open(QIODevice::ReadOnly | QIODevice::Text))
              QMessageBox::warning(0, "Erreur d'ouverture du fichier", "Le fichier n'a pas pu être ouvert");
    else
    {
        QTextStream in(&current_file);
        contenu_fichier = QString(in.readAll().constData());
        textEdit->setPlainText(contenu_fichier);
        current_file.close();
    }
}

QString Widget::extractFileName(QString link)
{
    if (link.isEmpty())
        return link;
    link.remove(QChar('\\'));
    int index = link.lastIndexOf('/');
    return link.mid(index == -1 ? 0 : index + 1);
}
