//lab-6 processes in round robin scheduling - memory handling added
//author: thakshak gudimetla
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
//#include <chrono>
using namespace std;
int DAEMONS=6;
int DAEMONScnt=6;

class Memory
{
private:
	int max_pages;
	vector<int> pages;//Each element of the vector represents a 1024 byte page of memory.
	//The vector will store 0's and 1's where a 0 indicates a free page and a 1 indicates the page is in use. 
public:
	Memory(int max_pages)
	{
		this->max_pages=max_pages;
		for(int i=0;i<max_pages;i++)
		{
			pages.push_back(0);
		}
	}
	int load(int limit)
	{
		int prev=0,current_index=-1;
		for(int i=0;i<max_pages;i++)
		{
			if(i-prev>=limit)
			{
				current_index=prev;
				break;
			}
			if(pages[i]!=0) prev=i;
		}
		if(current_index!=-1)
		{
			for(int i=0;i<limit;i++)
			{
				pages[current_index++]=1;
			}
			return prev;
		}
		return current_index;
	}
	int free(int base,int limit)
	{
		for(int i=0;i<limit;i++)
		{
			pages[base+i]=0;
		}
	}
};

class Process
{
private:
	int pid ;
	string process_name;
	string state;//(running, wait or ready)
	int ttl ;//(this is the total CPU time needed for the process & -1 for daemons)
	int burst; //(amount of time for the next CPU burst before entering the wait state)
	int ioburst; //(amount of time in wait state for I/O)
	int memory_address;
	int memory_size ;
public:
	Process(int pid, string process_name,int ttl, string state, int burst, int ioburst, int memory_address, int memory_size)
	{
		this->pid=pid;
		this->ttl=ttl;
		this->state=state;
		this->process_name=process_name;
		this->burst=burst;
		this->ioburst=ioburst;
		this->memory_address=memory_address;
		this->memory_size=memory_size;
	};

	//accessor
	int get_pid()
	{
		return this->pid;
	};
	string get_process_name()
	{
		return this->process_name;
	};
	int get_ttl()
	{
		return this->ttl;
	};
	string get_state()
	{
		return this->state;
	};
	int get_burst()
	{
		return this->burst;
	};
	int get_ioburst()
	{
		return this->ioburst;
	};
	int get_memory_size()
	{
		return this->memory_size;
	};
	int get_memory_address()
	{
		return this->memory_address;
	};
	//mutator
	int set_ttl(int val)
	{
		this->ttl=val;
	};
	int set_state(string val)
	{
		this->state=val;
	};
	int set_burst(int val)
	{
		this->burst=val;
	};
	int set_memory_address(int val)
	{
		this->memory_address=val;
	};
	int set_memory_size(int val)
	{
		this->memory_size=val;
	};
	int set_ioburst(int val)
	{
		this->ioburst=val;
	};
	
	int display()
	{
		//display all attributes
		cout<<"*------------------------*"<<endl;
		cout<<"pid:"<<this->pid<<endl;
		cout<<"ttl:"<<this->ttl<<endl;
		cout<<"processes_name:"<<this->process_name<<endl;
		cout<<"state:"<<this->state<<endl;

		cout<<"burst:"<<this->burst<<endl;
		cout<<"ioburst:"<<this->ioburst<<endl;
		cout<<"memory_address:"<<this->memory_address<<endl;
		cout<<"memory_size:"<<this->memory_size<<endl;
		
		
	};
};
/*myRand----------
int rand(long int n)
{
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
  	return (ms.count()%n);
}
*/
int rand(int n)
{
  long  num=time(0)*time(0);
  int num2=num%n +1;
  return num2;
}
int getioburst()
{
 return 500+rand(100);
}

int getburst()
{
 return 5+rand(100);
}
//----------
class ProcessTable
{
private:
	vector<Process > processes;
	int nextpid;
	Memory m=Memory(1024);
public:
	ProcessTable()
	{
		this->nextpid=1000;
	};
	int getnextpid() { return ++nextpid; }

	int add_process(Process newProcess)
	{
		int s=newProcess.get_memory_size();
		int allocated=m.load(s/1024+1);
		if(allocated==-1)
		{
			cout<<"insufficient memory for the new process"<<endl;
			return 1; 
		}
		newProcess.set_memory_address(allocated);
		this->processes.push_back(newProcess);
		return 0;
	};
	int remove_process(int pid)
	{
		for(int i=0; i<processes.size();i++)
		{

			if(processes[i].get_pid()==pid)
			{
				if(pid-1000<=DAEMONScnt && DAEMONS>0)
					DAEMONS--;
				cout<<"deleting process with pid"<<pid<<endl;
				if(processes[i].get_memory_size()!=0)
				{
					m.free(processes[i].get_memory_address(),processes[i].get_memory_size()/1024+1);
				}
				processes.erase(processes.begin()+i);
				cout<<"success...!"<<endl;
				return 0;
			}
		}
		cout<<"processes not found"<<endl;
		return 1;
	};
	int display_processes()
	{
		for(int i=0; i<processes.size();i++)
		{
			processes[i].display();
			cout<<"*------------------------*"<<endl;
		}
		return 0;
	};
	int run(int sec)
	{
		while(1)
		{
			int cnt=0;
			int n=processes.size();
			int time_quantum=1000/n;
			if(time_quantum<10) time_quantum=11;
			int msecs=sec*1000;

			for(int i=0; i<processes.size();i++)
			{
				if(msecs<=0)
				{
					cout<<"time-out"<<endl;
					return 1;
				}
				if(processes[i].get_state()=="ready")
				{
					cout<<processes[i].get_pid()<<" is in ready state"<<endl;
					int temp_burst=processes[i].get_burst()-time_quantum;
					int temp_ttl=processes[i].get_ttl()-time_quantum;
					if(temp_burst<=0)
					{
						msecs=time_quantum-temp_burst;
						temp_burst=0;
						processes[i].set_state("wait");
						processes[i].set_ioburst(getioburst());
					}
					msecs-=time_quantum;
					if(temp_ttl<=0) temp_ttl=0;
					if(processes[i].get_ttl()==-1)temp_ttl=-1;
					processes[i].set_burst(temp_burst);
					processes[i].set_ttl(temp_ttl);
				}
				else if(processes[i].get_state()=="wait" && (processes[i].get_ttl()>0 || processes[i].get_ttl()==-1) )
				{
					cout<<processes[i].get_pid()<<" is in wait state"<<endl;
					int temp_ioburst=processes[i].get_ioburst()-time_quantum;
					int temp_ttl=processes[i].get_ttl()-time_quantum;
					if(temp_ioburst<=0)
					{
						msecs=time_quantum-temp_ioburst;
						processes[i].set_state("ready");
						processes[i].set_burst(getburst());
					}
					msecs-=time_quantum;
					if(temp_ttl<=0) temp_ttl=0;
					if(processes[i].get_ttl()==-1)temp_ttl=-1;
					if(temp_ttl==0)
					{
						cout<<"process with pid "<<processes[i].get_pid()<<" has terminated"<<endl;
						if(processes[i].get_memory_size()!=0)
						{
							m.free(processes[i].get_memory_address(),processes[i].get_memory_size()/1024+1);
							processes[i].set_memory_size(0);
							processes[i].set_memory_address(-1);
						}
					}
					processes[i].set_ttl(temp_ttl);
				}
				if(processes[i].get_ttl()==0)
				{
				    cnt++;
				}
				//cout<<"-------------------------------------"<<endl;
				if(cnt==processes.size()-DAEMONS)
				{
				        cout<<"All processes have been terminated"<<endl<<"Daemons are running"<<endl;
				        cout<<"Time taken : "<<sec*1000-msecs<<" milliseconds"<<endl;
				        return 0;
				}
			}
		cout<<"-------------------------------------"<<endl;
		}
	}
};



Process getprocess(int pid)
{
   vector<string> progs;
   progs.push_back("null");
   progs.push_back("bash");
   progs.push_back("php");
   progs.push_back("c++");
   progs.push_back("prog1");
   progs.push_back("prog2");
   progs.push_back("prog3");
   progs.push_back("csh");
   progs.push_back("http");
   progs.push_back("mail");
   progs.push_back("lpl");
   return Process(pid,progs[rand(11)],100+rand(1000),"ready",5+rand(100),0,0,800+rand(10000));
}

int main()
{
	ProcessTable t=ProcessTable();
	// Add daemons
	t.add_process(Process(t.getnextpid(),"init",-1,"ready",5+rand(100),0,0,800+rand(10000)));
	t.add_process(Process(t.getnextpid(),"kblockd",-1,"ready",5+rand(100),0,0,800+rand(10000)));
	t.add_process(Process(t.getnextpid(),"sendmail",-1,"ready",5+rand(100),0,0,800+rand(10000)));
	t.add_process(Process(t.getnextpid(),"courier",-1,"ready",5+rand(100),0,0,800+rand(10000)));
	t.add_process(Process(t.getnextpid(),"httpd",-1,"ready",5+rand(100),0,0,800+rand(10000)));
	t.add_process(Process(t.getnextpid(),"aio",-1,"ready",5+rand(100),0,0,800+rand(10000)));

	int choice=0;
	do
	{
		cout << "Enter selection : 1 - New Processes, 2 - Run, 3 - Delete Process, 4 - Display all Processes, 5 - Quit : ";
		cin >> choice;
		if (choice==1)
		{
			int n=0;
			cout << "Enter the number of processes to add: ";
			cin >> n;
			for (int i=0; i<n; i++)
			{
			   t.add_process(getprocess(t.getnextpid()));
			}
		}
		else if (choice==2)
		{
			int sec=0;
			cout << "Enter the number of seconds to run : ";
			cin >> sec;
			t.run(sec);
		}
		else if (choice==3)
		{
			int pid;
			do
			{
				cout << "Enter the process id(pid>1000) to delete : ";
				cin >> pid;
			}while(pid<1001);
			t.remove_process(pid);
			//cout<<"daemons"<<DAEMONS<<endl;
		}
		else if (choice==4)
		{
			t.display_processes();
		}
		
	}while (choice != 5);
}
