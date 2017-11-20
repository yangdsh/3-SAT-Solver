#include "Twin.h"

typedef std::vector<double> vec;
typedef std::vector<vec> mat;

vec gvec(Twin&);
void dvec(Twin&, const vec&);
mat gmat(Twin&);
void dmat(Twin&, const mat&);
mat buildz(const mat&);
mat matinv(const mat&);
mat mattrans(const mat&);
mat matmult(const mat&, const mat&);
vec matmultvec(const mat&, const vec&);

int main() {

	Twin t("Multiple-Linear Regression");
	mat x;
	vec y;
	int m;

	do {

		t.println("Enter the first row of elements in matrix x.");
		t.println("Example: 2 4 5 2.1");
		t.display();

		x = gmat(t);

		t.println("Enter the first element of the vector y.");
		t.println("Example: 1.2");
		t.display();

		y = gvec(t);

		if (x.size() != y.size()) {
			t.println("Vector must have same dimension as matrix.");
			t.println();
		}

	} while (x.size() != y.size());

	const mat z = buildz(x);
	const mat zt = mattrans(z);
	const vec a = matmultvec(matinv(matmult(zt, z)), matmultvec(zt, y));
	const vec yf = matmultvec(z, a);

	t.println("Matrix x:");
	t.println();
	dmat(t, x);
	t.println();
	t.println("Press enter to continue.");
	t.display();
	std::cin.ignore(1000, '\n');
	std::cin.clear();

	t.println("Vector y:");
	t.println();
	dvec(t, y);
	t.println();
	t.println("Press enter to continue.");
	t.display();
	std::cin.ignore(1000, '\n');
	std::cin.clear();

	t.println("Vector a:");
	t.println();
	dvec(t, a);
	t.println();
	t.println("Press enter to continue.");
	t.display();
	std::cin.ignore(1000, '\n');
	std::cin.clear();

	t.println("Vector yf:");
	t.println();
	dvec(t, yf);
	t.println();
	t.println("Press enter to continue.");
	t.display();
	std::cin.ignore(1000, '\n');
	std::cin.clear();

	return EXIT_SUCCESS;
}

vec gvec(Twin& t) {

	vec b;
	double in;
	std::string input;

	std::getline(std::cin, input);
	std::stringstream iss(input);
	iss >> in;
	b.push_back(in);

	while (true) {

		t.println("Vector:");
		t.println();
		dvec(t, b);
		t.println("Enter the next element.");
		t.println("Enter 'x' to save and continue.");
		t.display();

		std::getline(std::cin, input);
		std::stringstream iss(input);
		iss >> in;

		if (input == "x" || input == "X") return b;
		else b.push_back(in);

	}
}

void dvec(Twin& t, const vec& vec) {

	for (auto&& i : vec) {
		t.println("[ ");
		t.print(i);
		t.print(" ]");
	}

	t.println();
}

mat gmat(Twin& t) {

	mat a;
	double in;
	std::string input;

	std::getline(std::cin, input);
	std::stringstream iss(input);
	a.push_back(vec());
	while (iss >> in) a[0].push_back(in);

	for (int i = 1; i > 0; i++) {

		t.println("Matrix:");
		t.println();
		dmat(t, a);
		t.println("Enter the next row.");
		t.println("Enter 'x' to save and continue.");
		t.display();

		std::getline(std::cin, input);
		std::stringstream iss(input);
		if (input == "x" || input == "X") i = -1;

		else {
			a.push_back(vec());
			while (iss >> in) a[i].push_back(in);

			if (a[i].size() != a[0].size()) {
				t.println("Invalid row.");
				a.pop_back();
				i--;
			}
		}
	}
	return a;
}

void dmat(Twin& t, const mat& mat) {

	t.println();

	for (int i = 0; i < int(mat.size()); i++) {

		t.print("[ ");

		for (int j = 0; j < int(mat[0].size()); j++) {
			t.print(mat[i][j]);
			if (j < int(mat[0].size()) - 1) t.print(",");
			t.print(" ");
		}

		t.print("]");
		t.println();

	}
}

mat buildz(const mat& x) {

	mat z(x.size(), vec(x[0].size() + 1));

	for (int i = 0; i < z.size(); i++) {
		z[i][0] = 1.0;
		for (int j = 1; j < z[0].size(); j++) {
				z[i][j] = x[i][j - 1];
			}
	}

	return z;

}

mat matinv(const mat& A) {

	int n = A.size();

	mat l, u(A), Ainv(n);

	for (int i = 0; i < n; i++) {
		l.push_back(vec(n, 0));
		l[i][i] = 1;
	}

	for (int i = 0; i < n; i++) {

		for (int k = i + 1; k < n; k++) {
			l[k][i] = u[k][i] / u[i][i];

			for (int j = i; j < n; j++) u[k][j] += -l[k][i] * u[i][j];

		}
	}

	for (int j = 0; j < n; j++) {

		vec x(n), y(n), b(n, 0);

		b[j] = 1;

		for (int i = 0; i < n; i++) {
			y[i] = b[i] / l[i][i];
			for (int k = 0; k < n; k++) {
				b[k] -= l[k][i] * y[i];
			}
		}

		for (int i = n - 1; i >= 0; i--) {
			x[i] = y[i] / u[i][i];
			for (int k = i - 1; k >= 0; k--) {
				y[k] -= u[k][i] * x[i];
			}
		}

		for (int k = 0; k < n; k++)
			Ainv[k].push_back(x[k]);
	}

	return Ainv;

}

mat mattrans(const mat& a) {

	mat t(a[0].size(), vec(a.size()));

	for (int i = 0; i < a.size(); i++)
		for (int j = 0; j < a[0].size(); j++)
			t[j][i] = a[i][j];

	return t;

}

mat matmult(const mat& a, const mat& b) {

	mat mult(a.size(), vec(b[0].size()));

	for (int i = 0; i < a.size(); i++)
		for (int j = 0; j < b[0].size(); j++)
			for (int k = 0; k < a[0].size(); k++)
				mult[i][j] += a[i][k] * b[k][j];

	return mult;

}

vec matmultvec(const mat& a, const vec& b) {

	vec mult(a.size());

	for (int i = 0; i < a.size(); i++)
		for (int j = 0; j < b.size(); j++)
			mult[i] += a[i][j] * b[j];

	return mult;

}