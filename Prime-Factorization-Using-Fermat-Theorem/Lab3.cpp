#include <gmp.h>
#include <iostream>
using namespace std;

void Fermat(mpz_t n, mpz_t p1, mpz_t p2){
	cout << "Factorizing N using Fermat's Method..." << endl;
	mpz_t root;
	mpz_init(root);
	//get the square root of n
	int ifExact = mpz_root(root, n, 2);
	if(ifExact == 0){//Judge whether t^2 < n or not
		mpz_add_ui(root, root, 1);//make t^2 > n
	}
	while(mpz_cmp(root, n) < 0){
		mpz_t w;
		mpz_init(w);
		mpz_mul (w, root, root);//w = t*t
		mpz_sub (w, w, n);      //w = t*t - n

		mpz_t s;
		mpz_init(s);
		int ifPerfectSquare = mpz_root(s, w, 2);//s = sqrt(w)
		if(ifPerfectSquare != 0){//Judge whether w is a perfect square
			mpz_t p;
			mpz_init(p);
			mpz_t q;
			mpz_init(q);

			mpz_add (p, root, s);//p = t + s
			mpz_sub (q, root, s);//q = t - s
			//Obtain the final results
			mpz_set (p1, p);//p1 = p
			mpz_set (p2, q);//p2 = q
			
			cout << "Factorization done" << endl;
			return;
		}
		
		mpz_add_ui(root, root, 1);//t++
	}
}

int main(){
	cout << "Initializing..." << endl;
	mpz_t N;
	//Initialize the N value
	mpz_init_set_str(N, 
	"648455842808071669662824265346772278726343720706976263060439070378797308618081116462714015276061417569195587321840254520655424906719892428844841839353281972988531310511738648965962582821502504990264452100885281673303711142296421027840289307657458645233683357077834689715838646088239640236866252211790085787877", 10);
//"179769313486231590772930519078902473361797697894230657273430081157732675805505620686985379449212982959585501387537164015710139858647833778606925583497541085196591615128057575940752635007475935288710823649949940771895617054361149474865046711015101563940680527540071584560878577663743040086340742855278549092581", 10);
	mpz_t f1, f2;
	mpz_init(f1);
	mpz_init(f2);

	//Factorization of N using Fermat's method
	Fermat(N, f1, f2);

	//Print the final results
	cout << "The final P is:" << endl;
	gmp_printf("%Zd\n", f1);
	cout << "The final Q is:" << endl;
	gmp_printf("%Zd\n", f2);

	return 0;
}
