#include<TimerOne.h>
//#include <AccelStepper.h>
// #include<math.h>

////////////////////////////////////////// Động cơ 1
const char enco1=2; //Chan ngat
const char enco2=5; //Chan doc encoder chan nao cung dc
const char in1=12; //Chan IN1 in1=12
const char in2=13; //Chan IN2 in2=13
float vitri1, vitridat1,vitritam;
float T1 = 0.01, E1=0, E1_1=0, E1_2=0, alpha1=0, beta1=0, gamma1=0;
long xung1 = 0;
float Output1=0, LastOutput1=0;
float Kp1 = 4.3, Kd1 = 0.12, Ki1=0.04;   //4.4   0.15  0.4
////////////////////////////////////////// Động cơ 2,3
const char en = 8;
const char dirY = 6; // dirY dirY = 6
const char stepY = 3; // stepY stepY = 3
const char dirZ = 7; // dirZ
const char stepZ = 4; // stepZ
const char namcham = 9; //nam cham dien
unsigned char DHT = 0;
unsigned char uu = 0;
unsigned char Start_mode = 0;
unsigned char Stop_mode = 0;
unsigned int delay1 = 5;
//////////////// cac cam bien
unsigned long x,y,z;
#define limitX A0 //limitX
#define limitY A2 //limitY
#define limitZ A1 //limitZ

unsigned long nStepY;
unsigned long nStepZ;

float Goc_DC2 = 0,tt=0,mm=0,dem=0;
float Goc_DC3 = 0;

float Goc_QKhu_DC2 = 0;
float Goc_QKhu_DC3 = 0;

float Goc_HTai_DC2 = 0;
float Goc_HTai_DC3 = 0;

unsigned char CBX;
unsigned char CBY;
unsigned char CBZ;

String inString = "";

void demxung1()
{
  if(Start_mode==1)
  {
  if(digitalRead(enco2) == 1 ) xung1++;
  else xung1--;
  }
}

void PID()
{
////////////////////////////////////////// Động cơ 1
  //Serial.println(xung1);
   //delay(800);
  if(Start_mode==1)
  {
        vitri1 = (xung1*8)/(168*11);// buoc xoan 1 vong di duoc 8mm can 168tisotruyen*11soxungencoder
        E1 = vitridat1 - vitri1;
        alpha1 = 2*T1*Kp1 + Ki1*T1*T1 + 2*Kd1;
        beta1  = T1*T1*Ki1 - 4*Kd1 - 2*T1*Kp1;
        gamma1 = 2*Kd1;
        Output1 = (alpha1*E1 + beta1*E1_1 + gamma1*E1_2 + 2*T1*LastOutput1)/(2*T1);
        LastOutput1 = Output1;
        E1_2=E1_1;
        E1_1=E1;
        if(Output1 > 2)
        {
          digitalWrite(in1,HIGH);
          digitalWrite(in2,LOW);
          //mm=0;
        }
        else if(Output1 < -2)
        {
          digitalWrite(in1,LOW);
          digitalWrite(in2,HIGH);
          //mm=0;
        }
        else
        {
          digitalWrite(in1,LOW);
          digitalWrite(in2,LOW);
          //tt=1;
          //mm++;
        }
  }
}
void setup() 
{
  Serial.begin(9600);

  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(enco1, INPUT_PULLUP);
  pinMode(enco2, INPUT_PULLUP); 

  attachInterrupt(0,demxung1,RISING);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(PID);

  pinMode(en,OUTPUT);
  pinMode(stepY,OUTPUT);
  pinMode(dirY,OUTPUT);
  pinMode(stepZ,OUTPUT);
  pinMode(dirZ,OUTPUT);
  pinMode(namcham,OUTPUT);
  pinMode(limitX, INPUT_PULLUP);
  pinMode(limitY, INPUT_PULLUP);
  pinMode(limitZ, INPUT_PULLUP);

  digitalWrite(en,1); 
  digitalWrite(dirY,1);
  digitalWrite(dirZ,1);
  digitalWrite(namcham,0);
  
  Serial.println("Vui long chon che do hoat dong: ");

}

void loop() 
{    
  while (Serial.available()) // <=> while (Serial.available()==1)
   {
     Chuc_nang(3.75);// ti so truyen 60/16/1=3.75 chia 1 vi ko dung vi buoc
     delay(50);
     if(DHT==1)
     {
       MoveStep123_FK(nStepY, stepY,  nStepZ, stepZ);
     }
   }
}

void MoveStep123_FK(unsigned long nStepY, const char stepY, unsigned long nStepZ, const char stepZ) //ten bien giong nhau nhung day la bien cuc bo
{
  Serial.print("docao1: "); Serial.print(vitridat1); Serial.println(" mm");
  Serial.print("theta2: "); Serial.print(Goc_HTai_DC2); Serial.println(" do");
  Serial.print("theta3: "); Serial.print(Goc_HTai_DC3); Serial.println(" do"); 
    
    for(x = 1; x <= nStepZ; x++)//Quay den vi tri Z
    {
       digitalWrite(stepZ, HIGH);
       delay(6);  //12
       digitalWrite(stepZ, LOW);
       delay(6);
    }  

    for(x = 1; x <= nStepY; x++)//Quay den vi tri Y
    {
       digitalWrite(stepY, HIGH);
       delay(6);
       digitalWrite(stepY, LOW);
       delay(6);
     }
    for(y=0;y<=1000;y++)
    {
      delay(2);
      vitridat1=vitritam;
    }
    y=0;
    for(;;)
    {
      //Serial.println("DANG DOI");
      if((digitalRead(in1)==0)&&(digitalRead(in2)==0))
      {
         break;
      }  
    }
    // ///////////////////////////////////////////////////////////////HUT THA VAT
              delay(1800);
              digitalWrite(namcham,1);
                  //////////THIET LAP THONG SO
                vitritam = 35; //ép kiểu chuỗi về dang số     20
                for(z=0;z<=1000;z++)
                {
                 delay(2);
                 vitridat1=vitritam;
                }
                z=0;
                for(;;)      // DOI
                {
                  //Serial.println("DANG DOI");
                  if((digitalRead(in1)==0)&&(digitalRead(in2)==0))
                  {
                   break;
                  }  
                }

                  inString="53.4";
                  if (Goc_QKhu_DC2 >= inString.toFloat())  
                  {
                    Goc_HTai_DC2 = inString.toFloat();
                    Goc_DC2 = abs(Goc_HTai_DC2 - Goc_QKhu_DC2);
                    nStepY = Goc_DC2 / (360.0 / (200.0 * 3.75)); // 1 buoc 1.8 do quay 360 do can 200buoc*3.75tisotruyen
                    digitalWrite(dirY, 0);
                    Goc_QKhu_DC2 = Goc_HTai_DC2 ;
                  }
                  else          
                  {
                    Goc_HTai_DC2 = inString.toFloat();
                    Goc_DC2 = abs(Goc_HTai_DC2 - Goc_QKhu_DC2);
                    nStepY = Goc_DC2 / (360.0 / (200.0 * 3.75));
                    digitalWrite(dirY, 1);
                    Goc_QKhu_DC2 = Goc_HTai_DC2 ;
                  }
                  inString = "";
                  
                   inString="72.4";
                if (Goc_QKhu_DC3 >= inString.toFloat()) 
                {
                  Goc_HTai_DC3 = inString.toFloat();
                  Goc_DC3 = abs(Goc_HTai_DC3 - Goc_QKhu_DC3);
                  nStepZ = Goc_DC3 / (360.0 / (200.0 * 3.75));
                  digitalWrite(dirZ, 0);
                  Goc_QKhu_DC3 = Goc_HTai_DC3 ;
                }
                else  
                {
                  Goc_HTai_DC3 = inString.toFloat();
                  Goc_DC3 = abs(Goc_HTai_DC3 - Goc_QKhu_DC3);
                  nStepZ = Goc_DC3 / (360.0 / (200.0 * 3.75));
                  digitalWrite(dirZ, 1);
                  Goc_QKhu_DC3 = Goc_HTai_DC3 ;
                }
                inString = "";

                delay(100);
                //////////////////QUAY HUT THA
               
              
          

              for(x = 1; x <= nStepY; x++)//Quay den vi tri Y
              {
                digitalWrite(stepY, HIGH);
                delay(6);
                digitalWrite(stepY, LOW);
                delay(6);
              }

                for(x = 1; x <= nStepZ; x++)//Quay den vi tri Z
               {
                digitalWrite(stepZ, HIGH);
                delay(6);
                digitalWrite(stepZ, LOW);
                delay(6);
               }  
              
              delay(1800);
              digitalWrite(namcham,0); 
    ////////////////////////////////////////////////////////HUT THA VAT
    vitridat1 = 35;        //20
    DHT=0;
    Serial.println("Chay xong dong hoc thuan ");
    
}

void Chuc_nang(float SoBuoc)
{
    String inChar = Serial.readString();
    delayMicroseconds(2);
    
    if ((inChar[0] == 'S') && (Start_mode == 0)) //set home 
    {
        Serial.println("Dang set home");  
        digitalWrite(en,0);   
        digitalWrite(dirY,1);   //quay nguoc chieu kim dong ho(chieu duong)
        digitalWrite(dirZ,0);   //quay cung chieu kim dong ho(chieu am)

        DHT = 0;
        Goc_DC2 = 0;
        Goc_DC3 = 0;

        Goc_QKhu_DC2 = 0;
        Goc_QKhu_DC3 = 0;

        Goc_HTai_DC2 = 0;
        Goc_HTai_DC3 = 0;

        for(;;)   //Doi den khi cham cam bien X // CHAY LEN
        {
          digitalWrite(in1,LOW); // CHAY LEN
          digitalWrite(in2,HIGH);
          //digitalWrite(in1,HIGH); // CHAY XUONG
          // digitalWrite(in2,LOW);
          // 
          CBX = digitalRead(limitX);
          if(CBX==1) 
          {
          digitalWrite(in1,LOW);
          digitalWrite(in2,LOW);
          xung1 = 0;
          break;
          }
        }

        for(x = 1; x <= 2920 ; x++)  //Doi den khi cham cam bien Z
        {
            CBZ = digitalRead(limitZ);
            if(CBZ==1) break;
            digitalWrite(stepZ,HIGH); 
            delay(6); 
            digitalWrite(stepZ,LOW); 
            delay(6); 
        }
          digitalWrite(dirZ,1);
        for(x = 1; x <= 175 ; x++)  //Quay den vi tri Sethome Z GOC 80 DO 80/(360/(200*3.75))=167
        {
            digitalWrite(stepZ,HIGH); 
            delay(6); 
            digitalWrite(stepZ,LOW); 
            delay(6); 
        }

        for(x = 1; x <= 1460 ; x++)  //Doi den khi cham cam bien Y
        {
            CBY = digitalRead(limitY);
            if(CBY==1) break;
            digitalWrite(stepY,HIGH); 
            delay(delay1); 
            digitalWrite(stepY,LOW); 
            delay(delay1);
        }
        digitalWrite(dirY,0);
        for(x = 1; x <= 278; x++)  //Quay den vi tri Sethome Y GOC 125 DO 125/(360/(200*3.75))=260
        {
            digitalWrite(stepY,HIGH); 
            delay(delay1); 
            digitalWrite(stepY,LOW); 
            delay(delay1);
        }

        Serial.println("Hoan thanh Set home");
        Serial.println("Robot mode: ON");
        Start_mode++;
        xung1 = 0;
        Stop_mode = 0;
    }
    // Trang thai STOP
    else if((inChar[0]=='T')&&(Stop_mode == 0))
    {
      digitalWrite(en,1);
      Serial.println("Robot mode: OFF");
      digitalWrite(namcham,0);
      Start_mode = 0;
      Stop_mode++;
      vitridat1=0;
      E1_1=0;
      E1_2=0;
      LastOutput1=0;
      nStepY=0;
      nStepZ=0;
      DHT = 0;   
    }

    // KÍ TỰ F
    // dong hoc thuan
    else if(inChar[0]=='F')
    {
      Serial.println("Dang chay dong hoc thuan");
      tt=0; DHT=1;
      inString = "";
    
      for(x = 1; x<inChar.length(); x++)
      {
       
        if((inChar[x] == '-') || (inChar[x] == '.'))   
        {
           inString += (char)inChar[x];  //inString = inString + (char)inChar[x];
        }
        else if(isDigit(inChar[x])) // <=>if (isDigit(inChar[x])==1)
        { 
            // Hàm kiểm tra xem ký tự đã truyền có phải chữ số thập phân ko
            // Chuyển đổi số đó thành ký tự và thêm vào chuỗi. 
            inString += (char)inChar[x];  //inString = inString + (char)inChar[x];
        }  

    // KÍ TỰ A
        if(inChar[x] == 'A')
        {
            // vitritam = abs(inString.toFloat()); //ép kiểu chuỗi về dang số
            vitritam  = abs(inString.toFloat()); //ép kiểu chuỗi về dang số   // DOI
            inString = ""; 
        }

    // KÍ TỰ B
        else if (inChar[x] == 'B')
        {
        if (Goc_QKhu_DC2 >= inString.toFloat())  
        {
           Goc_HTai_DC2 = inString.toFloat();
           Goc_DC2 = abs(Goc_HTai_DC2 - Goc_QKhu_DC2);
           nStepY = Goc_DC2 / (360.0 / (200.0 * SoBuoc)); // 1 buoc 1.8 do quay 360 do can 200buoc*3.75tisotruyen
           digitalWrite(dirY, 0);
           Goc_QKhu_DC2 = Goc_HTai_DC2 ;
        }
        else          
        {
          Goc_HTai_DC2 = inString.toFloat();
          Goc_DC2 = abs(Goc_HTai_DC2 - Goc_QKhu_DC2);
          nStepY = Goc_DC2 / (360.0 / (200.0 * SoBuoc));
          digitalWrite(dirY, 1);
          Goc_QKhu_DC2 = Goc_HTai_DC2 ;
        }
          inString = "";
        }

    // KÍ TỰ C
    else if (inChar[x] == 'C') 
    {
      if (Goc_QKhu_DC3 >= inString.toFloat()) 
      {
        Goc_HTai_DC3 = inString.toFloat();
        Goc_DC3 = abs(Goc_HTai_DC3 - Goc_QKhu_DC3);
        nStepZ = Goc_DC3 / (360.0 / (200.0 * SoBuoc));
        digitalWrite(dirZ, 0);
        Goc_QKhu_DC3 = Goc_HTai_DC3 ;
      }
      else  
      {
        Goc_HTai_DC3 = inString.toFloat();
        Goc_DC3 = abs(Goc_HTai_DC3 - Goc_QKhu_DC3);
        nStepZ = Goc_DC3 / (360.0 / (200.0 * SoBuoc));
        digitalWrite(dirZ, 1);
        Goc_QKhu_DC3 = Goc_HTai_DC3 ;
      }
      inString = "";
     }
    }

  }
    
  else if(inChar[0] == 'H')
  {
      Serial.println("Hut vat");
      DHT = 0;
      digitalWrite(namcham,1);
      delay(20);
      inString = "";
  }
  else if(inChar[0] == 'N')
  {
      Serial.println("Nha vat");
      DHT = 0;
      digitalWrite(namcham,0);
      delay(20);
      inString = "";
  } 
}
