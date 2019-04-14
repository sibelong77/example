#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 int passid=0;
int coachID=0;
int k=0;
typedef struct Passenger_tag
{
    char name[100];
    int pass_id;
    char address[100];
    char phone_no[100];
    int age;
    char preference[100];
    char allocated[100];
    int heightp;
    struct Passenger_tag* leftp;
    struct Passenger_tag* rightp;
}Passenger_type;
typedef struct Seat_tag
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
    int coach_id;
    int key;
    Seat_type seat[41];
    Passenger_type* passenger_ptr;
    Passenger_type* rac_passenger_ptr;
    int heightc;
    struct Coach_tag* leftc;
    struct Coach_tag* rightc;
}Coach_type;

int max(int a,int b)
{
    return (a > b)? a : b;
}
Passenger_type* initpassNode(){
    int j;
    Passenger_type* head=(Passenger_type*)malloc(sizeof(Passenger_type));
    printf("Enter the name of Passenger\n");
    scanf("%s",head->name);
    printf("Enter the address\n");
    scanf("%s",head->address);
    printf("Enter the Phone number\n");
    scanf("%s",head->phone_no);
    printf("Enter the Preference\n");
    scanf("%s",head->preference);
    printf("Enter the age\n");
    scanf("%d",&head->age);
    passid++;
    head->pass_id=passid;
    head->heightp=1;
    head->rightp= head->leftp= NULL;
    return head;
}
Coach_type* initcoachNode(){
    Coach_type* head=(Coach_type*)malloc(sizeof(Coach_type));
    k++;
    head->key=k;
    coachID=coachID+1;
    head->coach_id=coachID;
    int i;
    for(i=0;i<40;i++)
    {
    	head->seat[i].booked=0;
    	head->seat[i].count=0;
	}
	head->passenger_ptr=NULL;
	head->rac_passenger_ptr=NULL;
	head->heightc=1;
	head->leftc=NULL;
	head->rightc=NULL;
    return head;
}
//Calculates height
int hc(Coach_type *T)
{
    int lh,rh;
	if(T==NULL)
		return(0);

	if(T->leftc==NULL)
		lh=0;
	else
		lh=1+T->leftc->heightc;

	if(T->rightc==NULL)
		rh=0;
	else
		rh=1+T->rightc->heightc;

	if(lh>rh)
		return(lh);

	return(rh);
}
int hp(Passenger_type *T)
{
    int lh,rh;
	if(T==NULL)
		return(0);

	if(T->leftp==NULL)
		lh=0;
	else
		lh=1+T->leftp->heightp;

	if(T->rightp==NULL)
		rh=0;
	else
		rh=1+T->rightp->heightp;

	if(lh>rh)
		return(lh);

	return(rh);
}
struct Coach_tag *rightRotatec(struct Coach_tag *x){
    struct Coach_tag* y;
	y=x->leftc;
	x->leftc=y->rightc;
	y->rightc=x;
	x->heightc=hc(x);
	y->heightc=hc(y);
	return(y);
}
Passenger_type *rightRotatep(Passenger_type *x){
    Passenger_type *y;
	y=x->leftp;
	x->leftp=y->rightp;
	y->rightp=x;
	x->heightp=hp(x);
	y->heightp=hp(y);
	return(y);
}
Passenger_type *leftRotatep(Passenger_type *x){
    Passenger_type *y;
	y=x->rightp;
	x->rightp=y->leftp;
	y->leftp=x;
	x->heightp=hp(x);
	y->heightp=hp(y);

	return(y);
}
Coach_type *leftRotatec(Coach_type *x){
    Coach_type *y;
	y=x->rightc;
	x->rightc=y->leftc;
	y->leftc=x;
	x->heightc=hc(x);
	y->heightc=hc(y);

	return(y);
}
int getBalancec(Coach_type *N)
{
	if (N == NULL)
		return 0;
	return hc(N->leftc) - hc(N->rightc);
}
int getBalancep(Passenger_type *N)
{
	if (N == NULL)
		return 0;
	return hp(N->leftp) - hp(N->rightp);
}
Coach_type* addcoach(Coach_type* head, Coach_type* node){
    if(head==NULL){
        return node;
    }
    if(node->key < head->key){
        head->leftc=addcoach(head->leftc, node);
    }
    else if(node->key > head->key){
        head->rightc=addcoach(head->rightc, node);
    }
    else{
        return head;
    }
    head->heightc=1+max(hc(head->leftc), hc(head->rightc));
    int  balance=getBalancec(head);
    
    if (balance > 1 &&(node->key < head->key))
		return rightRotatec(head);

	// Right Right Case
	if (balance < -1 && (node->key > head->key))
		return leftRotatec(head);

	// Left Right Case
	if (balance > 1 && (node->key > head->leftc->key))
	{
		head->leftc = leftRotatec(head->leftc);
		return rightRotatec(head);
	}

	// Right Left Case
	if (balance < -1 && (node->key < head->rightc->key))
	{
		head->rightc = rightRotatec(head->rightc);
		return leftRotatec(head);
	}

	/* return the (unchanged) node pointer */
	return head;
}
Passenger_type* BookTicket(Passenger_type* head, Passenger_type* node){
    if(head==NULL){
        return node;
    }
    if(strcmp(node->name, head->name)<0){
        head->leftp=BookTicket(head->leftp, node);
    }
    else if(strcmp(node->name, head->name)>0){
        head->rightp=BookTicket(head->rightp, node);
    }
    else{
        return head;
    }
    head->heightp=1+max(hp(head->leftp), hp(head->rightp));
    int  balance=getBalancep(head);
    
    if (balance > 1 && strcmp(node->name, head->leftp->name)<0)
		return rightRotatep(head);

	// Right Right Case
	if (balance < -1 && strcmp(node->name, head->rightp->name)>0)
		return leftRotatep(head);

	// Left Right Case
	if (balance > 1 && strcmp(node->name, head->leftp->name)>0)
	{
		head->leftp = leftRotatep(head->leftp);
		return rightRotatep(head);
	}

	// Right Left Case
	if (balance < -1 && strcmp(node->name, head->rightp->name)<0)
	{
		head->rightp = rightRotatep(head->rightp);
		return leftRotatep(head);
	}

	/* return the (unchanged) node pointer */
	return head;
}
void printPreorderc(Coach_type* node) 
{ 
     if (node == NULL) 
          return; 
  
     /* first print data of node */
     printf("ID: %d\n",node->coach_id);  
  
     /* then recur on left subtree */
     printPreorderc(node->leftc);   
  
     /* now recur on right subtree */
     printPreorderc(node->rightc); 
}     
void printPreorder(Passenger_type* node) 
{ 
     if (node == NULL) 
          return; 
  
     /* first print data of node */
     printf("Name: %s\nPreference: %s\nID: %d\n", node->name, node->preference, node->pass_id);  
  
     /* then recur on left sutree */
     printPreorder(node->leftp);   
  
     /* now recur on right subtree */
     printPreorder(node->rightp); 
}     
void preference_transform(int age[],char berth[][100],char name[][100],int num)
{
	int i,j,len,index=0,index1=0,temp1;
	len=num;
	int temp_age[100];
	char temp_name[100][100],temp_berth[100][100];
	char temp2[100],temp3[100];
	
	for(i=0;i<len;i++)
	{
		if(strcmp(berth[i],"lower")==0 || strcmp(berth[i],"side_lower")==0)
		{
			temp_age[index]=age[i];
			strcpy(temp_name[index],name[i]);
			strcpy(temp_berth[index],berth[i]);
			index++;
		}
	}
	for(i=0;i<index;i++)
	{
		for(j=0;j<index-i-1;j++)
		{
			if((temp_age[j]>=15 || temp_age[j]<=60) && (temp_age[j+1]<15 || temp_age[j+1]>60))
			{
				temp1=temp_age[j];
				temp_age[j]=temp_age[j+1];
				temp_age[j+1]=temp1;
				
				strcpy(temp2,temp_name[j]);
				strcpy(temp_name[j],temp_name[j+1]);
				strcpy(temp_name[j+1],temp2);
				
				strcpy(temp3,temp_berth[j]);
				strcpy(temp_berth[j],temp_berth[j+1]);
				strcpy(temp_berth[j+1],temp3);
			}
		}
	}
	
	for(i=0;i<len;i++)
	{
		if(strcmp(berth[i],"lower")==0 || strcmp(berth[i],"side_lower")==0)
		{
			strcpy(berth[i],temp_berth[index1]);
			strcpy(name[i],temp_name[index1]);
			age[i]=temp_age[index1];
			index1++;
		}
	}
}
Passenger_type* minValueNode(Passenger_type* node) 
{ 
    Passenger_type* current = node; 
    while (current->leftp != NULL) 
        current = current->leftp; 
  
    return current;
}
Passenger_type* cancelticket(Passenger_type* root, int pid) 
{ 
  
    if (root == NULL) 
        return root; 
  
    
    if ( pid < root->pass_id ) 
        root->leftp = cancelticket(root->leftp,pid); 
  
    else if( pid > root->pass_id ) 
        root->rightp = cancelticket(root->rightp,pid); 
  
    else
    { 
        if( (root->leftp == NULL) || (root->rightp == NULL) ) 
        { 
            Passenger_type *temp = root->leftp ? root->leftp : 
                                             root->rightp; 
  
           
             if (temp == NULL) 
            { 
                temp = root; 
                root = NULL; 
            } 
            else
             *root = *temp; 
            free(temp); 
        } 
        else
        { 
            
            Passenger_type* temp = minValueNode(root->rightp); 
            root->pass_id = temp->pass_id; 
            root->rightp = cancelticket(root->rightp, temp->pass_id); 
        } 
    } 
  
    if (root == NULL) 
      return root; 
  
    root->heightp = 1 + max(hp(root->leftp),hp(root->rightp)); 
  
     int balance = getBalancep(root); 
  
    if (balance > 1 && getBalancep(root->leftp) >= 0) 
        return rightRotatep(root); 
  
    // Left Right Case 
    if (balance > 1 && getBalancep(root->leftp) < 0) 
    { 
        root->leftp =  leftRotatep(root->leftp); 
        return rightRotatep(root); 
    } 
  
    // Right Right Case 
    if (balance < -1 && getBalancep(root->rightp) <= 0) 
        return leftRotatep(root); 
  
    // Right Left Case 
    if (balance < -1 && getBalancep(root->rightp) > 0) 
    { 
        root->rightp= rightRotatep(root->rightp); 
        return leftRotatep(root); 
    } 
  
    return root; 
} 
void range(Passenger_type* node,int k1,int k2)
{
	if(node==NULL)
	{
		return;
	}
	if(node->pass_id>=k1 && node->pass_id<=k2)
	{
	   printf("Name: %s\nPreference: %s\nID: %d\n", node->name, node->preference, node->pass_id);	
	}
	range(node->leftp,k1,k2);
	range(node->rightp,k1,k2);
}
int main()
{
 Coach_type* head1=NULL;
 Passenger_type* phead=NULL;
 char choice1;
 int flag=1,i;
 printf("\t \t**** WELCOME TO INDIAN RAILWAY*****");
 printf("\n");

 printf("\t \t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
 while(flag==1)
 {
 	printf("\n");
 printf("1.ADD COACH");
 printf("\n");
 printf("2.BOOK TICKET");
 printf("\n");
 printf("3.CANCEL TICKET");
 printf("\n");
 printf("4.RANGE");
 printf("\n");
 printf("5.PASSENGER DETAILS");
 printf("\n");
 printf("6.EXIT");
 printf("\n");

   printf("Enter the choice:");
   scanf("%d",&i);
   switch(i)
   {

     case 1:{
                head1=addcoach(head1,initcoachNode());
                printPreorderc(head1);
                break;
            }
      case 2:
	        {
               phead=BookTicket(phead,initpassNode());
               //printPreorder(phead);
               break;
            }
      case 3:{
      	      int pid;
      	      printf("Enter the Passenger Id to Cancel:");
      	      scanf("%d",&pid);
      	      phead=cancelticket(phead,pid);
      	      //printPreorder(phead);
		      break;
		     }
      case 4:
	        {
	          int k1,k2;
			  printf("Enter the lower and upper limits\n");
		      scanf("%d %d",&k1,&k2);
	          range(phead,k1,k2);
		      break;
	        }
      	    
      case 5:
	        {
	         printPreorder(phead);
		     break;
	        }
      case 6:{
               flag=0;
               break;
             }
     default:{
              printf("Invalid");
              break;
             }

   }
 }
 return 0;
}
