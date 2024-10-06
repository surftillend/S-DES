#include <iostream>
#include <string>
#include <ctime>

using namespace std;
//全局参数
bool *code=new bool(8);//加-解秘的内容
bool *key=new bool(10);//原密钥
bool *roundKey = new bool(16);//轮密钥
bool *cache = new bool(10); //临时储存空间

static bool binaryTable[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};//二进制对应数位 空间换时间
static int SBoxTable[2][4][4] = {{{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 0, 2}}, {{0, 1, 2, 3}, {2, 3, 1, 0}, {3, 0, 1, 2}, {2, 1, 0, 3}}};
static int P10Table[]={3,5,2,7,4,10,1,9,8,6};
static int P8Table[]={6,3,7,4,8,5,10,9};


void print(bool* input, int n){//打印用函数，分别为输入内容指针及数组长度
	for(int i=0;i<n;i++)
		cout<<input[i];
	cout<<endl;
}

bool* binary(bool* input,int n){//二进制化，参数分别代表存储的数组指针及输入的int十进制数字
	bool* output= input;
	for(int i=0,k=128;i<8;i++,k/=2){
		output[i]=n/k;
		n=n-k;
	}
	return output;
}

int declize(bool* input){//十进制化，输入数组指针输出十进制数字
	int n=0;
	for(int i=7,k=1;i>=0;i--,k*=2)
		n+=input[i]*k;
	return n;
}

//工具函数

bool *inputIntoArray(bool* from, bool* to,int length){//测试期间输入原密钥用函数
	for(int i=0;i<length;i++)
		to[i]=from[i];
}

//密钥部分函数
bool *generateOriginalKey() { //生成原密钥的函数（文档中未提及）
	srand((unsigned int)time(NULL));
	for(int i=0;i<10;i++)
		key[i]=rand()%2;
	return key;
}


void addarr(bool *num,bool *x,bool *y,int n){  //合并数组函数 
	n/=2;
	for(int i=0;i<n;i++){
		num[i]=x[i];
		num[i+n]=y[i];
	}
}

void* P10(bool *z1){   //置换函数 P10
        bool z2[10];
		for(int i=0;i<10;++i){
            z2[i]=z1[P10Table[i]-1];
        }
		for(int i=0;i<10;++i){
            z1[i]=z2[i];
        }
}

bool* P8(bool *z1,int p){   //置换函数 P8，第二个参数代表ki
        bool *z2  = roundKey+(p-1)*8;
		for(int i=0;i<8;++i){
            int b=P8Table[i]-1;
            z2[i]=z1[b];
        }
			
        return z2;
}

bool *Shift(bool* keyp) {   //密钥拓展置ShiftI(LeftshiftI)，第二个参数代表Shift1/Shift2
	
    bool temp1[5];bool temp2[5];  

	for(int j=0;j<5;j++)   //拆分左右两部分
	{
		temp1[j]=keyp[j];
		temp2[j]=keyp[j+5];
	}
        char x=temp1[0];char y=temp2[0];  //左移一位
        for(int k=0;k<4;k++){
		temp1[k]=temp1[k+1];
		temp2[k]=temp2[k+1];
	    }
	    temp1[4]=x;temp2[4]=y;
	    addarr(keyp,temp1,temp2,10);//合并
		/* 
        char a=temp1[0],b=temp1[1];
	    char c=temp2[0],d=temp2[1];
	    for(int f=0;f<3;f++)    //左移两位
	    { 
		    temp1[f]=temp1[f+2];
		    temp2[f]=temp2[f+2];
	    }
	    temp1[3]=a,temp1[4]=b;
	    temp2[3]=c,temp2[4]=d;
        addarr(keyp,temp1,temp2,10);//合并
		*/
    
        
	return keyp;
};


bool *K(bool *inputKey) { //密钥拓展
	bool *output;
	/* shift1之后inputkey已经改变，再shift2就多转了一次，就把shift直接改成只转一次就好*/
	P10(inputKey);
    P8(Shift(inputKey),1);  //P8
	P8(Shift(inputKey),2); 
	/*cout<<"key:";
	print(inputKey,10);*/
	return output;
};


//加解密部分函数
bool *IP(bool *input) { //初始置换盒IP
	bool *output=input;
	int dir[8]={2,6,3,1,4,8,5,7};//方向数组
	bool temp[8]={};//临时储存结果
	for (int i = 0; i < 8; i++)
		temp[i] = output[dir[i]-1];
	for (int i = 0; i < 8; i++, input++)
		*input = temp[i];
	/*cout<<"IP:";
	print(output,8);*/
	return output;
};

bool *IP1(bool *input) { //最终置换盒IP-1
	bool *output=input;
	/*cout<<"IP1input:";
	print(input,8);*/
	int dir[8]={4,1,3,5,7,2,8,6};//方向数组
	bool temp[8]={};//临时储存结果
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

bool *EPBox(bool *input) { //轮函数的EPBox
	int EPBoxTable[8] = {3, 0, 1, 2, 1, 2, 3, 0};
	bool *output = cache;
	for (int i = 0; i < 8; i++)
		output[i] = input[EPBoxTable[i]];
	/*cout<<"EP:";
	print(output,8);*/
	return input;//结果储存在cache中，返回input以便于执行下一步
};

bool *SBox(bool *input) { //轮函数的SBoxI,
	bool *output = input;
	/*
	for (int k = 0; k < 5; k+=4) {//使用cache储存数据进行操作
		int num = SBoxTable[i - 1][cache[k] * 2 + cache[k + 3]][cache[k + 1] * 2 + cache[k + 2]]; //确认转换表中对应位置
		output[k] = binaryTable[num][0];
		output[k + 1] = binaryTable[num][1];//数据回到output中
	}
	*/
	//前四位用SBOX1，后四位用SBOX2；
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

bool *SPBox(bool *input) { //轮函数的SPBox
	bool *output = input;
	bool temp[4] = {};
	int dir[4] = {2, 4, 3, 1};//方向数组
	for (int i = 0; i < 4; i++)
		temp[i] = output[dir[i]-1];
	for (int i = 0; i < 4; i++, input++)
		*input = temp[i];
	/*cout<<"SP:";
	print(output,4);*/
	return output;
};

bool *addKey(bool *input, bool *inputKey) { //轮函数中，内容与密钥相加（文档未提及）
	bool *output = cache;//使用cache中数据进行操作
	for (int i = 0; i < 8; i++, inputKey++) 
		output[i] = output[i] ^ *inputKey;
	/*cout<<"ADD:";
	print(output,8);*/
	return input;//返回input以便执行下一步，数据存储在cache
};

bool *f(bool *input, int i) { //轮函数F,第二个参数代表fk1/fk2
	bool *output = new bool[8]; // 分配新内存
    for (int j = 0; j < 8; ++j) {
        output[j] = input[j]; // 复制每个元素
    }
	
	/*cout<<"k"<<i<<":";
	print(roundKey+(i-1)*8,8);*/

	SPBox(SBox(addKey(EPBox(output + 4), roundKey+(i-1)*8)));

	//注释：f仍然输入及输出八位完整内容，但按规则，运行时只处理右4位，
	//-->左四与右四结果做异或

	for (int i = 0; i < 4; i++) {
		output[i] = output[i] ^ output[i + 4];
	}
	
	//右四为原来的数字；
	for (int i = 4; i < 8; i++) {
		output[i] = input[i];
	}

	/*cout<<"F:";
	print(output,8);*/
	return output;
};

bool *SW(bool *input) { //SW，实现左右部分的交换（文档未提及）
	bool *output = input;
	for (int i = 0; i < 4; i++)
		swap(*(output + i), *(output + 4 + i));
	/*cout<<"SW:";
	print(output,8);*/
	return output;
}


bool *C(bool *input) {//加密算法，输入参数为明文
	K(key);//计算轮密钥
	return IP1(f(SW(f(IP(input), 1)), 2));
};

bool *P(bool *input) { //解密算法，输入参数为密文与原密钥
	K(key);//计算轮密钥
	return IP1(f(SW(f(IP(input), 2)), 1));
};


void dfsOriginalKey(bool* plainText,bool* cipherText, int i,bool* testKey){//深搜查询密钥，打印所有可能结果。参数分别为明文，密文，搜索深度，测试密钥
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
	bool plainText[8]={0,1,0,1,1,0,0,0};//大写X
	inputIntoArray(plainText,code,8);
	key = generateOriginalKey(); //生成原密钥
	code=C(code);
	cout<<"code:";
	print(code,8);
	结果：00001011垂直制表符*/
	

	delete cache;
	return 0;
}
