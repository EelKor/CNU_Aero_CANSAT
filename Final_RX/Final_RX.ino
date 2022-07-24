#include <SoftwareSerial.h>
 
SoftwareSerial lora(2,3);

int dt; float tem, pa, high, FS;
double angle_x, angle_y, angle_z;
String lat, lng, str1, str2, str3, str4, str5, str6, str7, str8;

  String inString;
  String cmd;



 
void setup()
{
  Serial.begin(9600);
  lora.begin(9600);
  lora.println("AT+PARAMETER=10,7,1,7");
  delay(100);
  lora.println("AT+IPR=9600"); //로라 속도
  delay(100);
  lora.println("AT+ADDRESS=77"); //로라 주소 지정
  delay(100);
  lora.println("AT+NETWORKID=2"); //네트워크 아이디
  delay(100);
  Serial.println("lora setup end");

  //액셀연동
  Serial.println("CLEARDATA");
  Serial.println("Label,dt,FallSpeed,tem,pa,high,angle_x,angle_y,angle_z,lat,lng");
 
}
 
void loop()
{

//수신
  while(lora.available())
  {
    if(lora.available())
    {
     inString = String(lora.readStringUntil('\n'));
     delay(50);
    }
  }

  if(inString.length() > 0)
  {
    int index = inString.indexOf(" ");
     int index2 = inString.indexOf(" ",index+1); 
     int index3 = inString.indexOf(" ",index2+1); 
     int index4 = inString.indexOf(" ",index3+1); 
     int index5 = inString.indexOf(" ",index4+1);
     int index6 = inString.indexOf(" ",index5+1); 
     int index7 = inString.indexOf(" ",index6+1);
     int index8 = inString.indexOf(" ",index7+1);
     int index9 = inString.indexOf(" ",index8+1);

     str1 = inString.substring(0, index);
     str2 = inString.substring(index+1,index2);
     str3 = inString.substring(index2+1,index3);
     str4 = inString.substring(index3+1,index4);
     str5 = inString.substring(index4+1,index5);
     str6 = inString.substring(index5+1,index6);
     str7 = inString.substring(index6+1,index7);
     str8 = inString.substring(index7+1,index8);
     
     dt =str1.toInt(); FS = str2.toFloat();
     tem = str3.toFloat(); pa = str4.toFloat(); high = str5.toFloat();
     angle_x = str6.toDouble(); angle_y = str7.toDouble(); angle_z = str8.toDouble();
     // */

     lat = inString.substring(index8+1,index9);
     lng = inString.substring(index9,inString.length());

     int d_index_lat = lat.indexOf(".");
     int d_index_lng = lng.indexOf(".");

     String d_lat = lat.substring(d_index_lat-2, d_index_lat);
     String d_lng = lng.substring(d_index_lng-2, d_index_lng);

     String m_lat = lat.substring(d_index_lat+1,lat.length());
     String m_lng = lng.substring(d_index_lng+1,lng.length());

     int d_lat_int = d_lat.toInt();
     int d_lng_int = d_lng.toInt();

     double m_lat_double = m_lat.toDouble();
     double m_lng_double = m_lng.toDouble();

     double lat_dd = d_lat_int + m_lat_double/60;
     double lng_dd = d_lng_int + m_lng_double/60;

   // Serial.println(inString);

     String printStr = "DATA,"+ String(dt)+','+String(FS)+','+String(tem)+','+String(pa)+','+String(high)+','
       +String(angle_x)+','+String(angle_y)+','+String(angle_z)+','+String(lat_dd)+','+String(lng_dd);

   Serial.println(printStr);
   }

  //송신
if(Serial.available())
    {
     cmd = String(Serial.readStringUntil('\n'));
     
       if(cmd.length() > 0){
         lora.println("AT+SEND=75,"+String(cmd.length())+","+cmd);
         //Serial.println(cmd);
          delay(50);
       }
    }

     
     
}
