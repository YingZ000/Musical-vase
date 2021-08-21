#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>


//----------------------初始化---------------------------
#define SET(x,y) (x |=(1<<y))        //-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))           // |
#define CHK(x,y) (x & (1<<y))               // |
#define TOG(x,y) (x^=(1<<y))                //-+
#define N 160  //How many frequencies 扫 160 个频率

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;


//int value = 0;//判断按键的值
//int i;//变量


void setup() {//初始化配置函数

  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;  
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin
  pinMode(11,OUTPUT);

  Serial.begin(9600);
  
  mp3_set_serial (Serial); //set Serial for DFPlayer-mini mp3 module
  mp3_set_volume (30);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+

}

void loop() {
  
  int vol=0;
  
  unsigned int d;
  
  int maxnum = 0;
  int maxval = 0;
  
  int counter = 0;
  
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator
    

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results 过滤结果
    
    freq[d] = d;
    
    //results[d]=v;
    
    if (results[d] >= maxval && d>3)
    {
      maxval = results[d];
      maxnum = d;
    }
  }

  
  Serial.print(maxnum);
  Serial.print(" ");
  Serial.println(maxval);

  if(maxval < 400){

    if(maxnum > 70 && maxnum <= 75){
      mp3_play(1);
      delay(1000);
    }

    if(maxnum > 75 && maxnum <= 80){
      mp3_play(2);
      delay(1000);
    }

    else if(maxnum > 80 && maxnum <=85){
      mp3_play(3);
      delay(1000);
    }

    else if(maxnum >85 && maxnum <= 90){
      mp3_play(4);
      delay(1000);
    }

    else if(maxnum >90 && maxnum <=95){
      mp3_play(5);
      delay(1000);
    }
    else if(maxnum >95 && maxnum <=100){
       mp3_play(6);
       delay(1000);
      }
    else if(maxnum >100 && maxnum <=110){
       mp3_play(7);
       delay(1000);
     }
    else{
      mp3_stop ();
    }
  }

  TOG(PORTB, 0); 
}
