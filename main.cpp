#include <iostream>
#include <string>
#include <ctime>

using namespace std;
//ȫ�ֲ���
bool *code=new bool(8);//��-���ص�����
bool *key=new bool(10);//ԭ��Կ
bool *roundKey = new bool(16);//����Կ
bool *cache = new bool(10); //��ʱ����ռ�

static bool binaryTable[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};//�����ƶ�Ӧ��λ �ռ任ʱ��
static int SBoxTable[2][4][4] = {{{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 0, 2}}, {{0, 1, 2, 3}, {2, 3, 1, 0}, {3, 0, 1, 2}, {2, 1, 0, 3}}};
static int P10Table[]={3,5,2,7,4,10,1,9,8,6};
static int P8Table[]={6,3,7,4,8,5,10,9};


void print(bool* input, int n){//��ӡ�ú������ֱ�Ϊ��������ָ�뼰���鳤��
	for(int i=0;i<n;i++)
		cout<<input[i];
	cout<<endl;
}

bool* binary(bool* input,int n){//�����ƻ��������ֱ����洢������ָ�뼰�����intʮ��������
	bool* output= input;
	for(int i=0,k=128;i<8;i++,k/=2){
		output[i]=n/k;
		n=n-k;
	}
	return output;
}

int declize(bool* input){//ʮ���ƻ�����������ָ�����ʮ��������
	int n=0;
	for(int i=7,k=1;i>=0;i--,k*=2)
		n+=input[i]*k;
	return n;
}

//���ߺ���

bool *inputIntoArray(bool* from, bool* to,int length){//�����ڼ�����ԭ��Կ�ú���
	for(int i=0;i<length;i++)
		to[i]=from[i];
}

//��Կ���ֺ���
bool *generateOriginalKey() { //����ԭ��Կ�ĺ������ĵ���δ�ἰ��
	srand((unsigned int)time(NULL));
	for(int i=0;i<10;i++)
		key[i]=rand()%2;
	return key;
}


void addarr(bool *num,bool *x,bool *y,int n){  //�ϲ����麯�� 
	n/=2;
	for(int i=0;i<n;i++){
		num[i]=x[i];
		num[i+n]=y[i];
	}
}

void* P10(bool *z1){   //�û����� P10
        bool z2[10];
		for(int i=0;i<10;++i){
            z2[i]=z1[P10Table[i]-1];
        }
		for(int i=0;i<10;++i){
            z1[i]=z2[i];
        }
}

bool* P8(bool *z1,int p){   //�û����� P8���ڶ�����������ki
        bool *z2  = roundKey+(p-1)*8;
		for(int i=0;i<8;++i){
            int b=P8Table[i]-1;
            z2[i]=z1[b];
        }
			
        return z2;
}

bool *Shift(bool* keyp) {   //��Կ��չ��ShiftI(LeftshiftI)���ڶ�����������Shift1/Shift2
	
    bool temp1[5];bool temp2[5];  

	for(int j=0;j<5;j++)   //�������������
	{
		temp1[j]=keyp[j];
		temp2[j]=keyp[j+5];
	}
        char x=temp1[0];char y=temp2[0];  //����һλ
        for(int k=0;k<4;k++){
		temp1[k]=temp1[k+1];
		temp2[k]=temp2[k+1];
	    }
	    temp1[4]=x;temp2[4]=y;
	    addarr(keyp,temp1,temp2,10);//�ϲ�
		/* 
        char a=temp1[0],b=temp1[1];
	    char c=temp2[0],d=temp2[1];
	    for(int f=0;f<3;f++)    //������λ
	    { 
		    temp1[f]=temp1[f+2];
		    temp2[f]=temp2[f+2];
	    }
	    temp1[3]=a,temp1[4]=b;
	    temp2[3]=c,temp2[4]=d;
        addarr(keyp,temp1,temp2,10);//�ϲ�
		*/
    
        
	return keyp;
};


bool *K(bool *inputKey) { //��Կ��չ
	bool *output;
	/* shift1֮��inputkey�Ѿ��ı䣬��shift2�Ͷ�ת��һ�Σ��Ͱ�shiftֱ�Ӹĳ�ֻתһ�ξͺ�*/
	P10(inputKey);
    P8(Shift(inputKey),1);  //P8
	P8(Shift(inputKey),2); 
	/*cout<<"key:";
	print(inputKey,10);*/
	return output;
};


//�ӽ��ܲ��ֺ���
bool *IP(bool *input) { //��ʼ�û���IP
	bool *output=input;
	int dir[8]={2,6,3,1,4,8,5,7};//��������
	bool temp[8]={};//��ʱ������
	for (int i = 0; i < 8; i++)
		temp[i] = output[dir[i]-1];
	for (int i = 0; i < 8; i++, input++)
		*input = temp[i];
	/*cout<<"IP:";
	print(output,8);*/
	return output;
};

bool *IP1(bool *input) { //�����û���IP-1
	bool *output=input;
	/*cout<<"IP1input:";
	print(input,8);*/
	int dir[8]={4,1,3,5,7,2,8,6};//��������
	bool temp[8]={};//��ʱ������
	for (int i = 0; i < 8; i++)
		temp[i] = output[dir[i]-1];
	/*cout<<"IP1temp:";
	for(int i=0;i<8;i++){
		cout<<temp[i];
	}*/
	for (int i = 0; i < 8; i++, input++)
		*input = temp[i];
	/*cout<<endl<<"IP1output:";
	print(output,8);*/
	
	return output;
};

bool *EPBox(bool *input) { //�ֺ�����EPBox
	int EPBoxTable[8] = {3, 0, 1, 2, 1, 2, 3, 0};
	bool *output = cache;
	for (int i = 0; i < 8; i++)
		output[i] = input[EPBoxTable[i]];
	/*cout<<"EP:";
	print(output,8);*/
	return input;//���������cache�У�����input�Ա���ִ����һ��
};

bool *SBox(bool *input) { //�ֺ�����SBoxI,
	bool *output = input;
	/*
	for (int k = 0; k < 5; k+=4) {//ʹ��cache�������ݽ��в���
		int num = SBoxTable[i - 1][cache[k] * 2 + cache[k + 3]][cache[k + 1] * 2 + cache[k + 2]]; //ȷ��ת�����ж�Ӧλ��
		output[k] = binaryTable[num][0];
		output[k + 1] = binaryTable[num][1];//���ݻص�output��
	}
	*/
	//ǰ��λ��SBOX1������λ��SBOX2��
	int num = SBoxTable[0][cache[0] * 2 + cache[3]][cache[1] * 2 + cache[2]];
		output[0] = binaryTable[num][0];
		output[1] = binaryTable[num][1];
	int num1 = SBoxTable[1][cache[4] * 2 + cache[7]][cache[5] * 2 + cache[6]];
		output[2] = binaryTable[num1][0];
		output[3] = binaryTable[num1][1];
	/*cout<<"SB:";
	print(output,4);*/
	return output;
};

bool *SPBox(bool *input) { //�ֺ�����SPBox
	bool *output = input;
	bool temp[4] = {};
	int dir[4] = {2, 4, 3, 1};//��������
	for (int i = 0; i < 4; i++)
		temp[i] = output[dir[i]-1];
	for (int i = 0; i < 4; i++, input++)
		*input = temp[i];
	/*cout<<"SP:";
	print(output,4);*/
	return output;
};

bool *addKey(bool *input, bool *inputKey) { //�ֺ����У���������Կ��ӣ��ĵ�δ�ἰ��
	bool *output = cache;//ʹ��cache�����ݽ��в���
	for (int i = 0; i < 8; i++, inputKey++) 
		output[i] = output[i] ^ *inputKey;
	/*cout<<"ADD:";
	print(output,8);*/
	return input;//����input�Ա�ִ����һ�������ݴ洢��cache
};

bool *f(bool *input, int i) { //�ֺ���F,�ڶ�����������fk1/fk2
	bool *output = new bool[8]; // �������ڴ�
    for (int j = 0; j < 8; ++j) {
        output[j] = input[j]; // ����ÿ��Ԫ��
    }
	
	/*cout<<"k"<<i<<":";
	print(roundKey+(i-1)*8,8);*/

	SPBox(SBox(addKey(EPBox(output + 4), roundKey+(i-1)*8)));

	//ע�ͣ�f��Ȼ���뼰�����λ�������ݣ�������������ʱֻ������4λ��
	//-->���������Ľ�������

	for (int i = 0; i < 4; i++) {
		output[i] = output[i] ^ output[i + 4];
	}
	
	//����Ϊԭ�������֣�
	for (int i = 4; i < 8; i++) {
		output[i] = input[i];
	}

	/*cout<<"F:";
	print(output,8);*/
	return output;
};

bool *SW(bool *input) { //SW��ʵ�����Ҳ��ֵĽ������ĵ�δ�ἰ��
	bool *output = input;
	for (int i = 0; i < 4; i++)
		swap(*(output + i), *(output + 4 + i));
	/*cout<<"SW:";
	print(output,8);*/
	return output;
}


bool *C(bool *input) {//�����㷨���������Ϊ����
	K(key);//��������Կ
	return IP1(f(SW(f(IP(input), 1)), 2));
};

bool *P(bool *input) { //�����㷨���������Ϊ������ԭ��Կ
	K(key);//��������Կ
	return IP1(f(SW(f(IP(input), 2)), 1));
};


void dfsOriginalKey(bool* plainText,bool* cipherText, int i,bool* testKey){//���Ѳ�ѯ��Կ����ӡ���п��ܽ���������ֱ�Ϊ���ģ����ģ�������ȣ�������Կ
	if(i==10){
		inputIntoArray(plainText,code,8);
		inputIntoArray(testKey,key,10);
		code=C(code);
		bool correct=true;
		for(int k=0;k<8;k++){
			if(code[k]!=cipherText[k]){
				correct=false;
				break;
			}
		}
		if(correct)
			print(testKey,10);
		return;
	}
	dfsOriginalKey(plainText,cipherText,i+1,testKey);
	testKey[i]=1;
	dfsOriginalKey(plainText,cipherText,i+1,testKey);
	testKey[i]=0;
	return;
}

int main() {
	cout<<"----------------------------\n| welcome to S-DES system!|\n----------------------------\n";
	int choice=-1;
	do{
		cout<<"for encrpt enter 1\nfor decrpt enter2\nfor cracking key enter 3\nfor exit enter 0\n";
		cin>>choice;
		if(choice==0)
			cout<<"You're now exiting!\n"<<endl;
		else if(choice==1){
			cout<<"Do you have 10 digit key? 1 for yes, 2 for no\n";
			int choice2=0;
			do{
				cin>>choice2;
				switch (choice2){
				case 1:
					cout<<"enter key\n";
					bool inputKey[10];
					for(int i=0;i<10;i++)
						cin>>inputKey[i];
					inputIntoArray(inputKey,key,10);
					break;
				case 2:
					key = generateOriginalKey();
					cout<<"a new key has been generate for you! The key is:";
					print(key,10);
					break;
				default:
					cout<<"Please enter a correct choice!\n";
					break;
				}
			}while(choice2!=1&&choice2!=2);
			choice2=0;
			cout<<"Are you entering 8 digit code or text? for code enter 1, for text enter 2\n";
			do{
				cin>>choice2;
				string inputText;
				int n;
				switch (choice2){
				case 1:
					cout<<"Please enter plaintext\n";
					bool plainText[8];
					for(int i=0;i<8;i++)
						cin>>plainText[i];
					inputIntoArray(plainText,code,8);
					code=C(code);
					cout<<"result:";
					print(code,8);
					break;
				case 2:
					cout<<"Please enter plaintext\n";
					cin >> inputText;
					n = inputText.length();
					cout<<"result:";
					for (int i = 0; i < n; i++){
						binary(code,(int)inputText[i]);
						code=C(code);
						cout<<(char)declize(code);
					}
					print(code,8);
					break;
				default:
					cout<<"Please enter a correct choice!\n";
					break;
				}
			}while(choice2!=1&&choice2!=2);
			
		}
		else if(choice==2){
			cout<<"please enter the 10 digit key\n";
			bool inputKey[10];
			for(int i=0;i<10;i++)
				cin>>inputKey[i];
			inputIntoArray(inputKey,key,10);
			cout<<"please enter ciphertext\n";
			bool cipherText[8];
			for(int i=0;i<8;i++)
				cin>>cipherText[i];
			inputIntoArray(cipherText,code,8);
			code=P(code);
			cout<<"result:";
			print(code,8);
		}
		else if(choice==3){
			cout<<"Please enter plaintext\n";
			bool plainText[8];
			for(int i=0;i<8;i++)
				cin>>plainText[i];
			cout<<"please enter ciphertext\n";
			bool cipherText[8];
			for(int i=0;i<8;i++)
				cin>>cipherText[i];
			bool testKey[10]={};
			cout<<"Possible key:\n";
			dfsOriginalKey(plainText,cipherText,0,testKey);
		}
		else
			cout<<"Please enter a correct choice!\n";
	}while(choice!=0);
	/*bool inputKey[10]={1,0,0,0,1,1,1,0,0,1};
	inputIntoArray(inputKey,key,10);
	bool plainText[8]={0,1,0,1,1,0,0,0};//��дX
	inputIntoArray(plainText,code,8);
	key = generateOriginalKey(); //����ԭ��Կ
	code=C(code);
	cout<<"code:";
	print(code,8);
	�����00001011��ֱ�Ʊ��*/
	

	delete cache;
	return 0;
}
