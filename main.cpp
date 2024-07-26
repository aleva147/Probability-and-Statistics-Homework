#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

// Unapred definisane konstante zadatka:
const int DUZ_NIZA = 10;

const double pA = 0.3;	// Prosek jedinica ako je signal poslat sa mesta A.
const double pB = 0.4;	// Prosek jedinica ako je signal poslat sa mesta B.
const double pC = 0.5;	// Prosek jedinica ako je signal poslat sa mesta C.

// Pomocne f-je:
int fakt(int br) {
	if (br < 0) exit(-1);
	if (br == 0) return 1;

	int rez = 1;
	for (int i = 2; i <= br; i++) {
		rez *= i;
	}
	return rez;
}

double izrUslovnuVrv(int brJedinica, double p) {
	return  fakt(DUZ_NIZA) / (fakt(brJedinica) * fakt(DUZ_NIZA - brJedinica)) * pow(p, brJedinica) * pow(1 - p, DUZ_NIZA - brJedinica);
}


// Glavni program:
int main() {
	random_device rd;					// Obezbedjuje nedeterministicko generisanje nasumicnih brojeva od strane generatora pri pokretanju programa.
	default_random_engine gen(rd());	// Generator brojeva.
	uniform_real_distribution<double> distr(0.0, 1.0);	// Za uniformno generisanje nasumicnih realnih brojeva izmedju nule i jedinice.


	double pHA, pHB, pHC;				// Apriorne verovatnoce.
	int niz[DUZ_NIZA], redniBroj = 0;

	cout << "Unesite inicijalnu apriornu verovatnocu da je signal poslat sa mesta A: ";  cin >> pHA;
	cout << "Unesite inicijalnu apriornu verovatnocu da je signal poslat sa mesta B: ";  cin >> pHB;
	cout << "Unesite inicijalnu apriornu verovatnocu da je signal poslat sa mesta C: ";  cin >> pHC;
	cout << endl << endl;

	while (1) {
		int brJedinica = 0;

		// Generisanje binarnog niza od deset znakova, i pamcenje broja jedinica u tom nizu:
		for (int i = 0; i < DUZ_NIZA; i++) {
			if (distr(gen) > 0.5) {	// Ukoliko je nasumicno generisan realan broj izmedju 0.5 i 1, smatramo da je tekuci znak niza jedinica.
				niz[i] = 1;
				brJedinica++;
			}
			else niz[i] = 0;		// U suprotnom, smatramo da je tekuci znak niza nula.
		}

		// Uslovne verovatnoce potrebne za formulu totalne verovatnoce (binomna raspodela je u pitanju):
		double pDlHA = izrUslovnuVrv(brJedinica, pA);	
		double pDlHB = izrUslovnuVrv(brJedinica, pB);
		double pDlHC = izrUslovnuVrv(brJedinica, pC);

		// Formula totalne verovatnoce:
		double pD = pHA*pDlHA + pHB*pDlHB + pHC*pDlHC;	

		// Aposteriorne verovatnoce tekuce iteracije (Bajesova formula):
		double pHAlD = pHA * pDlHA / pD;
		double pHBlD = pHB * pDlHB / pD;
		double pHClD = pHC * pDlHC / pD;

		// Ispis tekuce iteracije:
		cout << "|  RB: " << setw(2) << setfill('0') << redniBroj << "  |  "
			<< "Broj jedinica: " << setw(2) << setfill('0') << brJedinica << "  |  "
			<< "Apost. vrv. A: " << fixed << showpoint << setprecision(4) << pHAlD << "  |  "
			<< "Apost. vrv. B: " << fixed << showpoint << setprecision(4) << pHBlD << "  |  "
			<< "Apost. vrv. C: " << fixed << showpoint << setprecision(4) << pHClD << "  |  " << endl;

		// Provera uslova kraja iteriranja:
		if (pHAlD > 0.99) { cout << "\nSignal je sa mesta A.\n\n"; return 0; }
		if (pHBlD > 0.99) { cout << "\nSignal je sa mesta B.\n\n"; return 0; }
		if (pHClD > 0.99) { cout << "\nSignal je sa mesta C.\n\n"; return 0; }

		// Priprema za narednu iteraciju:
		redniBroj++;
		pHA = pHAlD;  pHB = pHBlD;  pHC = pHClD;  // Aposteriorne verovatnoce postaju apriorne u narednoj iteraciji.
	}

	return 0;
}