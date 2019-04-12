#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define m 5
#define DATA_SIZE 3
#define MIN_DATASIZE 2
#define MAX 4
#define MIN 2
int NumRecords=0;
typedef enum{SUCCESS,FAILURE} status_code;

typedef struct Passenger_tag
{
    char name[100];
    int id;
    char address[100];
    char phone_no[10];
    int age;
    char preference[10];
    char allocated[10];
    struct Passenger_tag* next;
    struct Passenger_tag* prev;
}passenger;

/*typedef struct Seat_tag
{
    int booked;
    int count;
    int id;
    int id1;
    int id2;
    char name[100];
    char name1[100];
    char name2[100];
    int age;
    int age1;
    int age2;
}Seat_type;

typedef struct Coach_tag
{
    int id;
    Seat_type seat[41];
    Passenger_type* passenger_ptr;
    Passenger_type* rac_passenger_ptr;
    struct Coach_tag* prev;
    struct Coach_tag* next;
}Coach_type;

typedef struct Train_tag
{
    char name[100];
    int id;
    int freeze;
    struct Train_tag* prev;
    struct Train_tag* next;
    Coach_type* coach_ptr;
    Passenger_type* waiting_passenger_ptr;
}Train_type;
*/
typedef struct union_tag{
	union btree{
		struct key_nodes{
			int acc_no[m+1];
			struct union_tag * ptr[m+1];
			int count;
		}key;
		struct data_nodes{
			passenger arr[DATA_SIZE+1];
			struct union_tag * next;
			struct union_tag * prev;
			int dataCount;
		}data;

	}u;
	char flag;
}record;
int validate_number(char str[])
{
	int len=strlen(str);
	int flag=1;
	int i;
	if(len<=10)
    {
        for(i=0;i<len && flag==1;i++)
        {
            int d=str[i];
            if(!(d>=48 && d<=57))
            {
                flag=0;
            }
        }
    }
    else
    {
        flag=0;
    }
    printf("********Flag:%d",flag);
	return flag;
}

//Function to validate the name of Passenger
int validate_name(char str[])
{
	int i,l,flag=0;
	l=strlen(str);
	if(l>=25)
	{
		printf("\nMaximum Characters allowed are 25\n");
		return 0;
	}
	for(i=0;i<l && flag==0;i++)
	{
		if(str[i]<65 || str[i]>122)
			flag=1;
		else
		{
			if(str[i]>=91 && str[i]<=96)
				flag=1;
		}
	}
	return !flag;
}


passenger makeRecord(char name[],unsigned long num,char address[],char pno[],int age,char pre[])
{
	passenger details;
	strcpy(details.name,name);
	details.id=num;
	strcpy(details.address,address);
	details.age=age;
	strcpy(details.phone_no,pno);
	strcpy(details.preference,pre);
	//strcpy(details.allocated,alloc);
	return details;
}
void printRecord(passenger rec)
{
			printf("------------------------------------------------------------------\n");
			printf("Name: %s\n",rec.name);
			printf("ID: %lu\n",rec.id);
			printf("Address: %s\n",rec.address);
			printf("Age: %d\n",rec.age);
			printf("Phone number is: %s\n",rec.phone_no);
			printf("Seat preference:%s\n",rec.preference);
			printf("------------------------------------------------------------------\n");
}


//Function which takes input from file and inserts it into tree.

//Function to print the whole tree.
void printTree(record *root)
{
	int i;
	if(root==NULL)
	{
		printf("No records in the tree\n");
	}
	else if(root->flag=='d')
	{
		record *iptr;
		iptr=root;
		while(iptr!=NULL)
		{
			for(i=0;i<iptr->u.data.dataCount;i++)
				printRecord(iptr->u.data.arr[i]);
			printf("\n*****node ends*****\n");
			iptr=iptr->u.data.next;
		}
	}
	else
	{
		printTree(root->u.key.ptr[0]);
	}
	
}
//Function which searches a data node where required no could be present.
//Returns a pointer to the data node for further inside search 
record * searchNode(record *root, int no)
{
	if(root->flag=='k')
	{
		int i=1;
		while(root->u.key.acc_no[i]<=no && i<=root->u.key.count)
		{
			i++;
		}
		root=searchNode(root->u.key.ptr[i-1],no);

	}
	return root;
}

//Searches for a particular value in data node and prints the details if value is present
void searchValueInDataNode(record *dptr, int val)
{
	int i;
	if(dptr==NULL)
	{
		printf("Error\n");
	}
	else if(dptr->flag=='d')
	{
		int found=0;
		for(i=0;i<DATA_SIZE && found==0;i++)
		{
			if(dptr->u.data.arr[i].id==val)
			{
				found=1;
			}
		}
		if(found==0)
		{
			printf("KEY NOT FOUND\n");
		}
		else
		{
			printf("The details of record are: \n");
			printf("Name: %s\n",dptr->u.data.arr[i-1].name);
			printf("ID: %lu\n",dptr->u.data.arr[i-1].id);
			printf("Address: %s\n",dptr->u.data.arr[i-1].address);
			printf("Age: %d\n",dptr->u.data.arr[i-1].age);
			printf("Phone number is: %s\n",dptr->u.data.arr[i-1].phone_no);
			printf("************************************************************");
		}
	}

}

//Actual Search function
void searchFunction(record *root,int val)
{
	record *result;
	result=searchNode(root,val);
	searchValueInDataNode(result,val);

}

//Recursive helper function in insert
int PushDown(int no, record *root, int *medno, record **right_subtree,passenger details)
{
	int pos,i=0;
	int found=0;
	//Stopping condition of recursion
	if(root->flag=='d')
	{
		//Move forward till you encounter a node having equal or greater value than the required acc no
		while(root->u.data.arr[i].id<no && i<root->u.data.dataCount)
		{
			i++;
		}
		//If the value is same as the one to be inserted, we update that record with the new fields.
		if(root->u.data.arr[i].id==no)
		{
			update(details,root,i);
		}
		//If the value is greater we insert it taking into cosideration the node capacity
		else
		{
			//If the node where data is to be inserted has less than maximum elements, we simply insert and we are done.
			if(root->u.data.dataCount<DATA_SIZE)
			{
				insertInDataNode(details,root);
				return 0;
			}
 			//If data node is already full, we have to split it into two and continue the recursion as it will affect the above levels
			else
			{
				splitLeaf(details,root,right_subtree,medno);
				return 1;
			}
		}
	}
	else if(root->flag=='k')
	{
		//We search for a position to move the pointer in order to reach a particular node
		searchPosition(no,root,&pos);
		//if the recursion is to be continued. The case when data node has been splitted into two.
		if(PushDown(no,root->u.key.ptr[pos], medno,right_subtree,details))
		{
			//Splitleaf sends a median which is to be inserted in key nodes, we assign it to no.
		    no=*medno;
		    //If the key node where median is to be inserted has less than maximum node, we simply insert the key and we are done.
			if(root->u.key.count<MAX)
			{
				insertInKey(no,root,*right_subtree,pos);
				return 0;
			}
			//If the key node has maximum number of nodes, we split the key node and continue the recursion for inserting keys in further levels.
			else
			{
				splitKey(no,*right_subtree,root,pos,medno,right_subtree);
				return 1;
			}
		}
		return 0;
	}
}



record* insert(record *root, passenger details, int no)
{
	int medno;
	record * right_subtree;
	record * newroot;
	//The very first node which will be inserted is a data node and created explicitly.
	if(root==NULL)
	{
		record *nptr;
		nptr=(record *)malloc(sizeof(record));
		nptr->flag='d';
		nptr->u.data.arr[0]=details;
		nptr->u.data.dataCount=1;
		nptr->u.data.next=NULL;
		nptr->u.data.prev=NULL;
		root=nptr;
		NumRecords++;
	}
	//For all other nodes we call the recursive function
	else if(PushDown(no,root,&medno,&right_subtree,details))
	{
		//The actual root of the tree is created here. Pushdown send the median to be inserted and the right and left subtrees to be attached.
		newroot=(record *)malloc(sizeof(record));
		newroot->flag='k';
		newroot->u.key.count=1;
		newroot->u.key.acc_no[1]=medno;
		newroot->u.key.ptr[0]=root;
		newroot->u.key.ptr[1]=right_subtree;
		return newroot;

	}
	return root;

}
//Function which shifts the entries in the data node and inserts the acc_no at desired position
void insertInDataNode(passenger details,record *root)
{
	int i=root->u.data.dataCount-1;
	if(root->flag=='d')
	{
		while(root->u.data.arr[i].id>=details.id && i>=0)
		{
			root->u.data.arr[i+1]=root->u.data.arr[i];
			i--;
		}
		root->u.data.arr[i+1]=details;
	}
	root->u.data.dataCount++;
	NumRecords++;
}

//Function which shifts the entries and pointer in the key node and inserts the acc_no at desired position and its right subtree at its appropriate position
void insertInKey(int no,record *root,record *right_subtree,int pos)
{
	int i;
	for(i=root->u.key.count;i>pos;i--)
	{
		root->u.key.acc_no[i+1]=root->u.key.acc_no[i];
		root->u.key.ptr[i+1]=root->u.key.ptr[i];
	}
	root->u.key.acc_no[pos+1]=no;
	root->u.key.ptr[pos+1]=right_subtree;
	root->u.key.count++;
}

//Function which gives the position of the branch which ultimately leads to the data node where the entry could be present
void searchPosition(int no,record *root,int *pos)
{
	if(root->flag=='k')
	{
		if(no<root->u.key.acc_no[1])
		{
			*pos=0;
		}
		else
		{
			for(*pos=root->u.key.count;no<root->u.key.acc_no[*pos];*pos=*pos-1);
		}

	}
}

//Function which splits the key nodes and sends the median and right and left subtree of the median
void splitKey(int medentry,record *medright,record *root,int pos, int *newmedian,record **newright)
{
	int i;
	int median;
	//We find the correct position of the median
	if(pos<=MIN)
	{
		median=MIN;
	}
	else
	{
		median=MIN+1;
	}
	//New node to act as a right node
	*newright=(record *)malloc(sizeof(record));
	//All the elements after median in original node are copied into the right subtree
	for(i=median+1;i<=MAX;i++)
	{
		(*newright)->u.key.acc_no[i-median]=root->u.key.acc_no[i];
		(*newright)->u.key.ptr[i-median]=root->u.key.ptr[i];
	}
	//Counts in both the nodes are set appropriately
	(*newright)->flag='k';
	(*newright)->u.key.count=MAX-median;
	root->u.key.count=median;
	if(pos<=MIN)
	{
		insertInKey(medentry,root,medright,pos);
	}
	else
	{
		insertInKey(medentry,*newright,medright,pos-median);
	}
	*newmedian=root->u.key.acc_no[root->u.key.count];
	(*newright)->u.key.ptr[0]=root->u.key.ptr[root->u.key.count];
	root->u.key.count--;

}

//Function which splits the data node
void splitLeaf(passenger details,record *root,record **right_subtree,int *no)
{
     passenger temp_arr[m+1];
     record *leaf_right;
     int i=0,j,k=0;
     //Copy all the record in a temporary array along with the one to be inserted 
     while(details.id>root->u.data.arr[i].id && i<root->u.data.dataCount)
     {
        temp_arr[i]=root->u.data.arr[i];
        i++;
     }
     temp_arr[i]=details;
     while(i<=root->u.data.dataCount)
     {
        temp_arr[i+1]=root->u.data.arr[i];
        i++;
     }
     *no=temp_arr[i/2].id;
     root->u.data.dataCount=(int)ceil((double)root->u.data.dataCount/2);
     leaf_right=(record *)malloc(sizeof(record));
     leaf_right->flag='d';
     leaf_right->u.data.dataCount=i-root->u.data.dataCount;
     
     for(j=0;j<i/2;j++)
     {
         root->u.data.arr[j]=temp_arr[j];

     }
     for(j=i/2;j<i;j++)
     {
        leaf_right->u.data.arr[k]=temp_arr[j];
        k++;
     }
     if(root->u.data.next!=NULL)
     {
     	(root->u.data.next)->u.data.prev=leaf_right;
	 }
     leaf_right->u.data.next=root->u.data.next;
     root->u.data.next=leaf_right;
     leaf_right->u.data.prev=root;
     *right_subtree=leaf_right;
     NumRecords++;
     

}

void update(passenger details,record *root,int i)
{
	root->u.data.arr[i]=details;
}

void MoveLeftKey(record *root,int pos)
{
	int c;
	record *t;
	t=root->u.key.ptr[pos-1];
	t->u.key.count++;
	t->u.key.acc_no[t->u.key.count]=root->u.key.acc_no[pos];
	t->u.key.ptr[t->u.key.count]=(root->u.key.ptr[pos])->u.key.ptr[0];
	t=root->u.key.ptr[pos];
	root->u.key.acc_no[pos]=t->u.key.acc_no[1];
	t->u.key.ptr[0]=t->u.key.ptr[1];
	t->u.key.count--;
	for(c=1;c<=t->u.key.count;c++)
	{
		t->u.key.acc_no[c]=t->u.key.acc_no[c+1];
		t->u.key.ptr[c]=t->u.key.ptr[c+1];
	}
}


void MoveRightKey(record *root,int pos)
{
	int c;
	record *t;
	t=root->u.key.ptr[pos];
	for(c=t->u.key.count;c>0;c--)
	{
		t->u.key.acc_no[c+1]=t->u.key.acc_no[c];
		t->u.key.ptr[c+1]=t->u.key.ptr[c];
	}
	t->u.key.ptr[1]=t->u.key.ptr[0];
	t->u.key.count++;
	t->u.key.acc_no[1]=root->u.key.acc_no[pos];
	t=root->u.key.ptr[pos-1];
	root->u.key.acc_no[pos]=t->u.key.acc_no[t->u.key.count];
	(root->u.key.ptr[pos])->u.key.ptr[0]=t->u.key.ptr[t->u.key.count];
	t->u.key.count--;
}


void CombineKey(record *root,int pos)
{
	int c;
	record * right, * left;
	right=root->u.key.ptr[pos];
	left=root->u.key.ptr[pos-1];
	left->u.key.count++;
	
	left->u.key.acc_no[left->u.key.count]=root->u.key.acc_no[pos];
	left->u.key.ptr[left->u.key.count]=right->u.key.ptr[0];
	
	for(c=1;c<=right->u.key.count;c++)
	{
		left->u.key.count++;
		left->u.key.acc_no[left->u.key.count]=right->u.key.acc_no[c];
		left->u.key.ptr[left->u.key.count]=right->u.key.ptr[c];
	}
	
	for(c=pos;c<root->u.key.count;c++)
	{
		root->u.key.acc_no[c]=root->u.key.acc_no[c+1];
		root->u.key.ptr[c]=root->u.key.ptr[c+1];
	}
	
	root->u.key.count--;
	free(right);		
}



void MoveLeftLeaf(record *root,int pos)
{
	record *lhs;
	record *rhs;
	int c;
	lhs=root->u.key.ptr[pos-1];
	rhs=root->u.key.ptr[pos];
	lhs->u.data.arr[lhs->u.data.dataCount]=rhs->u.data.arr[0];
	lhs->u.data.dataCount++;
	root->u.key.acc_no[pos]=rhs->u.data.arr[1].id;
	for(c=1;c<rhs->u.data.dataCount;c++)
	{
		rhs->u.data.arr[c-1]=rhs->u.data.arr[c];
		
	}
	rhs->u.data.dataCount--;
}

void MoveRightLeaf(record *root,int pos)
{
	record *lhs;
	record *rhs;
	int c;
	lhs=root->u.key.ptr[pos-1];
	rhs=root->u.key.ptr[pos];
	for(c=rhs->u.data.dataCount-1;c>=0;c--)
	{
		rhs->u.data.arr[c+1]=rhs->u.data.arr[c];
	}
	rhs->u.data.dataCount++;
	rhs->u.data.arr[0]=lhs->u.data.arr[lhs->u.data.dataCount-1];
	root->u.key.acc_no[pos]=rhs->u.data.arr[lhs->u.data.dataCount-1].id;
	lhs->u.data.dataCount--;	
}

void CombineLeaf(record *root,int pos)
{

	record *lhs,*rhs;
	int c;
	lhs=root->u.key.ptr[pos-1];
	rhs=root->u.key.ptr[pos];
	int elems=lhs->u.data.dataCount;
	for(c=0;c<rhs->u.data.dataCount;c++)
	{
	
		lhs->u.data.arr[elems]=rhs->u.data.arr[c];
		elems++;
	}
	for(c=pos+1;c<=root->u.key.count;c++)
	{
		root->u.key.acc_no[c-1]=root->u.key.acc_no[c];
		root->u.key.ptr[c-1]=root->u.key.ptr[c];
	}
	lhs->u.data.next=rhs->u.data.next;
	if(rhs->u.data.next!=NULL)
	{
		(rhs->u.data.next)->u.data.prev=lhs;
	}
	lhs->u.data.dataCount=elems;
	root->u.key.count--;
	free(rhs);
	
}

void DeleteFromLeafDirect(int target,record *root)
{
	int pos,i=0;
	if(root->flag=='d')
	{
		while(root->u.data.arr[i].id<target)
		{
			i++;
		}
		if(root->u.data.arr[i].id>target)
		{
			printf("ENTRY TO BE DELETED IS ABSENT\n");
		}
		else if(root->u.data.arr[i].id==target)
		{
			i++;
			for(i;i<root->u.data.dataCount;i++)
			{
				root->u.data.arr[i-1]=root->u.data.arr[i];
			}
			root->u.data.dataCount--;
		}
	}
	NumRecords--;
}

//Pos is always the right node
int RestoreLeaf(int target,record *root,int pos)
{
	int k=0;
	int retval=0;
	if(pos==0)
	{
		if((root->u.key.ptr[1])->u.data.dataCount>MIN_DATASIZE)
		{
			MoveLeftLeaf(root,1);
		}
		else
		{
			CombineLeaf(root,1);
			retval=1;
		}
	}
	else if(pos==root->u.key.count)
	{
		if((root->u.key.ptr[pos-1])->u.data.dataCount>MIN_DATASIZE)
		{
			MoveRightLeaf(root,pos);
		}
		else
		{
			CombineLeaf(root,pos);
			retval=1;
		}
	}
	else if((root->u.key.ptr[pos-1])->u.data.dataCount>MIN_DATASIZE)
	{
		MoveRightLeaf(root,pos);
	}
	else if((root->u.key.ptr[pos+1])->u.data.dataCount>MIN_DATASIZE)
	{
		MoveLeftLeaf(root,pos+1); 
	}
	else
	{
		CombineLeaf(root,pos);
		retval=1;
	}
	return retval;
	
}

int RestoreKey(int target,record *root,int pos)
{
	int k=0;
	int retval=0;
	if(pos==0)
	{
		if((root->u.key.ptr[1])->u.key.count>MIN)
		{
			MoveLeftKey(root,1);
		}
		else
		{
			CombineKey(root,1);
			retval=1;
		}
	}
	else if(pos==root->u.key.count)
	{
		if((root->u.key.ptr[pos-1])->u.key.count>MIN)
		{
			MoveRightKey(root,pos);
		}
		else
		{
			CombineKey(root,pos);
			retval=1;
		}
	}
	else if((root->u.key.ptr[pos-1])->u.key.count>MIN)
	{
		MoveRightKey(root,pos);
	}
	else if((root->u.key.ptr[pos+1])->u.key.count>MIN)
	{
		MoveLeftKey(root,pos+1);
	}
	else
	{
		CombineKey(root,pos);
		retval=1;
	}
	return retval;
}






int Delete_rec(int target,record *root)
{
	int pos,k,check_count;
	if((root->u.key.ptr[0])->flag=='d')
	{
		searchPosition(target,root,&pos);
		DeleteFromLeafDirect(target,root->u.key.ptr[pos]);
		check_count=(root->u.key.ptr[pos])->u.data.dataCount;
		if(check_count>=MIN_DATASIZE)
		{
			return 0;
		}
		else
		{
			k=RestoreLeaf(target,root,pos);		//returns 1 if merge else 0
			return k;
			
		}
		
	}
	else
	{
		searchPosition(target,root,&pos);
		if(Delete_rec(target,root->u.key.ptr[pos]))
		{
			check_count=(root->u.key.ptr[pos])->u.key.count;
			if(check_count>=MIN)
			{
				return 0;
			}
			else
			{
				k=RestoreKey(target,root,pos);
				return k;
			}
			
		}
		
		
	}
	return 0;
}

record * Delete(int target,record *root)
{
	record *oldroot;
	int k;
	if(root==NULL)
	{
		printf("Tree is empty\n");
	}
	else if(root->flag=='d')
	{
		DeleteFromLeafDirect(target,root);
	}
	else if (root->flag=='k')
	{
		Delete_rec(target,root);
		if(root->u.key.count==0)
		{
			oldroot=root;
			root=root->u.key.ptr[0];
			free(oldroot);
		}
	}
	
	return root;
}


void getNum(record *root)
{
	printf("The number of records in the database are %d\n",NumRecords);

}

int Height(record *root)
{
	int retval;
	if(root==NULL)
	{
		retval=-1;
	}
	else if(root->flag=='d')
	{
		retval=0;
	}
	else
	{
		retval=1+Height(root->u.key.ptr[0]);
	}
	return retval;
}



void RangeSearch(record *root,int key1,int key2)
{
	record *node,*sptr;
	int found=0,i;
	
	if(root==NULL)
	{
		printf("Tree is empty\n");
	}
	else
	{
		node=searchNode(root,key1);
		sptr=node;
		printf("The details of record are: \n");
		while(sptr!=NULL && found==0)
		{
			for(i=0;i<sptr->u.data.dataCount && found==0;i++)
			{
				if(sptr->u.data.arr[i].id>=key1 && sptr->u.data.arr[i].id<=key2)
				{
					printRecord(sptr->u.data.arr[i]);
				}
				if(sptr->u.data.arr[i].id>=key2)
				{
					found=1;
				}
	
			}
			sptr=sptr->u.data.next;
		}
		if(found==0)
		{
			printf("No records in the given range\n");
		}
	}
}
int main()
{
	int choice;
    char name[100],address[100],pno[15],pre[10],alloc[10];
    int age,flag=1;
    int id;
    record *root=NULL;
    int key1,key2,h,before,after;
    char enter;
    passenger details;
    //root=inputFromFile();
    while(flag==1){
	
    	printf("------------------------------------------------------------\n");
		printf("MENU\n");
		printf("0:PRINT LIST\n");
		printf("1:INSERT DATA\n");
		printf("2:DELETE DATA\n");
		printf("3:GET NUMBER OF ACTIVE RECORD\n");
		printf("4:SEARCH\n");
		printf("5.HEIGHT OF THE TREE\n");
		printf("6.RANGE SEARCH\n");
		printf("7:QUIT\n");
		printf("------------------------------------------------------------\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 0: printf("The data in the tree is:\n");
					printTree(root);
					break;
			case 1: printf("Enter Passenger name\n");
		            scanf("%c",&enter);
		            gets(name);
		            printf("Enter Passenger id\n");
		            scanf("%d",&id);
		            printf("Enter address\n");
		            scanf("%c",&enter);
		            gets(address);
		            printf("Enter age\n");
		            scanf("%d",&age);
		            printf("Enter phone number\n");
		            scanf("%c",&enter);
		            gets(pno);
		            printf("Enter the Preference");
		            scanf("%c",&enter);
		            gets(pre);
		            details=makeRecord(name,id,address,pno,age,pre);
		            
		            before=NumRecords;
		            root=insert(root,details,details.id);
		            after=NumRecords;
		            if(before<after)
		            {
		            	printf("INSERTED SUCCESSFULLY\n");
					}
					else
					{
						printf("INSERTION UNSUCCESSFUL\n");
					}
		            printf("Tree after insertion\n");
		            printTree(root);
		            break;
			case 2: printf("Enter the id to be deleted\n");
					scanf("%d",&id);
					before=NumRecords;
					root=Delete(id,root);
					after=NumRecords;
					if(before>after)
		            {
		            	printf("DELETED SUCCESSFULLY\n");
					}
					else
					{
						printf("DELETION UNSUCCESSFUL\n");
					}
					printf("Tree after deletion\n");
		            printTree(root);
		            break;
		    case 3:	getNum(root);
		    		break;
		    case 4: printf("Enter the id to be searched\n");
		    		scanf("%d",&id);
		    		searchFunction(root,id);
		    		break;
		    case 5: h=Height(root);
		    		printf("Height of the tree is %d\n",h);
		    		break;
		    case 6: printf("Enter the lower and upper limits\n");
		    		scanf("%d %d",&key1,&key2);
		    		RangeSearch(root,key1,key2);
		    		break;
		    case 7: printf("THANK YOU\n");
		            flag=0;
		    		//writeOutput(root);
		    		break;
		    default: printf("Invalid entry\n");
		    		break;
		    		
					
		}
	}
	return 0;
}

