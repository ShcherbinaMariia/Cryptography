#include <vector>
#include <complex>

void deleteLeadingZeros(std::vector<int>& a){
	for(int i = a.size() - 1; i>0 && a[i]==0; i--){
		a.pop_back();
	}
}

void fft(std::vector<std::complex<double>>& a, bool inverse = false){
	if (a.size() == 1) return;
	int n =  a.size();
	std::vector<std::complex<double>> a0, a1;

	for (int i = 0; i < n; i+=2)
	{
		a0.push_back(a[i]);
		a1.push_back(a[i + 1]);
	}

	fft(a0, inverse);
	fft(a1, inverse);

	double ang = 2*M_PI/n * (inverse ? -1 : 1);
	std::complex<double> wi(1), w(cos(ang), sin(ang));

	for (int i = 0; i < n/2; i++) {
		a[i] = a0[i] + wi * a1[i];
		a[i + n/2] = a0[i] - wi * a1[i];
		if (inverse){
			a[i] /= 2;
			a[i + n/2] /= 2;
		}
		wi *= w;
	}
}
