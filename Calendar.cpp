#include "Calendar.h"
#include "timing.h"
//Surcharge d'op�rateurs et affichage pour le namespace TIME

std::ostream& operator<<(std::ostream& f, const Duree & d){
    d.afficher(f);
    return f; }

QTextStream& operator>>(QTextStream& flot, Duree& duree){
    unsigned int h,m;
    bool ok=true;
    flot>>h;
    if (flot.status()!=QTextStream::Ok) ok=false;

    if(flot.read(1)=="H") {
        flot>>m;
        if (flot.status()!=QTextStream::Ok) ok=false;
    }
    else {
        ok=false;
    }
    if (ok) duree=Duree(h,m);
    return flot;
}

 void Duree::afficher(QTextStream& f) const {
     f.setPadChar('0');
     f.setFieldWidth(2);
     f<<nb_minutes/60;
     f.setFieldWidth(0);
     f<<"H";
     f.setFieldWidth(2);
     f<<nb_minutes%60;
     f.setFieldWidth(0);
     f.setPadChar(' ');
 }

QTextStream& operator<<(QTextStream& fout, const Tache& t){
	fout<<t.getId()<<"\n";
	fout<<t.getTitre()<<"\n";
	fout<<t.getDuree()<<"\n";
    fout<<t.getDateDisponibilite().toString()<<"\n";
    fout<<t.getDateEcheance().toString()<<"\n";
	return fout;
}

// FONCTIONS TACHE:

Tache& Tache::operator=(const Tache& t){
    identificateur=t.identificateur;
    titre=t.titre;
    disponibilite=t.disponibilite;
}

void Tache::setId(const QString& str){//on v�rifie si la tache n'existe pas d�j�
  if (TacheManager::getInstance().isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id d�j� existante");
  identificateur=str;
  }

TacheUnitaire &TacheUnitaire::operator=(const TacheUnitaire &t)
{
    echeance=t.echeance;
    duree=t.duree;
    preemptive=t.preemptive;
}

TacheComposite::TacheComposite(const TacheComposite &um):
    Tache(um), nb(p.nb),nbMax(p.nbMax), taches(new Tache*[p.nb]){
    for(unsigned int i=0; i<nb; i++) taches[i]=new Tache(*p.taches[i]);
}

TacheComposite& operator=(const TacheComposite& um)
{
    if (this==&p) return *this;//on v�rifie s'il ne s'agit pas du meme objet
    this->~Projet();//on d�truit l'objet existant
    for(unsigned int i=0; i<p.nb; i++) addTache(new Tache(*p.taches[i]));//nb et nbMax sont mis � jour dans addTache
    return *this;
}

void TacheComposite::addTache(Tache* t)
{
    if (nb==nbMax){
        Tache** newtab=new Tache*[nbMax+10];
        for(unsigned int i=0; i<nb; i++) newtab[i]=taches[i];
        nbMax+=10;
        Tache** old=taches;
        taches=newtab;
        delete[] old;
    }
    taches[nb++]=t;
}

Tache* TacheComposite::trouverTache(const QString& id) const
{
    for(unsigned int i=0; i<nb; i++)
        if (id==taches[i]->getId()) return taches[i];
    return 0;
}

TacheComposite::~TacheComposite()
{
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
}

virtual const Date& TacheComposite::getEcheance()const//Borne sup�rieure du tableau A REVOIR
{
    Date echeance=this->begin()->echeance;
    for(IteratorSTL it=this->begin();it!=this->end();++it)
        if(*it.echeance>echeance)echeance=*it.echeance;
}

Tache& TacheComposite::ajouterTache(const QString& id, const QString& t, const Duree& dur, const QDate& dispo){//on ajoute une tache si elle n'existe pas deja
    if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
    Tache* newt=new Tache(id,t,dur,dispo);//si la Tache n'existe pas deja on la cr�e
    addItem(newt);//on l'ajoute au tableau
    return *newt;
}

Tache& TacheComposite::getTache(const QString& id){//retourne la tache si elle existe, une erreur sinon
    Tache* t=trouverTache(id);
    if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
    return *t;
}

const Tache& TacheComposite::getTache(const QString& id)const{//getTache pour les objets constants
    return const_cast<Projet*>(this)->getTache(id);
}

// FONCTIONS PROJET:

Projet::Projet(const string& id, const string& t, const Date& dispo,unsigned int nb, unsigned nbMax):
    identificateur(id), titre(t), disponibilite(dispo), nb(nb),nbMax(nb), taches(new Tache*[nb]){}

Projet::Projet(const Projet &p):
    identificateur(p.identificateur), titre(p.titre), disponibilite(p.disponibilite), nb(p.nb),nbMax(p.nbMax), taches(new Tache*[p.nb]){
    for(unsigned int i=0; i<nb; i++) taches[i]=new Tache(*p.taches[i]);
}

Projet& Projet::operator=(const Projet& p)
{
    if (this==&p) return *this;//on v�rifie s'il ne s'agit pas du meme objet
    this->~Projet();//on d�truit l'objet existant
    for(unsigned int i=0; i<p.nb; i++) addTache(new Tache(*p.taches[i]));//nb et nbMax sont mis � jour dans addTache
    return *this;
}

Projet::~Projet()
{
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
}

//FONCTIONS TACHEMANAGER

TacheManager::TacheManager():taches(0),nb(0),nbMax(0){}

TacheManager::TacheManager(const TacheManager &um):nb(um.nb),nbMax(um.nbMax), taches(new Tache*[um.nb]){
    for(unsigned int i=0; i<nb; i++) taches[i]=new Tache(*um.taches[i]);
}

TacheManager &TacheManager::operator =(const TacheManager &um)
{
    if (this==&um) return *this;//on v�rifie s'il ne s'agit pas du meme objet
    this->~TacheManager();//on d�truit l'objet existant
    for(unsigned int i=0; i<um.nb; i++) addItem(new Tache(*um.taches[i]));//nb et nbMax sont mis � jour dans addItem
    return *this;
}

void TacheManager::addItem(Tache* t){//Ajoute une tache dans le tableau taches de TacheManager et l'agrandi s'il le faut
	if (nb==nbMax){
		Tache** newtab=new Tache*[nbMax+10];
		for(unsigned int i=0; i<nb; i++) newtab[i]=taches[i];
		nbMax+=10;
		Tache** old=taches;
		taches=newtab;
		delete[] old;
	}
    taches[nb++]=t;
}

Tache* TacheManager::trouverTache(const QString& id)const{//renvoie le pointeur si la tache existe, 0 sinon
	for(unsigned int i=0; i<nb; i++)
		if (id==taches[i]->getId()) return taches[i];
	return 0;
}

TacheManager::~TacheManager(){//on sauvegarde dans un fichier � la destruction
    if (file!="") save(file);
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
    file="";
}

Tache& TacheManager::ajouterTache(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt){//on ajoute une tache si elle n'existe pas deja
    if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
    Tache* newt=new Tache(id,t,dur,dispo,deadline,preempt);//si la Tache n'existe pas deja on la cr�e
    addItem(newt);//on l'ajoute au tableau
	return *newt;
}

Tache& TacheManager::getTache(const QString& id){//retourne la tache si elle existe, une erreur sinon
	Tache* t=trouverTache(id);
	if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
	return *t;
}

const Tache& TacheManager::getTache(const QString& id)const{//getTache pour les objets constants
	return const_cast<TacheManager*>(this)->getTache(id);
}



void TacheManager::load(const QString& f){
    //qDebug()<<"debut load\n";
    this->~TacheManager();
    file=f;
    QFile fin(file);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw CalendarException("Erreur ouverture fichier t�ches");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    //qDebug()<<"debut fichier\n";
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named taches, we'll go to the next.
            if(xml.name() == "taches") continue;
            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "tache") {
                qDebug()<<"new tache\n";
                QString identificateur;
                QString titre;
                QDate disponibilite;
                QDate echeance;
                Duree duree;
                bool preemptive;

                QXmlStreamAttributes attributes = xml.attributes();
                /* Let's check that Task has attribute. */
                if(attributes.hasAttribute("preemptive")) {
                    QString val =attributes.value("preemptive").toString();
                    preemptive=(val == "true" ? true : false);
                }
                //qDebug()<<"preemptive="<<preemptive<<"\n";

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named tache.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "tache")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found identificteur.
                        if(xml.name() == "identificateur") {
                            xml.readNext(); identificateur=xml.text().toString();
                            //qDebug()<<"id="<<identificateur<<"\n";
                        }

                        // We've found titre.
                        if(xml.name() == "titre") {
                            xml.readNext(); titre=xml.text().toString();
                            //qDebug()<<"titre="<<titre<<"\n";
                        }
                        // We've found disponibilite
                        if(xml.name() == "disponibilite") {
                            xml.readNext();
                            disponibilite=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"disp="<<disponibilite.toString()<<"\n";
                        }
                        // We've found echeance
                        if(xml.name() == "echeance") {
                            xml.readNext();
                            echeance=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"echeance="<<echeance.toString()<<"\n";
                        }
                        // We've found duree
                        if(xml.name() == "duree") {
                            xml.readNext();
                            duree.setDuree(xml.text().toString().toUInt());
                            //qDebug()<<"duree="<<duree.getDureeEnMinutes()<<"\n";
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                //qDebug()<<"ajout tache "<<identificateur<<"\n";
                ajouterTache(identificateur,titre,duree,disponibilite,echeance,preemptive);

            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw CalendarException("Erreur lecteur fichier taches, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
    //qDebug()<<"fin load\n";
}

void  TacheManager::save(const QString& f){
    file=f;
    QFile newfile( file);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException(QString("erreur sauvegarde t�ches : ouverture fichier xml"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("taches");
    for(unsigned int i=0; i<nb; i++){
        stream.writeStartElement("tache");
        stream.writeAttribute("preemptive", (taches[i]->isPreemptive())?"true":"false");
        stream.writeTextElement("identificateur",taches[i]->getId());
        stream.writeTextElement("titre",taches[i]->getTitre());
        stream.writeTextElement("disponibilite",taches[i]->getDateDisponibilite().toString(Qt::ISODate));
        stream.writeTextElement("echeance",taches[i]->getDateEcheance().toString(Qt::ISODate));
        QString str;
        str.setNum(taches[i]->getDuree().getDureeEnMinutes());
        stream.writeTextElement("duree",str);
        stream.writeEndElement();
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    newfile.close();
}


TacheManager::HandlerTacheManager TacheManager::HandlerTacheManager=TacheManager::HandlerTacheManager();//On initialise handler ici

TacheManager& TacheManager::getInstance(){
    //si l'instance interne n'est pas encore cr��e, on doit le faire
    if (HandlerTacheManager.instance==0) HandlerTacheManager.instance=new TacheManager;
    return *(HandlerTacheManager.instance);
}

void TacheManager::libererInstance(){
    //si l'instance interne n'est pas encore lib�r�e, on doit le faire
    if (HandlerTacheManager.instance!=0) delete HandlerTacheManager.instance;
    HandlerTacheManager.instance=0;
}
//******************************************************************************************

ProgrammationManager::ProgrammationManager():programmations(0),nb(0),nbMax(0){}
void ProgrammationManager::addItem(Programmation* t){
	if (nb==nbMax){
		Programmation** newtab=new Programmation*[nbMax+10];
		for(unsigned int i=0; i<nb; i++) newtab[i]=programmations[i];
		// ou memcpy(newtab,Programmations,nb*sizeof(Programmation*));
		nbMax+=10;
		Programmation** old=programmations;
		programmations=newtab;
		delete[] old;
	}
	programmations[nb++]=t;
}

Programmation* ProgrammationManager::trouverProgrammation(const Tache& t)const{
	for(unsigned int i=0; i<nb; i++)
		if (&t==&programmations[i]->getTache()) return programmations[i];
	return 0;
}

void ProgrammationManager::ajouterProgrammation(const Tache& t, const QDate& d, const QTime& h){
	if (trouverProgrammation(t)) throw CalendarException("erreur, ProgrammationManager, Programmation deja existante");	
	Programmation* newt=new Programmation(t,d,h);
	addItem(newt);
}


ProgrammationManager::~ProgrammationManager(){
	for(unsigned int i=0; i<nb; i++) delete programmations[i];
	delete[] programmations;
}

ProgrammationManager::ProgrammationManager(const ProgrammationManager& um):nb(um.nb),nbMax(um.nbMax), programmations(new Programmation*[um.nb]){
	for(unsigned int i=0; i<nb; i++) programmations[i]=new Programmation(*um.programmations[i]);
}

ProgrammationManager& ProgrammationManager::operator=(const ProgrammationManager& um){
	if (this==&um) return *this;
	this->~ProgrammationManager();
	for(unsigned int i=0; i<um.nb; i++) addItem(new Programmation(*um.programmations[i]));
	return *this;
}

/*
	const Tache* tache;
	Date date;
	Horaire horaire;
    QTextStream& operator<<(QDataStream& f, const Programmation& p);
*/










