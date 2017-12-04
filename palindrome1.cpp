#include <iostream>
#include <string>

using namespace std;

string removeChar(string);
bool isPal(string&, int, int);

int main()
{
	string testWord;
	string newWord;
	int first = 0;
	int last = string::npos;

	cout << "Palindrome Detection Program" << endl;
	cout << "Enter a word or sentence, and the program will tell whether or not it is a palindrome." << endl;

	getline(cin, testWord);

	newWord = removeChar(testWord);

	cout << "Your entry " << (isPal(newWord, first, last) ? "is a palindrome." : "is not a palindrome.") << endl;
	

	cin.get();

	return 0;
}

string removeChar(string testWord)
{
	string newWord;

	for (unsigned int c = 0; c < testWord.length(); c++)
	{
		if (isalnum(testWord[c]))
		{
			newWord += testWord[c];
		}

	}

	return newWord;
}

bool isPal(string& newWord, int first = 0, int last = string::npos)
{

	if (last == string::npos)
	{
		last = (newWord.length()-1);
	}
	if (newWord[first] == newWord[last])
	{
		if ((first-last) == 0)
		{
			return true;
		}
		else if (first == (last - 1))
		{
			return true;
		}
		else
		{
			return isPal(newWord, first+1, last-1);
		}
	}
	else
	{
		return false;
	}
}