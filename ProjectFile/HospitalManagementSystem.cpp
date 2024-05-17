#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::map;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

#define ENABLE_NEW_ALLOC
#define ENABLE_DE_ALLOC

class ObjectCounter
{
    map<string, int> counter;
    static ObjectCounter *instance;

    ObjectCounter() {}

public:
    static ObjectCounter *getInstance()
    {
        if (instance == NULL)
        {
            instance = new ObjectCounter;
        }
        return instance;
    }

    void increment(string className)
    {
        ++counter[className];
    }

    void decrement(string className)
    {
        --counter[className];
    }

    map<string, int> getCounter()
    {
        return counter;
    }
};

ObjectCounter *ObjectCounter::instance;

#ifdef ENABLE_NEW_ALLOC

inline void new_alloc(string className)
{
    ObjectCounter::getInstance()->increment(className);
}

#endif

#ifdef ENABLE_DE_ALLOC

inline void de_alloc(string className)
{
    ObjectCounter::getInstance()->decrement(className);
}

#endif

// common base class for all the entity class
class DataObject
{
public:
    DataObject() 
    {
        new_alloc("DataObject");
    }

    virtual string toString() const = 0;
    
    virtual ~DataObject() 
    {
        de_alloc("DataObject");
    }
};

class Person : public DataObject
{
protected:
    string name;
    string dateOfBirth;
    string gender;
    string mobile;
    string city;

public:
    Person() 
    { 
        new_alloc("Person");
    }

    Person(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_)
        : name(name_), dateOfBirth(dateOfBirth_), gender(gender_), mobile(mobile_), city(city_) 
    {
        new_alloc("Person");
    }

    string getName() const { return name; }
    void setName(const string &name) { this->name = name; }

    string getDateOfBirth() const { return dateOfBirth; }
    void setDateOfBirth(const string &dateOfBirth) { this->dateOfBirth = dateOfBirth; }

    string getGender() const { return gender; }
    void setGender(const string &gender) { this->gender = gender; }

    string getMobile() const { return mobile; }
    void setMobile(const string &mobile) { this->mobile = mobile; }

    string getCity() const { return city; }
    void setCity(const string &city) { this->city = city; }

    // utility function
    virtual string toString() const
    {
        stringstream ss;
        ss << "Name: " << name << "\n";
        ss << "Date of Birth: " << dateOfBirth << "\n";
        ss << "Gender: " << gender << "\n";
        ss << "Mobile: " << mobile << "\n";
        ss << "City: " << city << "\n";

        return ss.str();
    }

    ~Person() 
    { 
        de_alloc("Person");
    }
};

class MedicalRecord;

class Patient : public Person
{
private:
    string patientID;
    vector<MedicalRecord> medicalRecords;

public:
    Patient() : Person()
    {
        new_alloc("Patient");
    }

    Patient(const string &patientID_, const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_)
        : Person(name_, dateOfBirth_, gender_, mobile_, city_), patientID(patientID_)
    {
        new_alloc("Patient");
    }

    ~Patient()
    {
        de_alloc("Patient");
    }

    string getPatientID() const { return patientID; }
    void setPatientID(const string &patientID) { this->patientID = patientID; }

    const vector<MedicalRecord> &getMedicalRecords() const { return medicalRecords; }
    void addMedicalRecord(const MedicalRecord &medicalRecord) { medicalRecords.push_back(medicalRecord); }

    virtual string toString() const;
};

class Staff : public Person
{
protected:
    string id;

public:
    Staff() : Person() 
    {
        new_alloc("Staff");
    }

    Staff(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_)
        : Person(name_, dateOfBirth_, gender_, mobile_, city_), id(id_) 
    {
        new_alloc("Staff");
    }

    ~Staff()
    {
        de_alloc("Staff");
    }

    string getId() const { return id; }
    void setId(const string &id) { this->id = id; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "ID: " << id << "\n";
        ss << Person::toString();
        return ss.str();
    }
};

class AdministrativeStaff : public Staff
{
private:
    string designation;

public:
    AdministrativeStaff() : Staff() 
    {
        new_alloc("AdministrativeStaff");
    }

    AdministrativeStaff(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_, const string &designation_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), designation(designation_) 
    {
        new_alloc("AdministrativeStaff");
    }

    ~AdministrativeStaff()
    {
        new_alloc("AdministrativeStaff");
    }

    string getDesignation() const { return designation; }
    void setDesignation(const string &designation) { this->designation = designation; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Designation: " << designation << "\n";
        ss << Staff::toString();
        return ss.str();
    }
};

class Doctor;

class MedicalRecord : public DataObject
{
private:
    string recordID;
    string patientID;
    string doctorID;
    string diagnosis;
    string treatment;
    string medications;
    string labResults;

public:
    MedicalRecord()
    {
        new_alloc("MedicalRecord");
    }

    ~MedicalRecord() 
    {
        de_alloc("MedicalRecord");
    }

    MedicalRecord(const string &recordID_, const string &patientID_, const string &doctorID_, const string &diagnosis_, const string &treatment_, const string &medications_, const string &labResults_)
        : recordID(recordID_), patientID(patientID_), doctorID(doctorID_), diagnosis(diagnosis_), treatment(treatment_), medications(medications_), labResults(labResults_) 
    {
        new_alloc("MedicalRecord");
    }

    // Getters and setters
    string getRecordID() const { return recordID; }
    void setRecordID(const string &recordID) { this->recordID = recordID; }

    string getPatientID() const { return patientID; }
    void setPatientID(const string &patientID) { this->patientID = patientID; }

    string getDoctorID() const { return doctorID; }
    void setDoctorID(const string &doctorID) { this->doctorID = doctorID; }

    string getDiagnosis() const { return diagnosis; }
    void setDiagnosis(const string &diagnosis) { this->diagnosis = diagnosis; }

    string getTreatment() const { return treatment; }
    void setTreatment(const string &treatment) { this->treatment = treatment; }

    string getMedications() const { return medications; }
    void setMedications(const string &medications) { this->medications = medications; }

    string getLabResults() const { return labResults; }
    void setLabResults(const string &labResults) { this->labResults = labResults; }

    string toString() const
    {
        stringstream ss;
        ss << "Medical Record ID: " << recordID << endl;
        ss << "Patient ID: " << patientID << endl;
        ss << "Doctor ID: " << doctorID << endl;
        ss << "Diagnosis: " << diagnosis << endl;
        ss << "Treatment: " << treatment << endl;
        ss << "Medications: " << medications << endl;
        ss << "Lab Results: " << labResults << endl;

        return ss.str();
    }
};

string Patient::toString() const
{
    stringstream ss;
    ss << "Patient ID: " << patientID << "\n";
    ss << Person::toString();
    for (const auto &record : medicalRecords)
    {
        ss << "Medical Record: " << record.toString() << "\n";
    }
    return ss.str();
}

class ShiftDetails : public DataObject
{
private:
    string shiftType;
    string timeSlotRange;

public:
    ShiftDetails() 
    {
        new_alloc("ShiftDetails");
    }

    ShiftDetails(const string &shiftType_, const string &timeSlotRange_)
        : shiftType(shiftType_), timeSlotRange(timeSlotRange_) 
    {
        new_alloc("ShiftDetails");
    }

    ~ShiftDetails() 
    {
        de_alloc("ShiftDetails");
    }

    // Getters and setters
    string getShiftType() const { return shiftType; }
    void setShiftType(const string &shiftType) { this->shiftType = shiftType; }

    string getTimeSlotRange() const { return timeSlotRange; }
    void setTimeSlotRange(const string &timeSlotRange) { this->timeSlotRange = timeSlotRange; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Shift Type: " << shiftType << "\n";
        ss << "Time Slot Range: " << timeSlotRange << "\n";
        return ss.str();
    }
};

class ShiftSchedule : public DataObject
{
private:
    map<string, ShiftDetails> shifts; // Maps month to shift details

public:
    ShiftSchedule() 
    {
        new_alloc("ShiftSchedule");
    }

    ~ShiftSchedule() 
    {
        de_alloc("ShiftSchedule");
    }

    void addShift(const string &month, const ShiftDetails &shiftDetails)
    {
        shifts[month] = shiftDetails;
    }

    ShiftDetails getShift(const string &month) const
    {
        auto it = shifts.find(month);
        if (it != shifts.end())
        {
            return it->second;
        }
        return ShiftDetails("", "");
    }

    virtual string toString() const
    {
        stringstream ss;
        for (const auto &entry : shifts)
        {
            ss << "Month: " << entry.first << "\n";
            ss << entry.second.toString() << "\n"; // Use the toString() method of ShiftDetails
        }
        return ss.str();
    }
};

class Doctor : public Staff
{
private:
    string specialization;
    ShiftSchedule shiftSchedule;

public:
    Doctor() : Staff(), shiftSchedule()
    {
        new_alloc("Doctor");
    }

    Doctor(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_, const string &specialization_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), specialization(specialization_), shiftSchedule()
    {
        new_alloc("Doctor");
    }

    ~Doctor()
    {
        de_alloc("Doctor");
    }
    string getSpecialization() const { return specialization; }
    void setSpecialization(const string &specialization) { this->specialization = specialization; }

    const ShiftSchedule &getShiftSchedule() const { return shiftSchedule; }
    void setShiftSchedule(const ShiftSchedule &schedule) { shiftSchedule = schedule; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Specialization: " << specialization << "\n";
        ss << shiftSchedule.toString(); // Use the toString() method of ShiftSchedule
        ss << Staff::toString();
        return ss.str();
    }
};

class Nurse : public Staff
{
private:
    string specialization;
    ShiftSchedule shiftSchedule;

public:
    Nurse() : Staff(), shiftSchedule() 
    {
        new_alloc("Nurse");
    }

    Nurse(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_, const string &specialization_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), specialization(specialization_), shiftSchedule() 
    {
        new_alloc("Nurse");
    }

    ~Nurse()
    {
        de_alloc("Nurse");
    }

    string getSpecialization() const { return specialization; }
    void setSpecialization(const string &specialization) { this->specialization = specialization; }

    const ShiftSchedule &getShiftSchedule() const { return shiftSchedule; }
    void setShiftSchedule(const ShiftSchedule &schedule) { shiftSchedule = schedule; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Specialization: " << specialization << "\n";
        ss << shiftSchedule.toString();
        ss << Staff::toString();
        return ss.str();
    }
};

class Department : public DataObject
{
private:
    string departmentID;
    string name;
    string description;
    string doctorId;

public:
    Department() 
    {
        new_alloc("Department");
    }

    Department(const string& departmentID_, const string& name_, const string& description_, const string& doctorId_)
        : departmentID(departmentID_), name(name_), description(description_), doctorId(doctorId_) 
    {
        new_alloc("Department");
    }

    ~Department()
    {
        de_alloc("Department");
    }

    // Setter and Getter pairs
    void setDepartmentID(const string& departmentID) { this->departmentID = departmentID; }
    string getDepartmentID() const { return departmentID; }

    void setName(const string& name) { this->name = name; }
    string getName() const { return name; }

    void setDescription(const string& description) { this->description = description; }
    string getDescription() const { return description; }

    void setDoctorId(const string& doctorId) { this->doctorId = doctorId; }
    string getDoctorId() const { return doctorId; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Department ID: " << departmentID << "\n";
        ss << "Name: " << name << "\n";
        ss << "Description: " << description << "\n";
        ss << "Doctor Id: " << doctorId << "\n";
        return ss.str();
    }
};

class Appointment : public DataObject
{
private:
    string appointmentID;
    Patient *patient;
    Person *doctor;
    string dateTime;
    string reason;

public:
    Appointment() : patient(nullptr), doctor(nullptr) 
    {
        new_alloc("Appointment");
    }

    ~Appointment()
    {
        de_alloc("Appointment");
    }

    Appointment(const string &appointmentID_, Patient *patient_, Person *doctor_, const string &dateTime_, const string &reason_)
        : appointmentID(appointmentID_), patient(patient_), doctor(doctor_), dateTime(dateTime_), reason(reason_) 
    {
        new_alloc("Appointment");
    }

    // Setter and Getter pairs
    void setAppointmentID(const string &appointmentID) { this->appointmentID = appointmentID; }
    string getAppointmentID() const { return appointmentID; }

    void setPatient(Patient *patient) { this->patient = patient; }
    Person *getPatient() const { return patient; }

    void setDoctor(Person *doctor) { this->doctor = doctor; }
    Person *getDoctor() const { return doctor; }

    void setDateTime(const string &dateTime) { this->dateTime = dateTime; }
    string getDateTime() const { return dateTime; }

    void setReason(const string &reason) { this->reason = reason; }
    string getReason() const { return reason; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Appointment ID: " << appointmentID << "\n";
        ss << "Patient: " << patient->getName() << "\n";
        ss << "Doctor: " << doctor->getName() << "\n";
        ss << "Date & Time: " << dateTime << "\n";
        ss << "Reason: " << reason << "\n";
        return ss.str();
    }
};

class Billing : public DataObject
{
private:
    string billID;
    Person *patient;
    vector<string> servicesProvided;
    double cost;
    bool paymentStatus;

public:
    Billing() : patient(nullptr), cost(0), paymentStatus(false) 
    {
        new_alloc("Builling");
    }

    ~Billing()
    {
        de_alloc("DepartmentForm");
    }

    Billing(const string &billID_, Person *patient_, const vector<string> &servicesProvided_, double cost_)
        : billID(billID_), patient(patient_), servicesProvided(servicesProvided_), cost(cost_), paymentStatus(false) 
    {
        new_alloc("Builling");
    }

    // Setter and Getter pairs
    void setBillID(const string &billID) { this->billID = billID; }
    string getBillID() const { return billID; }

    void setPatient(Person *patient) { this->patient = patient; }
    Person *getPatient() const { return patient; }

    void setServicesProvided(const vector<string> &servicesProvided) { this->servicesProvided = servicesProvided; }
    const vector<string> &getServicesProvided() const { return servicesProvided; }

    void setCost(double cost) { this->cost = cost; }
    double getCost() const { return cost; }

    void setPaymentStatus(bool status) { this->paymentStatus = status; }
    bool getPaymentStatus() const { return paymentStatus; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Bill ID: " << billID << "\n";
        ss << "Patient: " << patient->getName() << "\n";
        ss << "Services Provided:\n";
        for (const auto &service : servicesProvided)
        {
            ss << "- " << service << "\n";
        }
        ss << "Cost: $" << cost << "\n";
        ss << "Payment Status: " << (paymentStatus ? "Paid" : "Pending") << "\n";
        return ss.str();
    }
};

class Prescription : public DataObject
{
private:
    string prescriptionID;
    string patient;
    string doctor;
    string prescriptionDetails;
    string datePrescribed;

public:
    Prescription()
    {
        new_alloc("Prescription");
    }

    ~Prescription()
    {
        de_alloc("Prescription");
    }

    Prescription(const string &prescriptionID_, const string &patient_, const string &doctor_, const string &prescriptionDetails_, const string &datePrescribed_)
        : prescriptionID(prescriptionID_), patient(patient_), doctor(doctor_), prescriptionDetails(prescriptionDetails_), datePrescribed(datePrescribed_) 
    {
        new_alloc("Prescription");
    }

    // Setter and Getter pairs
    void setPrescriptionID(const string &prescriptionID) { this->prescriptionID = prescriptionID; }
    string getPrescriptionID() const { return prescriptionID; }

    void setPatient(const string &patient) { this->patient = patient; }
    string getPatient() const { return patient; }

    void setDoctor(const string &doctor) { this->doctor = doctor; }
    string getDoctor() const { return doctor; }

    void setPrescriptionDetails(const string &prescriptionDetails) { this->prescriptionDetails = prescriptionDetails; }
    string getPrescriptionDetails() const { return prescriptionDetails; }

    void setDatePrescribed(const string &datePrescribed) { this->datePrescribed = datePrescribed; }
    string getDatePrescribed() const { return datePrescribed; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Prescription ID: " << prescriptionID << "\n";
        ss << "Patient: " << patient<< "\n";
        ss << "Doctor: " << doctor<< "\n";
        ss << "Prescription Details: " << prescriptionDetails << "\n";
        ss << "Date Prescribed: " << datePrescribed << "\n";
        return ss.str();
    }
};

class Item : public DataObject
{
private:
    string itemID;
    string itemName;
    string expiryDate;

public:
    Item()
    {
        new_alloc("Item");
    }

    ~Item()
    {
        de_alloc("Item");
    }

    Item(const string &itemID_, const string &itemName_, const string &expiryDate_)
        : itemID(itemID_), itemName(itemName_), expiryDate(expiryDate_) 
    {
        new_alloc("Item");
    }

    string getItemID() const { return itemID; }
    void setItemID(const string &itemID) { this->itemID = itemID; }

    string getItemName() const { return itemName; }
    void setItemName(const string &itemName) { this->itemName = itemName; }

    string getExpiryDate() const { return expiryDate; }
    void setExpiryDate(const string &expiryDate) { this->expiryDate = expiryDate; }

    string toString() const
    {
        return "Item ID: " + itemID + ", Item Name: " + itemName + ", Expiry Date: " + expiryDate;
    }
};

class Supplier : public DataObject
{
private:
    string supplierID;
    string name;
    string mobile;

public:
    Supplier()
    {
        new_alloc("Supplier");
    }

    ~Supplier()
    {
        de_alloc("Supplier");
    }

    Supplier(const string &supplierID_, const string &name_, const string &mobile_)
        : supplierID(supplierID_), name(name_), mobile(mobile_) 
    {
        new_alloc("Supplier");
    }

    string getSupplierID() const { return supplierID; }
    void setSupplierID(const string &supplierID) { this->supplierID = supplierID; }

    string getName() const { return name; }
    void setName(const string &name) { this->name = name; }

    string getMobile() const { return mobile; }
    void setMobile(const string &mobile) { this->mobile = mobile; }

    string toString() const
    {
        return "Supplier ID: " + supplierID + ", Name: " + name + ", Mobile: " + mobile;
    }
};

class StockedItem : public DataObject
{
private:
    string dtPurchase;
    int quantity;
    Item *item;
    Supplier *supplier;

public:
    StockedItem()
    {
        new_alloc("StockedItem");
    }

    ~StockedItem()
    {
        de_alloc("StockedItem");
    }

    StockedItem(const string &dtPurchase_, int quantity_, Item *item_, Supplier *supplier_)
        : dtPurchase(dtPurchase_), quantity(quantity_), item(item_), supplier(supplier_) 
    {
        new_alloc("StockedItem");
    }

    string getDtPurchase() const { return dtPurchase; }
    void setDtPurchase(const string &dtPurchase) { this->dtPurchase = dtPurchase; }

    int getQuantity() const { return quantity; }
    void setQuantity(int quantity) { this->quantity = quantity; }

    Item *getItem() const { return item; }
    void setItem(Item *item) { this->item = item; }

    Supplier *getSupplier() const { return supplier; }
    void setSupplier(Supplier *supplier) { this->supplier = supplier; }

    string toString() const
    {
        return "Date of Purchase: " + dtPurchase + ", Quantity: " + std::to_string(quantity);
    }
};

class Inventory
{
private:
    vector<StockedItem> stockedItems;

public:
    Inventory()
    {
        new_alloc("Inventory");
    }

    ~Inventory()
    {
        de_alloc("Inventory");
    }

    void addStockedItem(const StockedItem &stockedItem)
    {
        stockedItems.push_back(stockedItem);
    }

    const vector<StockedItem> &getStockedItems() const
    {
        return stockedItems;
    }
};

class Test : public DataObject
{
private:
    string testID;
    Patient *patient;
    AdministrativeStaff *technician;
    string testType;
    string result;

public:
    Test() : patient(nullptr), technician(nullptr) 
    {
        new_alloc("Test");
    }

    Test(const string &testID_, Patient *patient_, AdministrativeStaff *technician_, const string &testType_, const string &result_)
        : testID(testID_), patient(patient_), technician(technician_), testType(testType_), result(result_) 
    {
        new_alloc("Test");
    }

    string getTestID() const { return testID; }
    void setTestID(const string &testID) { this->testID = testID; }

    Patient *getPatient() const { return patient; }
    void setPatient(Patient *patient) { this->patient = patient; }

    AdministrativeStaff *getTechnician() const { return technician; }
    void setTechnician(AdministrativeStaff *technician) { this->technician = technician; }

    string getTestType() const { return testType; }
    void setTestType(const string &testType) { this->testType = testType; }

    string getResult() const { return result; }
    void setResult(const string &result) { this->result = result; }

    string toString() const
    {
        return "Test ID: " + testID + ", Test Type: " + testType + ", Result: " + result;
    }

    ~Test()
    {
        de_alloc("Test");
    }
};

class Lab : public DataObject
{
private:
    vector<Test> tests;

public:
    Lab()
    {
        new_alloc("Lab");
    }

    const vector<Test> &getTests() const { return tests; }

    void addTest(const Test &test)
    {
        tests.push_back(test);
    }

    string toString() const
    {
        string output = "Lab Tests:\n";
        for (const auto &test : tests)
        {
            output += test.toString() + "\n";
        }
        return output;
    }

    ~Lab()
    {
        de_alloc("Lab");
    }
};

class Hospital : public DataObject
{
private:
    vector<Department> departments;
    vector<Doctor> doctors;
    vector<Nurse> nurses;
    vector<AdministrativeStaff> administrativeStaff;
    vector<Patient> patients;
    vector<Appointment> appointments;
    vector<MedicalRecord> medicalRecords;
    vector<Billing> bills;
    Lab lab;
    Inventory inventory;

public:
    // Constructors
    Hospital() 
    {
        new_alloc("Hospital");
    }

    // Getter and setter for departments
    const vector<Department> &getDepartments() const { return departments; }
    void addDepartment(const Department &department) { departments.push_back(department); }

    // Getter and setter for doctors
    const vector<Doctor> &getDoctors() const { return doctors; }
    void addDoctor(const Doctor &doctor) { doctors.push_back(doctor); }

    // Getter and setter for nurses
    const vector<Nurse> &getNurses() const { return nurses; }
    void addNurse(const Nurse &nurse) { nurses.push_back(nurse); }

    // Getter and setter for administrative staff
    const vector<AdministrativeStaff> &getAdministrativeStaff() const { return administrativeStaff; }
    void addAdministrativeStaff(const AdministrativeStaff &staff) { administrativeStaff.push_back(staff); }

    // Getter and setter for patients
    const vector<Patient> &getPatients() const { return patients; }
    void addPatient(const Patient &patient) { patients.push_back(patient); }

    // Getter and setter for appointments
    const vector<Appointment> &getAppointments() const { return appointments; }
    void addAppointment(const Appointment &appointment) { appointments.push_back(appointment); }

    // Getter and setter for medical records
    const vector<MedicalRecord> &getMedicalRecords() const { return medicalRecords; }
    void addMedicalRecord(const MedicalRecord &medicalRecord) { medicalRecords.push_back(medicalRecord); }

    // Getter and setter for bills
    const vector<Billing> &getBills() const { return bills; }
    void addBill(const Billing &bill) { bills.push_back(bill); }

    // Getter and setter for lab
    Lab &getLab() { return lab; }

    // Getter and setter for inventory
    Inventory &getInventory() { return inventory; }

    string toString() const
    {
        string output = "Hospital Information:\n";
        // Add more details as needed
        return output;
    }

    ~Hospital()
    {
        de_alloc("Hospital");
    }
};

class TextFileReader
{
public:
    TextFileReader() 
    {
        new_alloc("TextFileReader");
    }

    vector<vector<string>> *readData(const string &filename, char delimiter)
    {
        vector<vector<string>> *data = new vector<vector<string>>;
        ifstream file(filename.c_str());

        if (!file.is_open())
        {
            throw runtime_error("Failed to open file: " + filename);
        }

        string line;
        while (getline(file, line))
        {
            if(line == "")
                continue;
            
            vector<string> row;
            stringstream ss(line);
            string item;

            while (getline(ss, item, delimiter))
            {
                row.push_back(item);
            }

            data->push_back(row);
        }

        if(data->size() == 0)
            throw runtime_error("File is empty: " + filename);

        file.close();

        return data;
    }

    ~TextFileReader()
    {
        de_alloc("TextFileReader");
    }
};

class ObjectCache
{
    static ObjectCache *instance;

    map<string, map<string, DataObject *>*> objectsMap;

    ObjectCache() 
    {
        new_alloc("ObjectCache");

        objectsMap["Patient"] = new map<string, DataObject *>;
        objectsMap["Doctor"] = new map<string, DataObject *>;
        objectsMap["Department"] = new map<string, DataObject *>;
        objectsMap["MedicalRecord"] = new map<string, DataObject *>;
        objectsMap["Prescription"] = new map<string, DataObject *>;
    }

public:
    static ObjectCache *getInstance()
    {
        if (instance == NULL)
            instance = new ObjectCache;
        return instance;
    }
    
    void addPatients(Patient *patient) 
    { 
        (*(objectsMap)["Patient"])[patient->getPatientID()] = patient;
    }

    void addDoctors(Doctor *doctor) 
    {
        (*(objectsMap)["Doctor"])[doctor->getId()] = doctor;
    }

    void addDepartments(Department *department) 
    { 
        (*(objectsMap)["Department"])[department->getDepartmentID()] = department;
    }
    
    void addMedicalRecords(MedicalRecord *medicalRecord)
    {
        (*(objectsMap)["MedicalRecord"])[medicalRecord->getRecordID()] = medicalRecord;
    }

    void addPrescriptions(Prescription *prescription)
    {
        (*(objectsMap)["Prescription"])[prescription->getPrescriptionID()] = prescription;
    }

    map<string, DataObject *> *getPatients() { return objectsMap["Patient"]; }
    map<string, DataObject *> *getDoctors() { return objectsMap["Doctor"]; }
    map<string, DataObject *> *getDepartments() { return objectsMap["Department"]; }
    map<string, DataObject *> *getMedicalRecords() { return objectsMap["MedicalRecord"]; }
    map<string, DataObject *> *getPrescriptions() { return objectsMap["Prescription"]; }


    Patient *getPatient(const string &id) 
    {
        return dynamic_cast<Patient*>((*objectsMap["Patient"])[id]);
    }

    Doctor *getDoctor(const string &id) 
    {
        return dynamic_cast<Doctor *>((*objectsMap["Doctor"])[id]);
    }

    Department *getDepartment(const string &id) 
    {
        return dynamic_cast<Department *>((*objectsMap["Department"])[id]);
    }

    MedicalRecord *getMedicalRecord(const string&id)
    {
        return dynamic_cast<MedicalRecord*>((*objectsMap["MedicalRecord"])[id]);
    }

    Prescription *getPrescription(const string&id)
    {
        return dynamic_cast<Prescription*>((*objectsMap["Prescription"])[id]);
    }
   
    vector<Patient *> *getPatientList()
    {
        map<string, DataObject *> &patients = *(objectsMap["Patient"]);
        vector<Patient *> *patientList = new vector<Patient *>;
        for (auto it = patients.begin(); it != patients.end(); ++it)
        {
            patientList->push_back(dynamic_cast<Patient*>(it->second));
        }
        return patientList;
    }

    vector<Doctor *> *getDoctorList()
    {
        map<string, DataObject *> &doctors = *(objectsMap["Doctor"]);
        vector<Doctor *> *doctorList = new vector<Doctor *>;
        for (auto it = doctors.begin(); it != doctors.end(); ++it)
        {
            doctorList->push_back(dynamic_cast<Doctor*>(it->second));
        }
        return doctorList;
    }

    vector<Department *> *getDepartmentList()
    {
        map<string, DataObject *> &departments = *(objectsMap["Department"]);
        vector<Department *> *departmentList = new vector<Department *>;
        for (auto it = departments.begin(); it != departments.end(); ++it)
        {
            departmentList->push_back(dynamic_cast<Department*>(it->second));
        }
        return departmentList;
    }

    vector<MedicalRecord*> *getMedicalRecordList()
    {
        map<string, DataObject *> &medicalRecords = *(objectsMap["MedicalRecord"]);
        vector<MedicalRecord *> *medicalRecordList = new vector<MedicalRecord *>;
        for (auto it = medicalRecords.begin(); it != medicalRecords.end(); ++it)
        {
            medicalRecordList->push_back(dynamic_cast<MedicalRecord *>(it->second));
        }
        return medicalRecordList;
    }

    vector<Prescription*> *getPrescriptionList()
    {
        map<string, DataObject *> &prescriptions = *(objectsMap["Prescription"]);
        vector<Prescription *> *prescriptionList = new vector<Prescription *>;
        for (auto it = prescriptions.begin(); it != prescriptions.end(); ++it)
        {
            prescriptionList->push_back(dynamic_cast<Prescription *>(it->second));
        }
        return prescriptionList;
    }

    ~ObjectCache()
    {  
        map<string, DataObject *> &patients = *(objectsMap["Patient"]);
        if(patients.size() > 0) releasePatients();

        map<string, DataObject *> &doctors = *(objectsMap["Doctor"]);      
        if(doctors.size() > 0) releaseDoctors();
        
        map<string, DataObject *> &departments = *(objectsMap["Department"]);
        if(departments.size() > 0) releaseDepartments();

        map<string, DataObject *> &medicalRecords = *(objectsMap["MedicalRecord"]);
        if(medicalRecords.size() > 0) releaseMedicalRecords();

        map<string, DataObject *> &prescriptions = *(objectsMap["Prescription"]);
        if(prescriptions.size() > 0) releasePrescriptions();

        delete objectsMap["Patient"];
        delete objectsMap["Doctor"];
        delete objectsMap["Department"];
        delete objectsMap["MedicalRecord"];
        delete objectsMap["Prescription"];

        de_alloc("ObjectCache");
    }

private:
    void releasePatients()
    {
        vector<Patient *> *dataObjects = this->getPatientList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = NULL;
    }

    void releaseDoctors()
    {
        vector<Doctor *> *dataObjects = this->getDoctorList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = NULL;
    }

    void releaseDepartments()
    {
        vector<Department *> *dataObjects = this->getDepartmentList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = NULL;
    }

    void releaseMedicalRecords()
    {
        vector<MedicalRecord *> *dataObjects = this->getMedicalRecordList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = NULL;
    }

    void releasePrescriptions()
    {
        vector<Prescription *> *dataObjects = this->getPrescriptionList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = NULL;
    }
};

ObjectCache *ObjectCache::instance;

class Builder
{
protected:
    string filepath;
    virtual vector<DataObject *> *constructDataObject(vector<vector<string>> *data) = 0;

public:
    Builder()
    {
        new_alloc("Builder");
        filepath = "C:/Users/care point/Desktop/TalenciaGlobal-Trainning/Training/Data/";
    }

    vector<DataObject *> *makeDataObjectList(char delimiter = ';')
    {
        TextFileReader *textFileReader = new TextFileReader;
        vector<vector<string>> *data = NULL;
        vector<DataObject *> *dataObjects = NULL;
        
        try{
            data = textFileReader->readData(filepath, delimiter);
            dataObjects = constructDataObject(data); 
        }
        catch(const runtime_error& e)
        {
            cout << "Caught a runtime error " << e.what() << endl;
        }

        delete data;
        delete textFileReader;
        return dataObjects;
    }

    virtual ~Builder()
    {
        de_alloc("Builder");
    }
};

class PatientBuilder : public Builder
{
public:
    PatientBuilder()
    {
        new_alloc("PatientBuilder");
        filepath += "Patients.data";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> >*data)
    {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());
        for (int i = 0; i < data->size(); ++i)
        {
            string &id = (*data)[i][0];

            string &name = (*data)[i][1];
            string &dateOfBirth = (*data)[i][2];
            string &gender = (*data)[i][3];
            string &mobile = (*data)[i][4];
            string &city = (*data)[i][5];

            Patient *patient = new Patient(id, name, dateOfBirth, gender, mobile, city);
            (*dataObjects)[i] = patient;

            ObjectCache::getInstance()->addPatients(patient);
        }
        return dataObjects;
    }

    ~PatientBuilder()
    {
        de_alloc("PatientBuilder");
    }
};

class DoctorBuilder : public Builder
{
public:
    DoctorBuilder()
    {
        new_alloc("DoctorBuilder");
        filepath += "Doctors.data";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> > *data)
    {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());

        for (int i = 0; i < data->size(); ++i)
        {
            string &name = (*data)[i][1];
            string &dateOfBirth = (*data)[i][2];
            string &gender = (*data)[i][3];
            string &mobile = (*data)[i][4];
            string &city = (*data)[i][5];

            string &id = (*data)[i][0];
            string &specialization = (*data)[i][6];

            Doctor *doctor = new Doctor(name, dateOfBirth, gender, mobile, city, id, specialization);
            (*dataObjects)[i] = doctor;

            ObjectCache::getInstance()->addDoctors(doctor);
        }
        return dataObjects;
    }

    ~DoctorBuilder()
    {
        de_alloc("DoctorBuilder");
    }
};

class DepartmentBuilder : public Builder
{
public:
    DepartmentBuilder()
    {
        new_alloc("DepartmentBuilder");
        filepath += "Departments.data";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> > *data)
    {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());

        for (int i = 0; i < data->size() && (*data)[i].size(); ++i)
        {
            string &departmentID = (*data)[i][0];
            string &name = (*data)[i][1];
            string &description = (*data)[i][2];
            string &doctorId = (*data)[i][3];

            Department *department = new Department(departmentID, name, description, doctorId);
            (*dataObjects)[i] = department;

            ObjectCache::getInstance()->addDepartments(department);
        }
        return dataObjects;
    }

    ~DepartmentBuilder()
    {
        de_alloc("DepartmentBuilder");
    }
};

class MedicalRecordBuilder : public Builder
{
public:
    MedicalRecordBuilder()
    {
        new_alloc("MedicalRecordBuilder");
        filepath += "MedicalRecords.Data";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> >* data)
    {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());
        for (int i = 0; i < data->size(); ++i)
        {
            string &recordId = (*data)[i][0];
            string &patientId = (*data)[i][1];
            string &doctorId = (*data)[i][2];
            string &diagnosis = (*data)[i][3];
            string &treatment = (*data)[i][4];
            string &medications = (*data)[i][5];
            string &labResults = (*data)[i][6];

            MedicalRecord *medicalRecord = new MedicalRecord(recordId, patientId, doctorId, diagnosis, treatment, medications, labResults);
            (*dataObjects)[i] = medicalRecord;

            ObjectCache::getInstance()->addMedicalRecords(medicalRecord);
        }

        return dataObjects;
    }

    ~MedicalRecordBuilder()
    {
        de_alloc("MedicalRecordBuilder");
    }
};

class PrescriptionBuilder : public Builder {
public:
    PrescriptionBuilder() {
        new_alloc("PrescriptionBuilder");
        filepath += "Prescriptions.Data";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> > *data) 
    {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());
        for (int i = 0; i < data->size(); ++i) 
        {
            string& prescriptionID = (*data)[i][0];  
            string& patient = (*data)[i][1];        
            string& doctor = (*data)[i][2];            
            string& prescriptionDetails = (*data)[i][3];
            string& datePrescribed = (*data)[i][4];

            Prescription *prescription = new Prescription(prescriptionID, patient, doctor, prescriptionDetails, datePrescribed);
            (*dataObjects)[i] = prescription;

            ObjectCache::getInstance()->addPrescriptions(prescription); 
        }
        
        return dataObjects;
    }

    ~PrescriptionBuilder() {
        de_alloc("PrescriptionBuilder");
    }
};

class BuilderFactory
{
    map<string, Builder *> builders;
    static BuilderFactory *instance;

    BuilderFactory()
    {
        new_alloc("BuilderFactory");

        builders["Patient"] = new PatientBuilder;
        builders["Doctor"] = new DoctorBuilder;
        builders["Department"] = new DepartmentBuilder;
        builders["MedicalRecord"] = new MedicalRecordBuilder;
        builders["Prescription"] = new PrescriptionBuilder;
    }

public:
    static BuilderFactory* getInstance()
    {
        if(instance == NULL)
            instance = new BuilderFactory;
        return instance;
    }

    Builder *createBuilder(string className)
    {
        return builders[className];
    }

    ~BuilderFactory()
    {
        for(auto &builder: builders)
        {
            delete builder.second;
        }
        de_alloc("BuilderFactory");
    }
};

BuilderFactory *BuilderFactory::instance;

class Form
{
public:
    virtual void displayData(vector<DataObject *>* dataObjects) = 0;
    virtual void displayCacheData() = 0;
    
    Form()
    {
        new_alloc("Form");
    }

    virtual ~Form()
    {
        de_alloc("Form");
    }
};

class PatientForm : public Form
{
    void displayPatient(Patient *patients)
    {
        cout << "Id: " << patients->getPatientID() << endl;
        cout << "Name: " << patients->getName() << endl;
        cout << "Date Of Birth: " << patients->getDateOfBirth() << endl;
        cout << "Gender: " << patients->getGender() << endl;
        cout << "Mobile: " << patients->getMobile() << endl;
        cout << "City: " << patients->getCity() << endl
        << endl;
    }

public:
    PatientForm()
    {
        new_alloc("PatientForm");
    }

    void displayData(vector<DataObject *>* dataObjects)
    {
        cout << "Patient's Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dataObject = (*dataObjects)[i];
            Patient *patient = dynamic_cast<Patient *>(dataObject);
            displayPatient(patient);
        }
    }

    void displayCacheData()
    {
        map<string, DataObject*>* dataObjects = ObjectCache::getInstance()->getPatients();
        cout << "Patient's Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (auto patient : (*dataObjects))
        {
            cout << dynamic_cast<Patient*>(patient.second)->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~PatientForm()
    {
        de_alloc("PatientForm");
    }
};

class DoctorForm : public Form
{
private:
    void displayDoctor(Doctor *doctor)
    {
        cout << "Name: " << doctor->getName() << endl;
        cout << "Date Of Birth: " << doctor->getDateOfBirth() << endl;
        cout << "Gender: " << doctor->getGender() << endl;
        cout << "Mobile: " << doctor->getMobile() << endl;
        cout << "City: " << doctor->getCity() << endl;
        cout << "Id: " << doctor->getId() << endl;
        cout << "Specialization: " << doctor->getSpecialization() << endl
        << endl;
    }

public:
    DoctorForm()
    {
        new_alloc("DoctorForm");
    }

    void displayData(vector<DataObject *>* dataObjects)
    {
        cout << "Doctor's Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dataObject = (*dataObjects)[i];
            Doctor *doctor = dynamic_cast<Doctor *>(dataObject);
            displayDoctor(doctor);
        }
    }

    void displayCacheData()
    {
        map<string, DataObject *>* dataObjects = ObjectCache::getInstance()->getDoctors();
        cout << "Doctor's Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (auto doctor : (*dataObjects))
        {
            cout << dynamic_cast<Doctor*>(doctor.second)->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~DoctorForm()
    {
        de_alloc("DoctorForm");
    }
};

class DepartmentForm : public Form
{
private:
    void displayDepartment(Department *department)
    {
        cout << "Department Id: " << department->getDepartmentID() << endl;
        cout << "Name: " << department->getName() << endl;
        cout << "Description: " << department->getDescription() << endl;
        cout << "Doctor Id: " << department->getDoctorId() << endl
                << endl;
    }

public:
    DepartmentForm()
    {
        new_alloc("DepartmentForm");
    }

    void displayData(vector<DataObject *>* dataObjects)
    {
        cout << "Departments's Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dataObject = (*dataObjects)[i];
            Department *department = dynamic_cast<Department *>(dataObject);
            displayDepartment(department);
        }
    }

    void displayCacheData()
    {
        map<string, DataObject *> *dataObjects = ObjectCache::getInstance()->getDepartments();
        cout << "Department's Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for(auto department: (*dataObjects))
        {
            cout << dynamic_cast<Department*>(department.second)->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~DepartmentForm()
    {
        de_alloc("DepartmentForm");
    }
};

class MedicalRecordForm : public Form
{
private:
    void displayMedicalRecords(MedicalRecord *medicalRecord)
    {
        cout << "Record Id: " << medicalRecord->getRecordID() << endl;
        cout << "Patient Id: " << medicalRecord->getPatientID() << endl;
        cout << "Doctor Id: " << medicalRecord->getDoctorID() << endl;
        cout << "Diagnosis: " << medicalRecord->getDiagnosis() << endl;
        cout << "Treatment: " << medicalRecord->getTreatment() << endl;
        cout << "Medications: " << medicalRecord->getMedications() << endl;
        cout << "Lab Results: " << medicalRecord->getLabResults() << endl
             << endl;
    }

public:
    MedicalRecordForm()
    {
        new_alloc("MedicalRecordForm");
    }

    void displayData(vector<DataObject *>* dataObjects)
    {
        cout << "Medical Record's Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dataObject = (*dataObjects)[i];
            MedicalRecord *medicalRecord = dynamic_cast<MedicalRecord *>(dataObject);
            displayMedicalRecords(medicalRecord);
        }
    }

    void displayCacheData()
    {
        map<string, DataObject *> *dataObjects = ObjectCache::getInstance()->getMedicalRecords();
        cout << "Medical Record's Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for(auto medicalRecord: (*dataObjects))
        {
            cout << dynamic_cast<MedicalRecord *>(medicalRecord.second)->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~MedicalRecordForm()
    {
        de_alloc("MedicalRecordForm");
    }
};

class PrescriptionForm : public Form
{
private:
    void displayPrescription(Prescription *prescription)
    {
        cout << "Prescription ID: " << prescription->getPrescriptionID() << endl;
        cout << "Patient: " << prescription->getPatient() << endl;
        cout << "Doctor: " << prescription->getDoctor() << endl;
        cout << "Prescription Details: " << prescription->getPrescriptionDetails() << endl;
        cout << "Date Prescribed: " << prescription->getDatePrescribed() << endl << endl;
    }

public:
    PrescriptionForm()
    {
        new_alloc("PrescriptionForm");
    }

    void displayData(vector<DataObject*>* dataObjects)
    {
        cout << "Prescription's Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dataObject = (*dataObjects)[i];
            Prescription *prescription = dynamic_cast<Prescription *>(dataObject);
            displayPrescription(prescription);
        }
    }

    void displayCacheData()
    {
        map<string, DataObject *> *dataObjects = ObjectCache::getInstance()->getPrescriptions();
        cout << "Prescription's Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for(auto prescription: (*dataObjects))
        {
            cout << dynamic_cast<Prescription *>(prescription.second)->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~PrescriptionForm()
    {
        de_alloc("PrescriptionForm");
    }
};

class FormFactory
{
    map<string, Form*> forms;
    static FormFactory *instance;

    FormFactory()
    {
        new_alloc("FormFactory");

        forms["Patient"] = new PatientForm;
        forms["Doctor"] = new DoctorForm;
        forms["Department"] = new DepartmentForm;
        forms["MedicalRecord"] = new MedicalRecordForm;
        forms["Prescription"] = new PrescriptionForm;
    }

public:
    static FormFactory* getInstance()
    {
        if(instance == NULL)
            instance = new FormFactory;
        return instance;
    }

    Form* createForm(string className)
    {
        return forms[className];
    }

    ~FormFactory()
    {
        for(auto &form: forms)
        {
            delete form.second;
        }
        de_alloc("FormFactory");
    }
};

FormFactory *FormFactory::instance;

class Controller
{
public:
    virtual void execute() = 0;
    virtual ~Controller() {}
};

class PatientController : public Controller
{
    static PatientController *instance;
    PatientController()
    {
        new_alloc("PatientController");
    }

public:
    static PatientController* getInstance()
    {
        if(instance == NULL)
            instance = new PatientController;
        return instance;
    }

    void execute()
    {
        Builder *builder = BuilderFactory::getInstance()->createBuilder("Patient");
        vector<DataObject *> *dataObjects = builder->makeDataObjectList();

        Form *form = FormFactory::getInstance()->createForm("Patient");
        form->displayData(dataObjects);

        cout << "-------------------------------------" << endl;
        form->displayCacheData();
    }    

    ~PatientController()
    {
        de_alloc("PatientController");
    }
};

PatientController *PatientController::instance;

class DoctorController : public Controller
{
    static DoctorController *instance;

    DoctorController()
    {
        new_alloc("DoctorController");
    }

public:
    static DoctorController* getInstance()
    {
        if(instance == NULL)
            instance = new DoctorController;
        return instance;
    }

    void execute()
    {
        Builder *builder = BuilderFactory::getInstance()->createBuilder("Doctor");
        vector<DataObject *> *dataObjects = builder->makeDataObjectList();

        Form *form = FormFactory::getInstance()->createForm("Doctor");
        form->displayData(dataObjects);

        cout << "-------------------------------------" << endl;
        form->displayCacheData();
    }

    ~DoctorController()
    {
        de_alloc("DoctorController");
    }
};

DoctorController *DoctorController::instance;

class DepartmentController : public Controller
{
    static DepartmentController *instance;

    DepartmentController()
    {
        new_alloc("DepartmentController");
    }

public:
    static DepartmentController* getInstance()
    {
        if(instance == NULL)
            instance = new DepartmentController;
        return instance;
    }

    void execute()
    {
        Builder *builder = BuilderFactory::getInstance()->createBuilder("Department");
        vector<DataObject *> *dataObjects = builder->makeDataObjectList(',');

        Form *form = FormFactory::getInstance()->createForm("Department");
        form->displayData(dataObjects);

        cout << "-------------------------------------" << endl;
        form->displayCacheData();
    }

    ~DepartmentController()
    {
        de_alloc("DepartmentController");
    }
};

DepartmentController *DepartmentController::instance;

class MedicalRecordController : public Controller
{
    static MedicalRecordController *instance;

    MedicalRecordController()
    {
        new_alloc("MedicalRecordController");
    }

public:
    static MedicalRecordController* getInstance()
    {
        if(instance == NULL)
            instance = new MedicalRecordController;
        return instance;
    }

    void execute()
    {
        Builder *builder = BuilderFactory::getInstance()->createBuilder("MedicalRecord");
        vector<DataObject *> *dataObjects = builder->makeDataObjectList();

        Form *form = FormFactory::getInstance()->createForm("MedicalRecord");
        form->displayData(dataObjects);

        cout << "-------------------------------------" << endl;
        form->displayCacheData();       
    }

    ~MedicalRecordController()
    {
        de_alloc("MedicalRecordController");
    }
};

MedicalRecordController * MedicalRecordController::instance;

class PrescriptionController : public Controller
{
    static PrescriptionController *instance;

    PrescriptionController()
    {
        new_alloc("PrescriptionController");
    }

public:
    static PrescriptionController* getInstance()
    {
        if(instance == NULL)
            instance = new PrescriptionController;
        return instance;
    }

    void execute()
    {
        Builder *builder = BuilderFactory::getInstance()->createBuilder("Prescription");
        vector<DataObject *> *dataObjects = builder->makeDataObjectList(';');

        Form *form = FormFactory::getInstance()->createForm("Prescription");
        form->displayData(dataObjects);

        cout << "-------------------------------------" << endl;
        form->displayCacheData();
    }

    ~PrescriptionController()
    {
        de_alloc("PrescriptionController");
    }
};

PrescriptionController *PrescriptionController::instance;

void displayObjectCounter()
{
    map<string, int> counter = ObjectCounter::getInstance()->getCounter();

    cout << endl
         << "Classes Count: " << counter.size() << endl;
    cout << "------------------------------------" << endl;

    for (auto it = counter.begin(); it != counter.end(); ++it)
    {
        cout << it->first << "=" << it->second << endl;
    }
}

class HMSAppController : public Controller
{
    static HMSAppController *instance;
    HMSAppController()
    {
        new_alloc("HMSAppController");
        #ifdef ENABLE_NEW_ALLOC
            displayObjectCounter();
        #endif
    }

public:
    static HMSAppController* getInstance()
    {
        if(instance == NULL)
            instance = new HMSAppController;
        return instance;
    }

    void execute()
    {
        PatientController::getInstance()->execute();
        DoctorController::getInstance()->execute();
        DepartmentController::getInstance()->execute();
        MedicalRecordController::getInstance()->execute();
        PrescriptionController::getInstance()->execute();
    }

    ~HMSAppController()
    {
        #ifdef ENABLE_NEW_ALLOC
            displayObjectCounter();
        #endif    

            delete PatientController::getInstance();
            delete DoctorController::getInstance();
            delete DepartmentController::getInstance();
            delete MedicalRecordController::getInstance();
            delete PrescriptionController::getInstance();

            delete ObjectCache::getInstance();
            delete FormFactory::getInstance();            
            delete BuilderFactory::getInstance();

            de_alloc("HMSAppController");

        #ifdef ENABLE_NEW_ALLOC
            displayObjectCounter();
        #endif
    }
};

HMSAppController *HMSAppController::instance;

int main()
{
    Controller *controller = HMSAppController::getInstance();
    controller->execute();

    delete controller;
    controller = NULL;

    delete ObjectCounter::getInstance();

    return 0;
}
