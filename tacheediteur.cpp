#include "tacheediteur.h"


TacheEditeur::TacheEditeur(Tache &ta, QWidget *parent) : t(ta), QWidget(parent)
{
    couche = new QVBoxLayout(this);
    couche1 = new QHBoxLayout;
    couche2 = new QHBoxLayout;
    couche3 = new QHBoxLayout;
    couche4 = new QHBoxLayout;
    //labels
    identificateur = new QLabel("identificateur");
    titre = new QLabel("titre");
    dispo = new QLabel("disponibilité");
    echeance = new QLabel("écheance");
    duree = new QLabel("durée");
    //boutons
    annuler = new QPushButton("Annuler");
    sauver = new QPushButton("Sauver");
    //lineEdit
    lineIdentificateur = new QLineEdit;
    //textEdit
    textTitre = new QTextEdit;
    //checkbox
    preemptive = new QCheckBox("preemptive");
    //dateEdit
    dateDispo = new QDateEdit;
    dateEcheance = new QDateEdit;
    //spinBox
    heures = new QSpinBox;
    minutes = new QSpinBox;
    QString("heure(s)");

    //vertical Layout
    couche->addLayout(couche1);
    couche->addLayout(couche2);
    couche->addLayout(couche3);
    couche->addLayout(couche4);
    //horizontal Layout
    couche1->addWidget(identificateur);
    couche1->addWidget(lineIdentificateur);
    couche1->addWidget(preemptive);

    couche2->addWidget(titre);
    couche2->addWidget(textTitre);

    couche3->addWidget(dispo);
    couche3->addWidget(dateDispo);
    couche3->addWidget(echeance);
    couche3->addWidget(dateEcheance);
    couche3->addWidget(duree);
    couche3->addWidget(heures);
    couche3->addWidget(minutes);

    couche4->addWidget(annuler);
    couche4->addWidget(sauver);

    //modifications
    lineIdentificateur->setText(t.getId());
    preemptive->setChecked(t.isPreemptive());
    textTitre->setText(t.getTitre());
    dateDispo->setDate(t.getDateDisponibilite());
    dateEcheance->setDate(t.getDateEcheance());
    heures->setValue(t.getDuree().getHeure());
    minutes->setValue(t.getDuree().getMinute());
    //evenements
    sauver->setEnabled(false);
    QObject::connect(lineIdentificateur, SIGNAL(textChanged(QString)), this, SLOT(verifierID()));
    QObject::connect(sauver, SIGNAL(clicked()), this, SLOT(verifierID()));
    QObject::connect(annuler, SIGNAL(clicked()), this, SLOT(close()));
    //ya des trucs à changer
    QObject::connect(lineIdentificateur, SIGNAL(textChanged(QString)), this, SLOT(activerBouton()));
    QObject::connect(preemptive, SIGNAL(clicked()), this, SLOT(activerBouton()));
    QObject::connect(textTitre, SIGNAL(textChanged(QString)), this, SLOT(activerBouton()));
    QObject::connect(dateDispo, SIGNAL(valueChanged(QString)), this, SLOT(activerBouton()));
    QObject::connect(dateEcheance, SIGNAL(valueChanged(QString)), this, SLOT(activerBouton()));
    QObject::connect(heures, SIGNAL(valueChanged(QString)), this, SLOT(activerBouton()));
    QObject::connect(minutes, SIGNAL(valueChanged(QString)), this, SLOT(activerBouton()));
}

TacheEditeur::~TacheEditeur()
{

}

void TacheEditeur::verifierID(){
    if(lineIdentificateur->text() != t.getId() && (TacheManager::getInstance().isTacheExistante(lineIdentificateur->text()))){
        QMessageBox::warning(this,"Attention !", "ID déjà pris !");
        return;
    }
    if(t.getId() != lineIdentificateur->text())
        t.setId(lineIdentificateur->text());
    if(preemptive->isChecked())
        t.setPreemptive();
    else
        t.setNonPreemptive();
    t.setTitre(textTitre->toPlainText());
    t.setDuree(Duree(heures->value(), minutes->value()));
    t.setDatesDisponibiliteEcheance(dateDispo->date(), dateEcheance->date());
}

void TacheEditeur::activerBouton(){
    sauver->setEnabled(true);
}
