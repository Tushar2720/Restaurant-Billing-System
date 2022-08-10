#include<stdio.h>
#include<string.h>
#include<conio.h>

//Building the structure of items
struct items{
	char item[20];
	float price;
	int qty;
};

//Building the structure of orders
struct orders{
	char customer[50];
	char date[50];
	int numOfItems;
	struct items itm[50];
};

//function to generate bills
void generateBillHeader(char name[50],char date[30]){
	printf("\n\n");
	    printf("\t Welcome To My Restaurant");          //  \t is used for horizontal tab
	    printf("\n\t--------------------------");
	    printf("\nDate: %s",date);
	    printf("\nInvoice To: %s",name);
	    printf("\n");
	    printf("---------------------------------------\n");
	    printf("Items\t\t");
	    printf("Qty\t\t");
	    printf("Total\t\t");
	    printf("\n---------------------------------------");
	    printf("\n\n");
}

void generateBillBody(char item[30], int qty, float price){
	printf("%s\t\t",item);
	printf("%d\t\t",qty);
	printf("%.2f\t\t",qty * price);
	printf("\n");
}

void generateBillFooter(float total){
	printf("\n");
	float dis = 0.1 * total;
	float netTotal = total - dis;
	float cgst = 0.09 * netTotal;
	float grandTotal = netTotal + 2*cgst;
	printf("---------------------------------------\n");
	printf("Sub Total\t\t\t%.2f",total);
	printf("\nDiscount @10%s\t\t\t%.2f","%",dis);
	printf("\n\t\t\t\t-------");
	printf("\nNet Total\t\t\t%.2f",netTotal);
	printf("\nCGST @9%s\t\t\t%.2f","%",cgst);
	printf("\nSGST @9%s\t\t\t%.2f","%",cgst);
	printf("\n---------------------------------------");
	printf("\nGrand Total\t\t\t%.2f",grandTotal);
	printf("\n---------------------------------------\n");
}

int main(){
	int opt,n;
	struct orders ord;
	struct orders order;
	float total;
	char saveBill = 'y', contFlag = 'y';
	char name[50];
	FILE *fp;
	//dashboard
	while(contFlag == 'y'){
		float total = 0;
		int invoiceFound = 0;
	printf("\t============== My Restaurant =============");
	printf("\n\n Please select your prefered operation:\t");
	printf("\n\n 1.Generate Invoice");
	printf("\n 2.Show All Invoices");
	printf("\n 3.Search Invoices");
	printf("\n 4.Exit ");
	
	printf("\n\n Your Choice:\t");
	scanf("%d",&opt);
	fgetc(stdin);        //To get the value
	
	//user can choose either of his options
	switch(opt){
		   	case 1:
			printf(" Please Enter The Name Of The Customer: \t");
			fgets(ord.customer,50,stdin);             //Get the user name as input
			ord.customer[strlen(ord.customer)-1] = 0;      //Strlen give length of string and [-1 means if string len is 10 then it will give len 9]
			strcpy(ord.date,__DATE__);            //Get the date and store in ord.date
			printf(" Please Enter The Number Of Items: \t");
			scanf("%d",&n);
			ord.numOfItems = n;
			
			for(int i=0;i<n;i++){
				fgetc(stdin);
				printf("\n\n");
				printf(" Please Enter The Item %d:\t",i+1);
				fgets(ord.itm[i].item,20,stdin);
				ord.itm[i].item[strlen(ord.itm[i].item)-1];
				printf(" Please Enter The Quantity:\t");
				scanf("%d",&ord.itm[i].qty);
				printf(" Please Enter The Unit price:\t");
				scanf("%f",&ord.itm[i].price);
				total += ord.itm[i].qty * ord.itm[i].price;
			}
			
			generateBillHeader(ord.customer,ord.date);
			for(int i =0;i<ord.numOfItems;i++){
				generateBillBody(ord.itm[i].item , ord.itm[i].qty , ord.itm[i].price);
			}
			generateBillFooter(total);
			
			printf("\n Do You Want To Save The Invoie [y/n]:\t");
			scanf("%s",&saveBill);
			
			if(saveBill == 'y'){
				fp = fopen("RestaurantBill.dat" , "a+");            //.dat is used for the extension   and  a+ means append
				fwrite(&ord,sizeof(struct orders),1,fp);
				if(fwrite != 0)
					printf("\n Successfully Saved");
				else
					printf("\n Error Saving");
				fclose(fp);	
			}
			break;
			
			case 2:
				fp = fopen("RestaurantBill.dat","r");             // r means open the .dat file in read mode
				printf("\n *****Your Previous Invoices*****\n");
				
				while(fread(&order , sizeof(struct orders),1,fp)){
					float  tot = 0;
					generateBillHeader(order.customer , order.date);
					for(int i=0;i<order.numOfItems;i++){
						generateBillBody(order.itm[i].item , order.itm[i].qty , order.itm[i].price);
						tot += order.itm[i].qty * order.itm[i].price;
					}
					generateBillFooter(tot);
				}
				fclose(fp);
				break;
		
			case 3:
				printf("\n Enter The Name Of The Customer:\t");
				//fgetc(stdin);
				fgets(name,50,stdin);
				name[strlen(name)-1] = 0;
				fp = fopen("RestaurantBill.dat","r");             // r means open the .dat file in read mode
				printf("\n *****Invoice Of %s*****\n",name);
				while(fread(&order , sizeof(struct orders),1,fp)){
					float  tot = 0;
					if(!strcmp(order.customer , name)){
						generateBillHeader(order.customer , order.date);
					   for(int i=0;i<order.numOfItems;i++){
						  generateBillBody(order.itm[i].item , order.itm[i].qty , order.itm[i].price);
						  tot += order.itm[i].qty * order.itm[i].price;
					    }
					generateBillFooter(tot);
					invoiceFound = 1;
				    }
					
				}
				if(!invoiceFound){
					printf("Sorry The Invoice For %s Does Not Exists.",name);
				}
				fclose(fp);
				break;
				
			case 4:
				printf("\n\t\t Bye Bye\n\n");
				break;
				
			default:
				printf("Sorry Invalid Option");
				break;
		}
		printf("\nDo You Want To Perform Another Operation?[y/n]:\t");
		scanf("%s",&contFlag);
	
	}
	return 0;
}

