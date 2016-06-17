#include"iostream"
using namespace std;

class queue
{
private:
	struct queue_ob
	{
		int value;
		queue_ob *next; 
	};

	queue_ob *head; 
	queue_ob *tail; 
	int size; 

public:
	
	queue(int x)
				
	{
		head = new(queue_ob); 
		tail = head; 
		head->value = x; 
		head->next = 0; 
		size = 1; 
	}

	int queue_size()
	{
		return size;
	}

	void push(int value)
	{
		size++;
		queue_ob *temp = new(queue_ob); 
		temp->next = 0; 
		temp->value = value; 
		tail->next = temp; 
		tail = temp; 
	}

	
	void pop(int *ret)
					  
	{
		if (size == 0)
		{
			cout << "Очередь пуста" << endl;
			return;
		}

		queue_ob *temp = head; 
		*ret = head->value; 
		head = head->next; 
		delete temp;  
		size--; 
	}


	void peek(int *ret)
	{
		if (size == 0) 
		{
			cout << "Очередь пуста!" << endl;
			return;
		}
		*ret = head->value;
	}

};
