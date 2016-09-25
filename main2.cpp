#include <iostream>
#include <fstream>

using namespace std;

struct emailNode {
    string name;
    string subject;
    string body;
    emailNode *next;
};

void printEmail(emailNode * node)
{
	cout << node->name << "\t" << node->subject << "\t" << node->body << endl;
}

struct emailList {
    emailNode *first;
    emailNode *last;
    emailList *next;
};

struct fileList {
    emailList *first;
    emailList *last;
};


string before(string s, string b)
{
    return s.substr(0,s.find(b));
}

string after(string s, string b)
{
    return s.substr(s.find(b)+1);
}

bool wildCompare(string s1, string s2)
{
    return (s1.compare(s2) == 0 || s1.compare("G:#wild#") == 0 || s2.compare("G:#wild#") == 0);
}

bool equals(emailNode * n1, emailNode * n2)
{
    return wildCompare(n1->name,n2->name) && wildCompare(n1->subject,n2->subject) && wildCompare(n1->body,n2->body);
}

bool contains(emailList * list, emailNode * node)
{
     emailNode * current = list->first;
     while(current) {
	if(equals(current,node))
	{
	    return true;
	}
	current = current->next;
     }
     return false;
}

bool contains(string s1, string s2)
{
    return (s1.find(s2)< s1.length());
}

void remove(emailList * list, emailNode * n)
{
    emailNode * previous = list->first;
    while(equals(previous,n)){
	list->first = n;
	previous = previous->next;
    }
    emailNode * current = previous->next;
    while(current) {
	if(equals(current,n)) {
	    previous->next = current->next;
	    break;
	}
        previous = current;
        current = current->next;
    }
}

void removeAll(emailList * list, emailNode * n)
{
    emailNode * previous = list->first;
    while(equals(previous,n)){
	list->first = n;
	previous = previous->next;
    }
    emailNode * current = previous->next;
    while(current) {
	if(equals(current,n)) {
	    previous->next = current->next;
	}
        else {
            previous = current;
	}
        current = current->next;
    }
}

void insert(fileList * list, emailList * l)
{
     if(list->first == NULL)
     {
	list->first = l;
	list->last = l;
     }
     else {
	list->last->next = l;
	list->last = l;
     }
}

void insert(emailList * list, string name, string subject, string body)
{
    emailNode *n = new emailNode;
    
    n->name = name;
    n->subject = subject;
    n->body = body;
    n->next = NULL;

    if(list->first == NULL)
    {
		list->first = n;
        list->last = n;
    }
    else
    {
		if(!contains(list, n))
		{
			if(contains(subject,"(Cancelled)"))
			{
				emailNode * remNode = new emailNode;
				remNode->name = name;
				remNode->subject = before(subject, "(Cancelled)");
				remNode->body = body;             

				remove(list, remNode);
    	    }
			else {
				if(subject.compare("Purchase-order-cancellation") == 0) {
					emailNode * remNode = new emailNode;
					remNode->name = "G:#wild#";
					remNode->subject = "G:#wild#";
					remNode->body = body;
		    
					removeAll(list,remNode);
				}
				list->last->next = n;
				list->last = n;
			}
		}
    }
}

void printList(emailList * list)
{
    emailNode * current = list->first;
    while(current) {
        cout << current->name << "\t" << current->subject << "\t" << current->body << endl;
	current = current->next;
    }
}

void printAllLike(emailNode * list)
{
    emailNode * prev = NULL;
    emailNode * current = list;
    string name = current->name;
    string subject = current->subject;
    
    cout << name << "\t" << subject << "\t" << current->body;
    prev = current;
    current = current->next;
    
    while (current) {
	if(current->name.compare(name) == 0 && current->subject.compare(subject) == 0)
	{
	    cout << ", " << current->body;
            prev->next = current->next;
	}
        prev = current;
	current = current->next;
    }
    cout << endl;
}

void addAllLike(emailList * list, emailNode * node)
{
    emailNode * prev = NULL;
    emailNode * current = node;
    string name = current->name;
    string subject = current->subject;

    string ALL = current->body;
    prev = current;
    current = current->next;
    
	while(current) {
		if(current->name.compare(name) == 0 && current->subject.compare(subject) == 0)
		{
			ALL += ", " + current->body;
			prev->next = current->next;
        }
        else {
			prev = current;
		}        
        current = current->next;
    }
    
    insert(list,name,subject,ALL);
}

emailList * getGrouped (emailList * list)
{
    emailList * newlist = new emailList;
    newlist->first = NULL;
    newlist->last = NULL;
    emailNode * current = list->first;
    while(current) {
		addAllLike(newlist,current);
        current = current->next;
    }

    return newlist;
}

void printGrouped(emailList * list)
{
    emailNode * current = list->first;
    while(current) {
		printAllLike(current);
		current = current->next;
    }
}

void interleavePrint(fileList * l, int size)
{
	emailList * finalList = new emailList;
	finalList->first = NULL;
	finalList->last = NULL;
	
	emailNode ** list = new emailNode*[size];
	
	emailList * current = l->first;
	
	for(int i = 0;current;i++) {
		list[i] = current->first;
		current = current->next;
	}
	
	bool repeat = false;
	
	do
	{
		for(int i = 0; i < size; i++)
		{
			if(list[i])
			{
				repeat = false;
				insert(finalList,list[i]->name,list[i]->subject,list[i]->body);
				list[i] = list[i]->next;
			}
		}
	}
	while(repeat);
}

int main(int argc, const char * argv[])
{
    string input = argv[1];
    input = input.substr(6, input.length() - 12);

    fileList * f = new fileList;
    f->first = NULL;
    f->last = NULL;

    int i = 1;

    for(;true; i++)
    {
		ifstream file (input + "." + to_string(i) + ".txt");
		string line = "";

        if(!file.is_open()) break;

		emailList *e;
		e = new emailList;

		e->first = NULL;
		e->last = NULL;

		while(getline(file, line))
		{
			if(line.length() != 0 && line.at(0) != '#')
			{
				string name = before(line,"\t");
				string subject = before(after(line,"\t"), "\t");
				string body = after(after(line, "\t"),"\t");
				insert(e, name, subject, body);
			}
		}
        
        emailList * list = getGrouped(e);
    
		insert(f,list);
    }
    
    interleavePrint(f,i - 1);
}


