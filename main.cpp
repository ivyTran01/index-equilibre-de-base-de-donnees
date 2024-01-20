#include <iostream>
#include "BPlusTree.h"
using namespace std;

int main() {
	cout << "Hello World" << endl;

	BPlusTree* tree = new BPlusTree(3);
	tree->printValues();

	/*tree->insert(2, "two");
	tree->insert(21, "twenty1");
	tree->insert(11, "eleven");

	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(8, "eight");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(64, "sixtyfour");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(5, "five");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(23, "twenty three");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(6, "six");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(9, "nine");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(19, "nineteen");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(7, "seven");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(31, "thirtyone");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(39, "thirtynine");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(45, "fourtyfive");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(51, "fiftyone");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(60, "sixty");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(97, "ninety-seven");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(17, "seventeen");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(16, "sixteen");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(15, "fifteen");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(20, "twenty");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(22, "twentytwo");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(12, "twelve");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->insert(13, "thirteen");
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;*/


	tree->insert(2, "two");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(21, "twentyone");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(11, "eleven");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(8, "eight");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(64, "sixty-four");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(5, "five");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(23, "twenty-three");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(6, "six");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(19, "nineteen");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(9, "nine");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(7, "seven");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(45, "fourty-five");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(60, "sixty");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(93, "ninety-three");
	tree->printKeys();
	cout << endl << endl;



	tree->insert(39, "thirty-nine");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(51, "fifty-one");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(31, "thirtyone");
	tree->printKeys();
	cout << endl << endl;

	tree->insert(77, "seventy-seven");
	tree->printKeys();
	cout << endl << endl;

	tree->printValues();



	////////////// TEST REMOVAL ///////////////////////
/*	tree->remove(19);
	tree->printKeys();
	cout << endl << endl;*/

/*	tree->remove(45);
	tree->printKeys();
	cout << endl << endl;*/

	tree->remove(9);
	tree->printKeys();
	cout << endl << endl;

	tree->remove(6);
	tree->printKeys();
	cout << endl << endl;

	tree->remove(8);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(23);
	tree->printKeys();
	cout << endl << endl;

	tree->remove(31);
	tree->printKeys();
	cout << endl << endl;

	tree->remove(77);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(11);
	tree->remove(19);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(5);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(39);
	tree->remove(45);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(2);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(60);
	tree->remove(93);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(21);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(7);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(51);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;

	tree->remove(64);
	tree->printKeys();
	tree->printValues();
	cout << endl << endl;


	return 0;
}
