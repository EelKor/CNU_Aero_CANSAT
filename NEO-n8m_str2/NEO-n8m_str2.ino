#include <SoftwareSerial.h>

SoftwareSerial GPS(2,3);  // Lora TX Lora Rx
byte buff[100];
  String positionFix ;
  String lat; double lat_dd;
  String lng; double lng_dd;
  int index, index2, index3, index4,index5,index6;

  
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  GPS.begin(9600);
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  
}

void loop() { // run over and over
  if (GPS.available()) {
    GPS.readBytesUntil('\n',buff,100);
    String msg = buff;
    
    Serial.println(msg);
    if(msg.substring(0,6).equals("$GNGGA") || msg.substring(0,6).equals("$GPGGA"))
    {
     index = msg.indexOf(",");// 첫 번째 콤마 위치
     index2= msg.indexOf(",",index+1); 
     index3 = msg.indexOf(",",index2+1); 
     index4 = msg.indexOf(",",index3+1); 
     index5 = msg.indexOf(",",index4+1);
     index6 = msg.indexOf(",",index5+1);  
     
      positionFix = msg.substring(index6+1,index6+2);
      if (index6<43 || index6>47){positionFix = "0";}

     if(positionFix != "0"){   
      lat = msg.substring(17,29);
      lng = msg.substring(30,43);

     int d_index_lat = lat.indexOf(".");
     int d_index_lng = lng.indexOf(".");

     String d_lat = lat.substring(0, d_index_lat-2);
     String d_lng = lng.substring(0, d_index_lng-2);

     String m_lat = lat.substring(d_index_lat-2,lat.length());
     String m_lng = lng.substring(d_index_lng-2,lng.length());

     int d_lat_int = d_lat.toInt();
     int d_lng_int = d_lng.toInt();

     double m_lat_double = m_lat.toDouble();
     double m_lng_double = m_lng.toDouble();

     lat_dd = d_lat_int + m_lat_double/60;
     lng_dd = d_lng_int + m_lng_double/60;
     }

     else{
      lat = '0'; lng = '0';
      lat_dd = 0; lng_dd = 0;
    }
    }
   else{
    lat = "377"; lng = "377";
    lat_dd = 377; lng_dd = 377;
   }
  Serial.print(msg+'\t');
  Serial.print(String(index)+'\t'+String(index2)+'\t'+String(index3)+'\t'+String(index4)+'\t'+String(index5)+'\t'+String(index6)+'\t'+positionFix+'\t'+lat+'\t'+lng+'\t');
  Serial.println(String(lat_dd)+'\t'+String(lng_dd));
  }
  
}
