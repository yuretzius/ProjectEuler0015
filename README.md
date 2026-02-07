# ProjectEuler0015
My work on 

problem #15 of projecteuler.net:

Lattice Paths

Starting in the top left corner of a $2 \times 2$ grid, and only being able to move to the right and down, there are exactly $6$ routes to the bottom right corner.

![problem 15 image](0015.png)

How many such routes are there through a $20 \times 20$ grid?

Completed on Wed, 24 Feb 2021, 03:43

#######################################################################################
Please note: Project Euler's policy allows publication of solutions for the first 100 problems,
that's why I am sharing my work here for reference and educational purposes.
#######################################################################################

OK, so you always have to go $n$ times right and $n$ times down. So total is always $n+n$, and the rights
(and, correspondngly, downs) can be in any possible combination. Which is the very definition of $\binom{n+n}{n}$.

In python I just used scipy.special.binom, which in this case is the same as scipy.special.comb (in general they
are different, see [SciPy documentation on binom](https://docs.scipy.org/doc/scipy/reference/generated/scipy.special.binom.html)).

In C++ I solved a more complicated problem given in Hackerrank, namely calculating the number of combinations for
much larger numbers which would overflow unsigned long when directly multiplied. The answer is calculated
modulo large prime $M$ (e.g. $1000000007$).

I implemented directly the usual formula for $\binom{n}{m}$:

$$\binom{n}{m} = \frac{(n - m + 1)(n - m + 2)\ldots n}{m!}$$.

But instead of multiplying each term in the numerator and denominator I make use of the fact that by design
the denominator must cancel out completely. So I factorize the denominator into primes and then run through each
term in the numerator and divide it by as many as possible factors in the denominator. 
Then the leftover terms of the numerator are multiplied by each other, doing modulo $M$ operation
every time and ensuring that there is no overflow this way.

The factorizing machinery is taken from problems 9 and 10, only slightly rewritten, so
instead of returning separately unique prime factors with corresponding multiplicities it repeats the prime factors
the necessary number of times, e.g. for $24$ it returns $2$, $2$, $2$, $3$.

