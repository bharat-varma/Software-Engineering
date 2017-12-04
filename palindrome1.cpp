 varma

/* I managed to do both the palindrome and checking the file parts individually but could upload just the palindrome one. I have included
 the WordCheck part just to show that I did it. I have commented out the code in main so that it doesn't affect output of earlier code
 Be Kind */ 
 master
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
 varma


	
	/* WordCheck("input.txt", "need"); */
	
 master
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
 varma
}

}

bool WordCheck(char* filename, char* search)
{

/* List of words saved on file named input.txt */
    int offset; 
    string line;
    ifstream Myfile;
    Myfile.open (filename);

    if(Myfile.is_open())
    {
        while(!Myfile.eof())
        {
            getline(Myfile,line);
            if ((offset = line.find(search, 0)) != string::npos) 
            {
             cout << "Word found in list '" << search << " \n\n"<< line  <<endl;
             return true;
            }
           
        }
        Myfile.close();
    }
    else
    cout<<"Unable to open this file."<<endl;

    return false;
}

 master
