
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


