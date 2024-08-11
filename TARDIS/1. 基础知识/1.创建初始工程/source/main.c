
void delay(int count){
	while(--count);
}

int flag;

int main(void){
	while(1){
		flag = 1;
		delay(100);
		flag = 0;
		delay(100);	
	}
}

