#pragma once
#include "Date.h"
#include "Disease.h"
#include <iomanip>

class Reception
{
private:
	Date date;
	int receptionHour;
	int doctorId;
	int patientId;
	bool done = false;
	string patientComplaint;
	string doctorConclusion;

public:
	//Reception(Disease) : Reception() {}
	Reception();
	Reception(string date, int receptionHout, int doctorId, int patientId, string patientComplaint);
	Reception(string reception);

	Date getDate() const { return date; }
	int getDoctorId() const { return doctorId; }
	int getPatientId() const { return patientId; }
	int getReceptionHour() const { return receptionHour; }
	string getComplaint() const{ return patientComplaint; }
	string getConclusion() const { return doctorConclusion; }
	bool isDone() const { return done; }
	bool isEqual(Reception* other);

	string getReceptionString() const;
	void makeReception(string doctorConclusion);
	void tablePrint(int index);
	void print();

	static void printTableHeader();
	friend ostream& operator<<(ostream& output, const Reception* reception);
	friend istream& operator>>(istream& input, Reception* reception);

};