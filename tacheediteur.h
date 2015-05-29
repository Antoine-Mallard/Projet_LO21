#ifndef TACHEEDITEUR_H
#define TACHEEDITEUR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include "Calendar.h"

class TacheEditeur : public QWidget
{
    Q_OBJECT
    Tache & t;
    QVBoxLayout *couche;
    QHBoxLayout *couche1,
                *couche2,
                *couche3,
                *couche4;
    QLabel  *identificateur,
            *titre,
            *dispo,
            *echeance,
            *duree;
    QPushButton *annuler,
                *sauver;
    QLineEdit *lineIdentificateur;
    QTextEdit *textTitre;
    QDateEdit   *dateDispo,
                *dateEcheance;
    QSpinBox    *heures,
                *minutes;
    QCheckBox *preemptive;
public:
    explicit TacheEditeur(Tache& ta, QWidget *parent = 0);
    ~TacheEditeur();

signals:

public slots:
    void verifierID();
    void activerBouton();
};

#endif // TACHEEDITEUR_H
