#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class MyDatabase : public QObject
{
    Q_OBJECT
public:

    static MyDatabase &getInstance()
    {
        static MyDatabase instance;
        return instance;
     }
     QString userLogin(QString userName,QString password);
     QString doctorLogin(QString userName,QString password);

private:
    explicit MyDatabase(QObject *parent = nullptr);
    MyDatabase(MyDatabase const&)= delete;
    void operator=(MyDatabase const&)  = delete;

    QSqlDatabase database;

    void InitDatabase();


signals:


public:
    bool initPatientModel();
    int addNewPatient();
    bool searchPatient(QString filter);
    bool deleteCurrentPatient();
    bool submitPatientEdit();
    void revertPatient();
    bool initMedicineModel();
    int addNewMedicine();
    bool searchMedicine(QString filter);
    bool deleteCurrentMedicine();
    bool submitMedicineEdit();
    void revertMedicine();
    bool initRecoreModel();
    int addNewRecore();
    bool searchRecore(QString filter);
    bool deleteCurrentRecore();
    bool submitRecoreEdit();
    void revertRecore();
    bool initDoctorModel();
    int addNewDoctor();
    bool searchDoctor(QString filter);
    bool deleteCurrentDoctor();
    bool submitDoctorEdit();
    void revertDoctor();

    QSqlTableModel *patientTabModel;
    QItemSelectionModel *thePatientSelection;
    QSqlTableModel *medicineTabModel;
    QItemSelectionModel *theMedicineSelection;
    QSqlTableModel *recordTabModel;
    QItemSelectionModel *theRecordSelection;
    QSqlTableModel *doctorTabModel;
    QItemSelectionModel *theDoctorSelection;
};

#endif // MYDATABASE_H
