#include "MyDatabase.h"
#include <QDebug>
#include <QUuid>

void MyDatabase::InitDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile="Lab4a.db";
    database.setDatabaseName(aFile);
    if(!database.open()){
        qDebug()<<"fail to open database";
    }else{
        qDebug()<<"open database is ok";
    }
}

bool MyDatabase::initPatientModel()
{
    patientTabModel=new QSqlTableModel(this,database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientTabModel->setSort(patientTabModel->fieldIndex("name"),Qt::AscendingOrder);
    if(!(patientTabModel->select()))
        return false;

    thePatientSelection=new QItemSelectionModel(patientTabModel);
    return true;
}

int MyDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount()-1,1);

    int curRecNo=curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);
    curRec.setValue("CREATEDTIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));
    patientTabModel->setRecord(curRecNo,curRec);
    return curIndex.row();
}

bool MyDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool MyDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex=thePatientSelection->currentIndex();
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();

    return true;
}

bool MyDatabase::submitPatientEdit()
{
    return patientTabModel->submitAll();
}

void MyDatabase::revertPatient()
{
    return patientTabModel->revertAll();
}

bool MyDatabase::initMedicineModel()
{
    medicineTabModel=new QSqlTableModel(this,database);
    medicineTabModel->setTable("medicine");
    medicineTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    medicineTabModel->setSort(medicineTabModel->fieldIndex("billname"),Qt::AscendingOrder);
    if(!(medicineTabModel->select()))
    {
        qDebug() << QStringLiteral("初始化错误 ！");
        return false;
    }

    theMedicineSelection=new QItemSelectionModel(medicineTabModel);
    qDebug() << "初始化成功";
    return true;
}

int MyDatabase::addNewMedicine()
{
    medicineTabModel->insertRow(medicineTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = medicineTabModel->index(medicineTabModel->rowCount()-1,1);
    int curRecNo=curIndex.row();
    QSqlRecord curRec = medicineTabModel->record(curRecNo);
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));
    medicineTabModel->setRecord(curRecNo,curRec);
    return curIndex.row();
}

bool MyDatabase::searchMedicine(QString filter)
{
    medicineTabModel->setFilter(filter);
    return medicineTabModel->select();
}

bool MyDatabase::deleteCurrentMedicine()
{
    QModelIndex curIndex=theMedicineSelection->currentIndex();
    medicineTabModel->removeRow(curIndex.row());
    medicineTabModel->submitAll();
    medicineTabModel->select();

    return true;
}

bool MyDatabase::submitMedicineEdit()
{
    return medicineTabModel->submitAll();
}

void MyDatabase::revertMedicine()
{
    return medicineTabModel->revertAll();
}

bool MyDatabase::initRecoreModel()
{
    recordTabModel=new QSqlTableModel(this,database);
    recordTabModel->setTable("record");
    recordTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    recordTabModel->setSort(recordTabModel->fieldIndex("pname"),Qt::AscendingOrder);
    if(!(recordTabModel->select()))
    {
        qDebug() <<"初始化错误 ！";
        return false;
    }

    theRecordSelection=new QItemSelectionModel(recordTabModel);
    qDebug()<<"初始化成功";
    return true;
}

int MyDatabase::addNewRecore()
{
    recordTabModel->insertRow(recordTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = recordTabModel->index(recordTabModel->rowCount()-1,1);
    int curRecNo=curIndex.row();
    QSqlRecord curRec = recordTabModel->record(curRecNo);
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));
    recordTabModel->setRecord(curRecNo,curRec);
    return curIndex.row();
}

bool MyDatabase::searchRecore(QString filter)
{
    recordTabModel->setFilter(filter);
    return recordTabModel->select();
}

bool MyDatabase::deleteCurrentRecore()
{
    QModelIndex curIndex=theRecordSelection->currentIndex();
    recordTabModel->removeRow(curIndex.row());
    recordTabModel->submitAll();
    recordTabModel->select();

    return true;
}

bool MyDatabase::submitRecoreEdit()
{
    return recordTabModel->submitAll();
}

void MyDatabase::revertRecore()
{
    return recordTabModel->revertAll();
}

bool MyDatabase::initDoctorModel()
{
    doctorTabModel=new QSqlTableModel(this,database);
    doctorTabModel->setTable("Doctor");
    doctorTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    doctorTabModel->setSort(doctorTabModel->fieldIndex("dname"),Qt::AscendingOrder);
    if(!(doctorTabModel->select()))
    {
        qDebug()<<"初始化错误";
        return false;
    }

    theDoctorSelection=new QItemSelectionModel(doctorTabModel);
    qDebug()<<"初始化成功";
    return true;
}

int MyDatabase::addNewDoctor()
{
    doctorTabModel->insertRow(doctorTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = doctorTabModel->index(doctorTabModel->rowCount()-1,1);
    int curRecNo=curIndex.row();
    QSqlRecord curRec = doctorTabModel->record(curRecNo);
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));
    doctorTabModel->setRecord(curRecNo,curRec);
    return curIndex.row();
}

bool MyDatabase::searchDoctor(QString filter)
{
    doctorTabModel->setFilter(filter);
    return doctorTabModel->select();
}

bool MyDatabase::deleteCurrentDoctor()
{
    QModelIndex curIndex=theDoctorSelection->currentIndex();
    doctorTabModel->removeRow(curIndex.row());
    doctorTabModel->submitAll();
    doctorTabModel->select();\

    return true;
}

bool MyDatabase::submitDoctorEdit()
{
    return doctorTabModel->submitAll();
}

void MyDatabase::revertDoctor()
{
    return doctorTabModel->revertAll();
}

QString MyDatabase::userLogin(QString userName, QString password)//管理员登录
{

    QSqlQuery query;
    //查询数据库表
    query.prepare("select username,password from user where username=:USER");
    query.bindValue(":USER",userName);
    query.exec();
    //检验账号密码
    if(query.first()&&query.value("username").isValid()){
        QString passwd=query.value("password").toString();
        if(passwd==password){
            return "loginOk";
        }
        else{
             qDebug()<<"wrongPassword";
            return "wrongPassword";
        }
    }
    else{
        qDebug()<<"no such user";
        return "wrongUsername";
    }

}

QString MyDatabase::doctorLogin(QString userName, QString password)
{
    QSqlQuery query;
    query.prepare("select dname,dpassword from doctorlogin where dname=:USER");
    query.bindValue(":USER",userName);
    query.exec();
    //检验账号密码
    if(query.first()&&query.value("dname").isValid()){
        QString passwd=query.value("dpassword").toString();
        if(passwd==password){
            return "loginOk";
        }
        else{
             qDebug()<<"wrongPassword";
            return "wrongPassword";
        }
    }
    else{
        qDebug()<<"no such doctor";
        return "wrongDoctorname";
    }
}

MyDatabase::MyDatabase(QObject *parent) : QObject(parent)
{
    InitDatabase();
}
