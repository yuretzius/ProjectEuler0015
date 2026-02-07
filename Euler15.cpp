#include <ctime>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

unsigned long M = 1000000007; // the result is calulated modulo M

// Erat2 is from problem 10

//the sieve of Eratosthenes, the limit by both memory and time is about 10^10
// Returns the list of primes lower or equal than N
// Only tracks candidates 6k+1 and 6k-1
// Thus should be slightly more memory-efficient 
// (10^10 takes about 92 seconds)

vector<unsigned long> Erat2(unsigned long N) {
    vector<unsigned long> primes;
    if (N < 5) {
        if (N < 2) return primes;
        primes.push_back(2);
        if (N == 2) return primes;
        primes.push_back(3);
        return primes; 
    }
    // only taking care of primes starting from 5 and 7
    // which all are in form 6k - 1 and 6k + 1
    unsigned long top_index = N / 6; // max k
    vector<bool> low_cand (top_index, true); // to track 6k-1 candidates
    vector<bool> high_cand (top_index, true); // to track 6k+1 candidates
    vector<bool>::iterator itr = low_cand.begin();
    unsigned long k = 1;
    unsigned long p = 6*k ; 
    unsigned long p_low, p_high, t, i;
    p_low = p_high = t = i = 0;    
    while ((p-1)*(p-1) <= N) 
    // if even the lower part of the pair <= N, go in the cycle
    {
        // 6*k - 1 sieve
        if (low_cand[k-1]) {
            p_low = p - 1;
            t = p_low * p_low; // start with p*p, as usual
            while (t <= N) { 
                // take action only if there is an index solution
                // for either low or high candidate                
                if ((t + 1) % 6 == 0) {
                    i = (t + 1)/6;
                    itr = low_cand.begin() + i - 1;
                    *itr = false;
                }
                if ((t - 1) % 6 == 0) {
                    i = (t - 1)/6 ;
                    itr = high_cand.begin() + i - 1;
                    *itr = false;
                }
                t += p_low;
            }
        }
        // 6*k + 1 sieve
        if (high_cand[k-1]) {
            p_high = p + 1;
            t = p_high * p_high; // start with p*p, as usual
            while (t <= N) { 
                // take action only if there is an index solution
                // for either low or high candidate                
                if ((t + 1) % 6 == 0) {
                    i = (t + 1)/6;
                    itr = low_cand.begin() + i - 1;
                    *itr = false;
                }
                if ((t - 1) % 6 == 0) {
                    i = (t - 1)/6;
                    itr = high_cand.begin() + i - 1;
                    *itr = false;
                }
                t += p_high; 
            }                
        }       
        k ++; // go to the next pair
        p = 6*k; 
    }
    primes.push_back(2);
    primes.push_back(3);
    for (i = 1; i != top_index+1; i++) {
        // convert the candidate arrays into actual numbers
        if (low_cand[i-1])  {primes.push_back(6*i - 1);}
        if (high_cand[i-1]) {primes.push_back(6*i + 1);}
    }
    if (*(primes.end() - 1) > N) 
    // need to do this check since the top of the last pair can cross the threshold
    {
        primes.pop_back(); // remove the last item
    }      
    return primes;
}

// IncompletePrimeFactor is from problem 9

vector<unsigned long> IncompletePrimeFactor(unsigned long N) {
    vector<unsigned long> primes = Erat2((unsigned long)(floor(sqrt((double) N))));
    vector<unsigned long>::iterator itr;
    vector<unsigned long> factors;
    for (itr = primes.begin(); itr != primes.end(); itr ++) {
        if (N % *itr == 0) factors.push_back(*itr);
    }
    return factors;
} 

vector<pair <unsigned long,int>> FactorMultiplicity(unsigned long N){
    vector<unsigned long> factors = IncompletePrimeFactor(N);
    vector<pair <unsigned long,int>> mfactors;
    if (factors.empty()) {
        mfactors.push_back(make_pair(N,1));
        return mfactors;
    }
    vector<unsigned long>::iterator itr;
    int m;
    for (itr = factors.begin(); itr != factors.end(); itr++) {
        m = 0;
        while (N % *itr == 0) {
            m++;
            N /= *itr;
        }
        mfactors.push_back(make_pair(*itr,m));
    }
    if (N != 1) mfactors.push_back(make_pair(N, 1));
    return mfactors;
}

// Factors is just FactorMultiplicity from problem 9
// but slightly rewritten, instead of keeping factor and multiplicity
// info separate this tracks only factors, but repeats them the necessary
// number of times, e. g. for 12 the result will be 2, 2, 3.
 
vector<unsigned long> Factors(unsigned long N){
    vector<unsigned long> factors = IncompletePrimeFactor(N);
    vector<unsigned long> ffactors;
    if (factors.empty()) {
        ffactors.push_back(N);
        return ffactors;
    }
    vector<unsigned long>::iterator itr;
    for (itr = factors.begin(); itr != factors.end(); itr++) {
        while (N % *itr == 0) {
            ffactors.push_back(*itr);
            N /= *itr;
        }
    }
    if (N != 1) ffactors.push_back(N);
    
    return ffactors;
}

// C_n^m or \binom{n}{m} mod M
// using the formula (n - m + 1) ... n / m!
// but modulo M
// This is intended to work for cases when directly multiplying 
// values will lead to overflowing unsigned long
unsigned long binom_modulo (unsigned long n, unsigned long m, unsigned long M) {
    if (m == 0) return 1;
    if (m == 1) return n % M;
    vector<unsigned long> numerator, denominator, f;
    vector<unsigned long>::iterator itn;
    // cout << "numerator" << endl;
    for (unsigned long i = (n - m + 1); i != n + 1; i++) {       
        numerator.push_back(i);
        // cout << i << ", ";
    }
    // cout << endl;
    // cout << "denominator" << endl;
    for (unsigned long i = 2; i != m+1; i++) {
        f = Factors(i); // deconstructing denominator multipliers into prime factors
        
        for (vector<unsigned long>::iterator t = f.begin(); t != f.end(); t++) {
            denominator.push_back(*t);
            // cout << *t << ", ";
        }
    }
    // cout << endl;
    itn = numerator.begin();
    while (!denominator.empty()) {
        // when possible, divide a multiplier of the numerator by a prime factor from
        // the denominator, and "cross out" the used up factor
        while (!denominator.empty() && *itn % denominator.back() == 0) {
            *itn = *itn/ denominator.back();
            denominator.pop_back(); 
        }  
        if (itn == (numerator.end()-1)) itn = numerator.begin(); // cycle around if necessary
        else itn++; 
    }
    unsigned long prod = 1;
    // at this moment only numerator has any values left and we can finally start multiplying
    // but %M at every step to avoid overflowing
    // cout << "leftover" << endl;
    for (itn = numerator.begin(); itn != numerator.end(); itn++) {
        // cout << *itn << ", ";
        prod *= *itn;
        prod %= M;
    }  
    // cout << endl;
    return prod;
}

int main() {
    int n, m;
    cout << "enter n and m for nCm:" << endl;
    cin >> n >> m;
    clock_t c_start = clock();
    unsigned long res = binom_modulo(n,m,M);
    clock_t c_end = clock();
    cout << "the answer is given modulo " << M << endl;
    cout << res << endl;
    cout << endl;
    cout << 1000.0*(c_end - c_start) / CLOCKS_PER_SEC << " ms\n";
    return 0;
}
