#include <SoftwareSerial.h>

#define  BUFF_LEN 200
#define  GPS_TX  13  // Arduino receives on this pin
#define  GPS_RX  11  // Arduino transmits on this pin

enum RecommendedMinimumParam {
  TIME,
  VALIDITY,
  LATITUDE,
  LAT_NORTH_SOUTH,
  LONGITUDE,
  LON_EAST_WEST,
  SPEED,
  TRUE_COURSE,
  DATE,
  VARIATION,
  VAR_EAST_WEST,
  CHECKSUM
};
  
 enum SatellitesInView {
  TOTAL_MSG_COUNT,
  MSG_NUM,
  TOTAL_SATS_IN_VIEW,
  SV_PRN,
  ELEVATION,  // DEG (0-90)
  AZIMUTH,  // DEG (0-359)
  SNR  // DB (0-99)
};

//char buffer[BUFF_LEN] = "";

SoftwareSerial gpsSerial(GPS_TX, GPS_RX);  // configures pin 10 to be RX and pin 11 to be TX

//-----------------------------------------------------------------------------------/
void setup() {
  // open serial communications
  Serial.begin(115200);  // begin serial communications over pins 0 & 1
  gpsSerial.begin(9600);  // begin serial communications over pins 10 & 11
}

//-----------------------------------------------------------------------------------/

char buffer[BUFF_LEN] = "";  // buffer to hold raw GPS data

void loop() 
{ 
  fillGPSDataBuffer(buffer, gpsSerial, BUFF_LEN); // gathering raw GPS data
  String buffStr = String(buffer);  // converting to String

  //Serial.println(buffStr);
  
  // printing the RMC data
  String RMCData = getData(buffStr, "RMC");
  String GSVData = getData(buffStr, "GSV");
  
  Serial.println("---------------------------------------------------------------------");
  Serial.print("RMC Data string: "); Serial.println(RMCData);  
  Serial.print("GSV Data string: "); Serial.println(GSVData);
  Serial.println();
  
  // printing parameters
  Serial.print("Time:         "); Serial.println(parseData(RMCData,TIME));
  Serial.print("Validity:     "); Serial.println(parseData(RMCData,VALIDITY));
  Serial.print("Latitude:     "); Serial.println(parseData(RMCData,LATITUDE)+parseData(RMCData,LAT_NORTH_SOUTH));
  Serial.print("Longitude:    "); Serial.println(parseData(RMCData,LONGITUDE)+parseData(RMCData,LON_EAST_WEST));
  Serial.println();  
  
  Serial.print("Elevation:    "); Serial.println(parseData(GSVData,ELEVATION));
  Serial.print("Azimuth:      "); Serial.println(parseData(GSVData,AZIMUTH));
  Serial.print("Sats in view: "); Serial.println(parseData(GSVData,TOTAL_SATS_IN_VIEW));
  Serial.println("---------------------------------------------------------------------");
  //delay(2000);
}

//-----------------------------------------------------------------------------------/
// Returns a string containing the RMC data (Recommended Minimum data) which contains
// the data specified in the RecommendedMinimumParam enum list

String getData(String buffString, String DataName)
{
  Serial.println();
//  int startIndex = buffString.indexOf("$GPRMC");
  int startIndex = buffString.indexOf("$GP" + DataName);
  String temp = buffString.substring(startIndex + 1); // chopping off '$'
  int endIndex = temp.indexOf('$');  
//  int endIndex = buffString.lastIndexOf("$GPVTG");

  // error handling
  if (startIndex == -1 || endIndex == -1)  // if string not found...
  {
    Serial.println("Error: String not found"); 
    //Serial.println(); 
    return "";
  }
  if (startIndex == endIndex)  // if only one instance of the string found...
  {
    Serial.println("Error: String only found once"); 
    //Serial.println();  
    return "";
  }  
  
  return temp.substring(0, endIndex);
  //String RMCData = buffString.substring(startIndex, endIndex);  // getting the relevant substring
  
  ///return RMCData;
}

//-----------------------------------------------------------------------------------/
// Parses NMEA data and returns strings containing the desired parameter as
// specified by the enum value ctr corresponding to those in the relevant ENUM

String parseData(String data, int ctr){
    String temp = data.substring(data.indexOf(',') + 1);
    for (int i = 0; i < ctr; i++)
    {
       temp = temp.substring(temp.indexOf(',') + 1);
    }
      return temp.substring(0, temp.indexOf(','));
      /*
      if (ctr == CHECKSUM)  // getting rid of the newline character
      return temp.substring(0, temp.indexOf('\n'));
    else 
      return temp.substring(0, temp.indexOf(','));
      */
}


//-----------------------------------------------------------------------------------/
// fills the char array buffer of size BUFF_LEN with incoming data from the GPS.
// gpsSerial is the SoftwareSerial object used to gather data over UART and
// buffer is the array storing the data.

void fillGPSDataBuffer(char * buffer, SoftwareSerial &gpsSerial, int bufferLength)
{
  for (int i = 0; i < bufferLength; i++)
  {
    while (!(gpsSerial.available()))
      ;
    buffer[i] = gpsSerial.read();
  }
}
















