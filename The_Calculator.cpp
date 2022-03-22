#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

enum class TipElement { null, cifra, operatie, pds, pdd, prs, prd };

enum class TipOperatie { null, adunare, scadere, inmultire, impartire, putere, radacina };

class Expresie {
private:
	char* ecuatie;
	string buffer;
	const TipElement tipElm;
	const TipOperatie tipOp;
public:
	Expresie() :tipElm(TipElement::null), tipOp(TipOperatie::null) {
		ecuatie = nullptr;
		buffer = "";
	}

	Expresie(const char* ecuatie, string buffer, TipElement te, TipOperatie to) : tipElm(te), tipOp(to) {
		if (ecuatie != nullptr) {
			this->ecuatie = new char[strlen(ecuatie) + 1];
			strcpy_s(this->ecuatie, strlen(ecuatie) + 1, ecuatie);
		}
		else
			this->ecuatie = nullptr;
		this->buffer = buffer;
	}

	Expresie(const Expresie& e) :tipElm(e.tipElm), tipOp(e.tipOp) {
		if (e.ecuatie != nullptr) {
			ecuatie = new char[strlen(e.ecuatie) + 1];
			strcpy_s(ecuatie, strlen(e.ecuatie) + 1, e.ecuatie);
		}
		else
			ecuatie = nullptr;
		buffer = e.buffer;
	}

	Expresie& operator=(const Expresie& e) {
		if (this != &e) {
			if (ecuatie != nullptr) {
				delete[] ecuatie;
				ecuatie = nullptr;
			}
			if (e.ecuatie != nullptr) {
				ecuatie = new char[strlen(e.ecuatie) + 1];
				strcpy_s(ecuatie, strlen(e.ecuatie) + 1, e.ecuatie);
			}
			else
				ecuatie = nullptr;
			buffer = e.buffer;
		}
		return *this;
	}

	~Expresie() {
		if (ecuatie != nullptr) {
			delete[] ecuatie;
			ecuatie = nullptr;
		}
	}

	char* getEcuatie() {
		if (ecuatie != nullptr) {
			char* copy = new char[strlen(ecuatie) + 1];
			strcpy_s(copy, strlen(ecuatie) + 1, ecuatie);
			return copy;
		}
		else
			return nullptr;
	}

	void setEcuatie(const char* ecuatie) {
		if (ecuatie != nullptr) {
			if (this->ecuatie != nullptr)
				delete[] this->ecuatie;
			this->ecuatie = new char[strlen(ecuatie) + 1];
			strcpy_s(this->ecuatie, strlen(ecuatie) + 1, ecuatie);
		}
		else {
			ecuatie = nullptr;
		}
	}

	string getBuffer() {
		return buffer;
	}

	void setBuffer(string s) {
		buffer = s;
	}

	TipElement getTipElement() {
		return tipElm;
	}

	TipOperatie getTipOperatie() {
		return tipOp;
	}

	friend istream& operator>>(istream& in, Expresie& e);
	
	friend ostream& operator<<(ostream& out, Expresie& e);

	void operator()() {
		string e = getEcuatie();
		if (e.length() > 0) {
			if (e[0] == '-')
				e = "0" + e;
			for (int i = 1; i < e.length(); i++)
				if (e[i] == '-')
					if (e[i - 1] == '[' || e[i - 1] == '(')
						e.insert(i, "0");
			buffer = e;
			Expresie::setEcuatie(buffer.c_str());
		}
	}

	bool operator!() {
		return buffer != "";
	}

	void stergereSpatii() {
		string e = getEcuatie();
		for (int i = e.length(); i > -1; i--)
			if (e[i] == ' ')
				e.erase(i, 1);
		buffer = e;
		Expresie::setEcuatie(buffer.c_str());
	}

	TipElement tipCaracter(char c) {
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '#')
			return TipElement::operatie;
		else if (isdigit(c) || c == '.')
			return TipElement::cifra;
		else if (c == '[')
			return TipElement::pds;
		else if (c == ']')
			return TipElement::pdd;
		else if (c == '(')
			return TipElement::prs;
		else if (c == ')')
			return TipElement::prd;
		else
			return TipElement::null;
	}

	TipOperatie tipOperatie(char c) {
		if (c == '+')
			return TipOperatie::adunare;
		else if (c == '-')
			return TipOperatie::scadere;
		else if (c == '*')
			return TipOperatie::inmultire;
		else if (c == '/')
			return TipOperatie::impartire;
		else if (c == '^')
			return TipOperatie::putere;
		else if (c == '#')
			return TipOperatie::radacina;
		else
			return TipOperatie::null;
	}
};

istream& operator>>(istream& in, Expresie& e) {
	in >> ws;
	getline(in, e.buffer);
	if (e.ecuatie != nullptr)
		delete[] e.ecuatie;
	e.ecuatie = new char[e.buffer.length() + 1];
	e.setEcuatie(e.buffer.c_str());
	return in;
}

ostream& operator<<(ostream& out, Expresie& e) {
	out << "Ecuatia scrisa: " << e.ecuatie << endl;
	out << "Bufferul (acelasi cu ecuatia): " << e.buffer << endl;
	return out;
}

class StivaOperatori {
private:
	char* stivaOperatori;
	int dimSo;
public:
	StivaOperatori() {
		stivaOperatori = nullptr;
		dimSo = 0;
	}

	StivaOperatori(const char* c, int dim) {
		if (c != nullptr && dim > 0) {
			stivaOperatori = new char[dim];
			for (int i = 0; i < dim; i++)
				stivaOperatori[i] = c[i];
			dimSo = dim;
		}
		else {
			stivaOperatori = nullptr;
			dimSo = 0;
		}
	}

	StivaOperatori(const StivaOperatori& so) :StivaOperatori() {
		if (so.stivaOperatori != nullptr && so.dimSo > 0) {
			stivaOperatori = new char[so.dimSo];
			for (int i = 0; i < so.dimSo; i++)
				stivaOperatori[i] = so.stivaOperatori[i];
			dimSo = so.dimSo;
		}
	}

	StivaOperatori& operator=(const StivaOperatori& so) {
		if (this != &so) {
			if (stivaOperatori != nullptr) {
				delete[] stivaOperatori;
				stivaOperatori = nullptr;
			}
			if (so.stivaOperatori != nullptr && so.dimSo > 0) {
				stivaOperatori = new char[so.dimSo];
				for (int i = 0; i < so.dimSo; i++)
					stivaOperatori[i] = so.stivaOperatori[i];
				dimSo = so.dimSo;
			}
			else {
				stivaOperatori = nullptr;
				dimSo = 0;
			}
		}
		return *this;
	}

	~StivaOperatori() {
		if (stivaOperatori != nullptr) {
			delete[] stivaOperatori;
			stivaOperatori = nullptr;
		}
	}

	char* getStivaOperatori() {
		if (stivaOperatori != nullptr && dimSo > 0) {
			char* copie = new char[dimSo];
			for (int i = 0; i < dimSo; i++)
				copie[i] = stivaOperatori[i];
			return copie;
		}
		else
			return nullptr;
	}

	void setStivaOperatori(char* so, int dim) {
		if (so != nullptr && dim > 0) {
			delete[] stivaOperatori;
			if (stivaOperatori != nullptr)
				delete[] stivaOperatori;
			stivaOperatori = new char[dim];
			for (int i = 0; i < dim; i++)
				stivaOperatori[i] = so[i];
		}
		else {
			stivaOperatori = nullptr;
			dimSo = 0;
		}
	}

	int getDimSo() {
		return dimSo;
	}

	void setDimSo(int dim) {
		dimSo = dim;
	}

	friend istream& operator>>(istream& in, StivaOperatori& so);

	friend ostream& operator<<(ostream& out, StivaOperatori& so);

	StivaOperatori& operator+=(char c) {
		if (stivaOperatori != nullptr && dimSo > 0) {
			char* copie = new char[dimSo];
			for (int i = 0; i < dimSo; i++)
				copie[i] = stivaOperatori[i];
			delete[] stivaOperatori;
			dimSo += 1;
			stivaOperatori = new char[dimSo];
			for (int i = 0; i < dimSo; i++)
				if (i < dimSo - 1)
					stivaOperatori[i] = copie[i];
				else if (i < dimSo)
					stivaOperatori[i] = c;
			delete[] copie;
		}
		else {
			dimSo = 1;
			stivaOperatori = new char[dimSo];
			stivaOperatori[0] = c;
		}
		return *this;
	}

	StivaOperatori operator++() {
		dimSo++;
		return *this;
	}

	char popSo() {
		if (stivaOperatori != nullptr && dimSo > 0) {
			char* copie = new char[dimSo];
			for (int i = 0; i < dimSo; i++)
				copie[i] = stivaOperatori[i];
			delete[] stivaOperatori;
			char op = copie[dimSo - 1];
			dimSo = dimSo - 1;
			stivaOperatori = new char[dimSo];
			for (int i = 0; i < dimSo; i++)
				stivaOperatori[i] = copie[i];
			return op;
		}
		else
			throw exception("Underflow: stiva nu mai contine operatori.");
	}

	char operatorVarf() {
		return stivaOperatori[dimSo - 1];
	}
};

istream& operator>>(istream& in, StivaOperatori& so) {
	in >> so.dimSo;
	if (so.stivaOperatori != nullptr)
		delete[] so.stivaOperatori;
	so.stivaOperatori = new char[so.dimSo];
	for (int i = 0; i < so.dimSo; i++) {
		char c = '\0';
		in >> c;
		so.stivaOperatori[i] = c;
	}
	return in;
}

ostream& operator<<(ostream& out, StivaOperatori& so) {
	for (int i = 0; i < so.dimSo; i++)
		out << so.stivaOperatori[i] << " ";
	out << endl;
	out << so.dimSo << endl;
	return out;
}

class StivaTermeni {
private:
	double* stivaTermeni;
	int dimSt;
public:
	StivaTermeni() {
		stivaTermeni = nullptr;
		dimSt = 0;
	}

	StivaTermeni(const double* stivaTermeni, int dimSt) {
		if (stivaTermeni != nullptr && dimSt > 0) {
			if (this->stivaTermeni != nullptr)
				delete[] this->stivaTermeni;
			this->stivaTermeni = new double[dimSt];
			for (int i = 0; i < dimSt; i++) {
				this->stivaTermeni[i] = stivaTermeni[i];
			}
			this->dimSt = dimSt;
		}
		else {
			this->stivaTermeni = nullptr;
			this->dimSt = 0;
		}
	}

	StivaTermeni(const StivaTermeni& st) :StivaTermeni() {
		if (st.stivaTermeni != nullptr && st.dimSt > 0) {
			stivaTermeni = new double[st.dimSt];
			for (int i = 0; i < st.dimSt; i++)
				stivaTermeni[i] = st.stivaTermeni[i];
			dimSt = st.dimSt;
		}
	}

	StivaTermeni& operator=(const StivaTermeni& st) {
		if (this != &st) {
			if (stivaTermeni != nullptr) {
				delete[] stivaTermeni;
				stivaTermeni = nullptr;
			}
			if (st.stivaTermeni != nullptr && st.dimSt > 0) {
				stivaTermeni = new double[st.dimSt];
				for (int i = 0; i < st.dimSt; i++)
					stivaTermeni[i] = st.stivaTermeni[i];
				dimSt = st.dimSt;
			}
			else {
				stivaTermeni = nullptr;
				dimSt = st.dimSt;
			}
		}
		return *this;
	}

	~StivaTermeni() {
		if (stivaTermeni != nullptr && dimSt > 0) {
			delete[] stivaTermeni;
			stivaTermeni = nullptr;
		}
	}

	double* getStivaTermeni() {
		return stivaTermeni;
	}

	void setStivaTermeni(double* st, int dim) {
		if (st != nullptr && dim > 0) {
			delete[] stivaTermeni;
			stivaTermeni = new double[dim];
			for (int i = 0; i < dim; i++)
				stivaTermeni[i] = st[i];
			dimSt = dim;
		}
		else {
			stivaTermeni = nullptr;
			dimSt = 0;
		}
	}

	int getDimSt() {
		return dimSt;
	}

	void setDimSt(int dim) {
		if (dim > 0) {
			dimSt = dim;
		}
		else
			dimSt = 0;
	}

	friend istream& operator>>(istream& in, StivaTermeni& st);

	friend ostream& operator<<(ostream& out, StivaTermeni& st);

	double& operator[](int index) {
		if (index >= 0 && index < dimSt && stivaTermeni != nullptr)
			return stivaTermeni[index];
		else if (index > dimSt && stivaTermeni != nullptr)
			throw exception("Overflow: stiva de termeni este prea mica.");
		else if (index < 0)
			throw exception("Underflow: stiva nu mai contine termeni.");
	}

	StivaTermeni& operator--() {
		if (stivaTermeni != nullptr && dimSt > 0) {
			double* copie = new double[dimSt];
			for (int i = 0; i < dimSt; i++)
				copie[i] = stivaTermeni[i];
			delete[] stivaTermeni;
			dimSt--;
			stivaTermeni = new double[dimSt];
			for (int i = 0; i < dimSt; i++)
				stivaTermeni[i] = copie[i];
			delete[] copie;
			return *this;
		}
		else {
			throw exception("Underflow: stiva nu mai contine termeni.");
		}
	}

	void pushSt(double val) {
		if (stivaTermeni != nullptr && dimSt > 0) {
			double* copie = new double[dimSt];
			for (int i = 0; i < dimSt; i++)
				copie[i] = stivaTermeni[i];
			delete[] stivaTermeni;
			dimSt++;
			stivaTermeni = new double[dimSt];
			for (int i = 0; i < dimSt; i++) {
				if (i < dimSt - 1)
					stivaTermeni[i] = copie[i];
				else if (i = dimSt - 1)
					stivaTermeni[i] = val;
			}
			delete[] copie;
		}
		else {
			dimSt = 1;
			stivaTermeni = new double[dimSt];
			stivaTermeni[dimSt - 1] = val;
		}
	}

	double termenVarf() {
		return stivaTermeni[dimSt - 1];
	}
};

istream& operator>>(istream& in, StivaTermeni& st) {
	in >> st.dimSt;
	if (st.stivaTermeni != nullptr)
		delete[] st.stivaTermeni;
	st.stivaTermeni = new double[st.dimSt];
	for (int i = 0; i < st.dimSt; i++)
		in >> st.stivaTermeni[i];
	return in;
}

ostream& operator<<(ostream& out, StivaTermeni& st) {
	for (int i = 0; i < st.dimSt; i++) {
		out << st.stivaTermeni[i] << " ";
	}
	out << endl;
	out << st.dimSt << endl;
	return out;
}

class Rezolvare {
	static double rezultat;
public:
	Rezolvare() {
		rezultat = NULL;
	};

	Rezolvare(double rez) {
		Rezolvare::rezultat = rez;
	};

	double getRezultat() {
		return rezultat;
	}

	void setRezultat(double r) {
		Rezolvare::rezultat = r;
	}

	friend istream& operator>>(istream& in, Rezolvare& r);

	friend ostream& operator<<(ostream& out, Rezolvare& r);

	bool operator==(Rezolvare r) {
		return rezultat == r.rezultat;
	}

	bool operator!=(Rezolvare r) {
		return rezultat != r.rezultat;
	}

	int ordineOperatii(Expresie& e, char c) {
		if (e.tipOperatie(c) == TipOperatie::putere || e.tipOperatie(c) == TipOperatie::radacina)
			return 1;
		else if (e.tipOperatie(c) == TipOperatie::inmultire || e.tipOperatie(c) == TipOperatie::impartire)
			return 2;
		else if (e.tipOperatie(c) == TipOperatie::adunare || e.tipOperatie(c) == TipOperatie::scadere)
			return 3;
		else return 4;
	}

	double implementareOperatii(Expresie& e, StivaOperatori& so, StivaTermeni& st) {
		double y = st[st.getDimSt() - 1]; --st;
		double x = st[st.getDimSt() - 1]; --st;
		char c = so.popSo();
		if (e.tipOperatie(c) == TipOperatie::impartire && y == 0) //x/0
			throw exception("Input invalid: nu se poate efectua impartirea la 0.");
		if (e.tipOperatie(c) == TipOperatie::adunare)
			return x + y;
		else if (e.tipOperatie(c) == TipOperatie::scadere)
			return x - y;
		else if (e.tipOperatie(c) == TipOperatie::inmultire)
			return x * y;
		else if (e.tipOperatie(c) == TipOperatie::impartire)
			return x / y;
		else if (e.tipOperatie(c) == TipOperatie::putere)
			return pow(x, y);
		else if (e.tipOperatie(c) == TipOperatie::radacina) {
			if (y < 2) //x#1
				throw exception("Input invalid: nu se poate extrage o radical de ordin mai mic decat 2.");
			if (x < 0) { //(-x)#y
				if ((int)y % 2 == 0 && (int)y == y) //(-x)#par
					throw exception("Input invalid: nu se poate extrage radical de ordin par dintr-un numar negativ.");
				if ((int)y % 2 == 1 && (int)y == y) //(-x)#impar
					return (-1) * pow(-x, 1 / y);
			}
			if ((int)y != y) //x#rational
				throw exception("Input invalid: ordinul radicalului trebuie sa fie numar natural.");
			return pow(x, 1 / y);
		}
	}

	static void rezolvare(Expresie& e, Rezolvare& r) {
		e();
		e.stergereSpatii();
		StivaOperatori so;
		StivaTermeni st;
		int nrPds = 0, nrPdd = 0, nrPrd = 0, nrPrs = 0;
		for (int i = 0; i < strlen(e.getEcuatie()); i++) {
			if (e.tipCaracter(e.getEcuatie()[i]) == TipElement::null) //alte caractere
				throw exception("Input invalid: contine caractere care nu pot fi procesate.");
			else if (e.tipCaracter(e.getEcuatie()[i]) == TipElement::cifra) {
				string s = "";
				while (i < strlen(e.getEcuatie()) && (e.tipCaracter(e.getEcuatie()[i]) == TipElement::cifra || e.getEcuatie()[i] == '.')) {
					s += e.getEcuatie()[i];
					i++;
				}
				st.pushSt(stod(s));
				s = "";
				i--;
			}
			else if (e.tipCaracter(e.getEcuatie()[i]) == TipElement::operatie) {
				if (e.tipCaracter(e.getEcuatie()[i + 1]) == TipElement::operatie) //operatieoperatie
					throw exception("Input invalid: contine operatori adiacenti.");
				while (so.getDimSo() > 0 && r.ordineOperatii(e, so.operatorVarf()) <= r.ordineOperatii(e, e.getEcuatie()[i]))
					st.pushSt(r.implementareOperatii(e, so, st));
				so += e.getEcuatie()[i];
			}
			else if (e.tipCaracter(e.getEcuatie()[i]) == TipElement::pds) {
				nrPds++;
				if (nrPds - nrPdd > 1) //[[]]
					throw exception("Input invalid: paranteze de acelasi tip nu se pot gasi pe doua niveluri diferite.");
				so += e.getEcuatie()[i];
			}
			else if (e.tipCaracter(e.getEcuatie()[i]) == TipElement::pdd) {
				nrPdd++;
				while (so.getDimSo() > 0 && so.operatorVarf() != '[')
					st.pushSt(r.implementareOperatii(e, so, st));
				if (so.getDimSo() > 0)
					so.popSo();
			}
			else if (e.tipCaracter(e.getEcuatie()[i]) == TipElement::prs) {
				nrPrs++;
				if (nrPrs - nrPrd > 1) //(())
					throw exception("Input invalid: paranteze de acelasi tip nu se pot gasi pe doua niveluri diferite.");
				so += e.getEcuatie()[i];
			}
			else if (e.tipCaracter(e.getEcuatie()[i]) == TipElement::prd) {
				nrPrd++;
				while (so.getDimSo() > 0 && so.operatorVarf() != '(')
					st.pushSt(r.implementareOperatii(e, so, st));
				if (so.getDimSo() > 0)
					so.popSo();
			}
		}
		if (nrPdd != nrPds || nrPrd != nrPrs)
			throw exception("Input invalid: numarul de paranteze deschise nu corespunde cu numarul celor inchise.");
		while (so.getDimSo() > 0)
			st.pushSt(r.implementareOperatii(e, so, st));
		r.rezultat = st[st.getDimSt() - 1];
	}

	unsigned int precizieRezultat(Rezolvare& r) {
		string s = to_string(r.getRezultat());
		if (s.find(".") > 0) {
			int i = s.length() - 1;
			while (i > s.find(".")) {
				if (s.substr(s.length() - 1, 1) == "0")
					s.erase(s.length() - 1);
				i--;
			}
		}
		unsigned int numarZecimale = (s.substr(s.find(".") + 1)).length();
		return numarZecimale;
	}
};

double Rezolvare::rezultat = 0;

istream& operator>>(istream& in, Rezolvare& r) {
	in >> r.rezultat;
	return in;
}

ostream& operator<<(ostream& out, Rezolvare& r) {
	out << r.rezultat;
	return out;
}

int main() {
	Expresie input;
	Rezolvare output;
	cin >> input;
	while (input.getBuffer() != "exit") {
		try {
			Rezolvare::rezolvare(input, output);
			cout << fixed << setprecision(output.precizieRezultat(output)) << output << endl;
		}
		catch (exception& e) {
			cout << e.what() << endl;
		}
		cin >> input;
	}
}